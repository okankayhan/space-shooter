#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include "HelloWorldScene.h"
#include "GameScene.h"
USING_NS_CC;

Scene* MainMenu::createScene()
{
	return MainMenu::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainMenu::init(){
	// 1. super init first
	if (!Scene::init())	{
		return false;
	}

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	auto playButton = cocos2d::ui::Button::create();
	playButton->setTitleText("Play");
	float x = origin.x + visibleSize.width / 2 - playButton->getContentSize().width / 2;
	float y = origin.y + visibleSize.height / 2 - playButton->getContentSize().height / 2;
	playButton->setPosition(Vec2(x, y));
	playButton->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			playCallback(sender);
			break;
		default:
			break;
		}
	});
	this->addChild(playButton);
	log("MainMenu Scene initialized");
	return true;
}

void MainMenu::playCallback(Ref* pSender){
	auto director = Director::getInstance();
	auto gameScene = Game::createScene();
	director->replaceScene(gameScene);
}

void MainMenu::update(float delta) {
	//do update stuff
}
