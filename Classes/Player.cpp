#include "Player.h"
#include "Bullet.h"
USING_NS_CC;

Sprite* Player::createSprite() {
	log("creating player");
	return Player::create();
}

bool Player::init() {
	if (!Sprite::init()) {
		return false;
	}

	this->health = 3;
	this->fireRate = 3;
	this->setTexture("player.png");
	this->fireTimer = 0;
	this->setName("player");

	//poolbullets
	for (int i = 0; i < 20; i++) {
		auto bullet = Bullet::createSprite();
		bullet->setVisible(false);
		auto name = "bullet" + std::to_string(i);
		bullet->setName(name);
		this->bullets.pushBack(bullet);
	}
	
	//move
	initControls();

	this->scheduleUpdate();
	return true;
}

void Player::update(float delta) {
	fireTimer += delta*1000;
	//fire every 1000 / fireRate ms
	if (fireTimer >= 1000 / fireRate) {
		fireTimer = 0;
		fire();
	}
}

void Player::fire() {
	Sprite* bullet;
	//find bullet to fire from pool
	for (auto & elem : this->bullets) {
		if (!elem->isVisible()) {
			bullet = elem;
			break;
		}
	}
	if (bullet == nullptr) {
		log("No bullets left :(");
		return;
	}
	//spawn bullet from the pool
	bullet->setVisible(true);
	auto playerPos = this->getPosition();
	this->getParent()->addChild(bullet);
	//calculate bullet padding
	auto bulletPadding = (this->getBoundingBox().size.height / 2) + (bullet->getBoundingBox().size.height / 2);
	bullet->setPosition(Vec2(playerPos.x,playerPos.y+bulletPadding));
	//make sure bullets go out of screen
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	
	auto moveBy = MoveBy::create(1, Vec2(0, origin.y+visibleSize.height));
	auto callbackMoveBy = CallFunc::create([bullet]() {
		//return used bullet to pool
		bullet->setVisible(false);
		bullet->removeFromParent();
	});
	auto sequence = Sequence::create(moveBy, callbackMoveBy, nullptr);
	bullet->runAction(sequence);
}

/**
 *	Initialize Touch Controls
*/
void Player::initControls() { 
	auto moveSpeed = 250; //250 per sec
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
		auto loc = touch->getLocation();
		//left and right touch area calc
		auto rightBoundary = (origin.x + visibleSize.width)*0.7f;
		auto leftBoundary = (origin.x + visibleSize.width)*0.3f;
		//calculate limits player can move in
		auto rightLimit = origin.x + visibleSize.width - getBoundingBox().size.width / 2;
		auto leftLimit = origin.x + getBoundingBox().size.width / 2;
		auto pos = this->getPosition();
		if (loc.x >= rightBoundary) {//moveto right limit
			auto time = abs(rightLimit - pos.x) / moveSpeed;
			auto moveTo = MoveTo::create(time, Vec2(rightLimit, pos.y));
			runAction(moveTo);
		}
		else if (loc.x <= (origin.x + visibleSize.width)*0.3f) {//moveto left limit
			auto time = abs(leftLimit - pos.x) / moveSpeed;
			auto moveTo = MoveTo::create(time, Vec2(leftLimit, pos.y));
			runAction(moveTo);
		}
		return true;
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event* event) {//stop moving
		this->stopAllActions();
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}
