#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"

class MainMenu : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init() override;
	CREATE_FUNC(MainMenu);

	void update(float) override;
	void playCallback(cocos2d::Ref* pSender);
};

#endif 