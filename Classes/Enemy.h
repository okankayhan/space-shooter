#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

class Enemy : public cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createSprite();
	virtual bool init() override;
	CREATE_FUNC(Enemy);
	void despawn();
	void spawn();
	float delay = 1.0f;
	float animationTime = 0.5f;
	void update(float) override;
private:
	void checkGameOverCondition();
	void initMovement();
	int health;
	int fireRate;
};

#endif 