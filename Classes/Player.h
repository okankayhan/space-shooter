#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player : public cocos2d::Sprite
{
public:
	float health;
	static cocos2d::Sprite* createSprite();
	virtual bool init() override;
	CREATE_FUNC(Player);

	virtual void update(float) override;
	void fire();
private:
	float fireRate;
	float fireTimer;
	void initControls();
	cocos2d::Vector<cocos2d::Sprite*> bullets;
};

#endif 