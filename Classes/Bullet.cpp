#include "Bullet.h"
#include "Enemy.h"
USING_NS_CC;

Sprite* Bullet::createSprite() {
	return Bullet::create();
}

bool Bullet::init() {
	if (!Sprite::init()) {
		return false;
	}

	this->damage = 50;
	this->setTexture("bullet.png");
	auto rect = this->getTextureRect();

	auto physicsBody = PhysicsBody::createBox(rect.size, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setCategoryBitmask(0x02);
	physicsBody->setCollisionBitmask(0x01);
	physicsBody->setContactTestBitmask(0x01);
	physicsBody->setDynamic(false);
	this->addComponent(physicsBody);

	return true;
}
