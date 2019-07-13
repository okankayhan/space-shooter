#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createSprite();
	virtual bool init() override;
	CREATE_FUNC(Bullet);

	int damage;
};

#endif 