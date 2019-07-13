#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "cocos2d.h"

class Background : public cocos2d::Sprite
{
public:
	static cocos2d::Sprite* createSprite();
	virtual bool init() override;
	CREATE_FUNC(Background);
private:
};

#endif 