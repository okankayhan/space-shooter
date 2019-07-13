#include "MainMenuScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
USING_NS_CC;

unsigned int score;

Scene* GameOver::createScene(unsigned int tempScore)
{	
	score = tempScore;
	return GameOver::create();
}

// on "init" you need to initialize your instance
bool GameOver::init() {
	// 1. super init first
	if (!Scene::init()) {
		return false;
	}
	
	initializeButtons();
	
	return true;
}

void GameOver::initializeButtons() {
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	auto middleX = origin.x + visibleSize.width / 2;
	//gameover label
	auto gameoverLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 24);

	//score label
	auto scoreLabel = Label::createWithTTF("Score: 1234567890", "fonts/Marker Felt.ttf", 16);
	scoreLabel->setString("Score: " + std::to_string(score));
	//restart game button
	auto retryButton = cocos2d::ui::Button::create();
	retryButton->setTitleText("Retry");
	retryButton->addTouchEventListener([=](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			this->retryCallback(sender);
			break;
		default:
			break;
		}
	});
	//return to main menu button
	auto returnButton = cocos2d::ui::Button::create();
	returnButton->setTitleText("Main Menu");
	returnButton->addTouchEventListener([=](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			this->returnCallback(sender);
			break;
		default:
			break;
		}
	});

	gameoverLabel->setPosition(Vec2(middleX, origin.y + visibleSize.height*0.9f));
	scoreLabel->setPosition(Vec2(middleX, origin.y + visibleSize.height*0.7f));
	retryButton->setPosition(Vec2(middleX, origin.y + visibleSize.height / 2));
	returnButton->setPosition(Vec2(middleX, origin.y + visibleSize.height*0.3f));
	this->addChild(gameoverLabel);
	this->addChild(scoreLabel);
	this->addChild(retryButton);
	this->addChild(returnButton);

}

void GameOver::retryCallback(Ref* pSender) {
	auto director = Director::getInstance();
	auto gameScene = Game::createScene();
	director->replaceScene(gameScene);
}

void GameOver::returnCallback(Ref* pSender) {
	auto director = Director::getInstance();
	auto mainMenuScene = MainMenu::createScene();
	director->replaceScene(mainMenuScene);
}
