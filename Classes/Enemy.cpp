#include "Enemy.h"
#include "GameScene.h"
USING_NS_CC;

static const float _animationTime = 0.5f;
static const float _delay = 10.0f;

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

	physicsBody->setCategoryBitmask(0x01);
	physicsBody->setCollisionBitmask(0x02);
	physicsBody->setContactTestBitmask(0x02);
	physicsBody->setDynamic(false);
	this->addComponent(physicsBody);
	this->setScale(0.5);
	return true;
}

void Enemy::update(float dt) {
	checkGameOverCondition();
}

//Useful when/if I decide to add health system to player
//TODO: add physics body to limit to check collision instead
void Enemy::checkGameOverCondition() {
	auto gameScene = (Game*)this->getScene();
	if (this->getPosition().y + this->getBoundingBox().size.height/2 <= gameScene->limitY) { //when enemy touches the limit game over
		gameScene->despawnEnemy(this);
		gameScene->endGame();
	}
}

void Enemy::initMovement() {
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	auto pos = this->getPosition();
	auto size = this->getBoundingBox().size;
	auto moveByY = (origin.y - size.height / 2) - pos.y;
	auto moveBy = MoveBy::create(_animationTime, Vec2(0, -size.height*2));
	auto delay = DelayTime::create(_delay);
	auto seq = Sequence::create(moveBy, delay, nullptr);
	auto intervalAction = RepeatForever::create(seq);
	this->runAction(intervalAction);
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
