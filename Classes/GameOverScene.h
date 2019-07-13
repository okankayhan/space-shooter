#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__
#include "ui/CocosGUI.h"
#include "cocos2d.h"

class GameOver : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene( unsigned int tempScore);
	virtual bool init() override;
	CREATE_FUNC(GameOver);
private:
	void initializeButtons();
	void retryCallback(Ref * pSender);
	void returnCallback(Ref * pSender);

};

#endif 