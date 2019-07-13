#include "Enemy.h"
#include "GameScene.h"
USING_NS_CC;

static const float _animationTime = 0.5f;
static const float _delay = 5.0f;

Sprite* Enemy::createSprite() {
	return Enemy::create();
}

bool Enemy::init() {
	if (!Sprite::init()) {
		return false;
	}

	this->delay = _delay;
	this->animationTime = _animationTime;
	this->health = 0;
	this->fireRate = 2;
	this->setTexture("enemy64.png");
	auto size = this->getContentSize();
	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setCategoryBitmask(0x1);
	physicsBody->setContactTestBitmask(0x2);
	physicsBody->setDynamic(false);
	this->addComponent(physicsBody);
	this->setScale(0.5);

	return true;
}

void Enemy::initMovement() {
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	auto pos = this->getPosition();
	auto size = this->getBoundingBox().size;
	auto moveByY = (origin.y - size.height / 2) - pos.y;

	//Move Down
	auto moveBy = MoveBy::create(_animationTime, Vec2(0, -size.height*2));
	auto delay = DelayTime::create(_delay);
	auto seq = Sequence::create(moveBy, delay, nullptr);
	
	//Swing left and right
	auto moveLeft = MoveBy::create(1, Vec2(-size.width,0));
	auto moveRight = MoveBy::create(2, Vec2(size.width*2,0));
	auto swingSequence = Sequence::create(moveLeft, moveRight, moveLeft, nullptr);
	
	auto intervalAction = RepeatForever::create(seq);
	auto intervalSwing = RepeatForever::create(swingSequence);
	
	this->runAction(intervalAction);
	this->runAction(intervalSwing);
}

void Enemy::despawn() {
	this->setVisible(false);
	this->stopAllActions();
	this->removeFromParent();
}

void Enemy::spawn() {
	this->setVisible(true);
	this->initMovement();
}
