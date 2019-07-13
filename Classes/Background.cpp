#include "Background.h"
USING_NS_CC;

Sprite* Background::createSprite() {
	return Background::create();
}

bool Background::init() {
	//call super first
	if (!Sprite::init()) {
		return false;
	}
	setTexture("background.png");

	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	auto imageSize = getContentSize();
	Vec2 origin = director->getVisibleOrigin();
	setAnchorPoint(Vec2(0, 0));
	setScaleY(visibleSize.height / imageSize.height);
	setScaleX(visibleSize.width / imageSize.width);

	auto moveBy = MoveBy::create(5, Vec2(0, -(imageSize.height*getScaleY())));
	auto callback = CallFunc::create([=]() {
		auto pos = this->getPosition();
		this->setPosition(origin.x,pos.y+imageSize.height*getScaleY());
	});
	auto sequence = Sequence::create(moveBy, callback, nullptr);
	auto intervalAction = RepeatForever::create(sequence);
	this->runAction(intervalAction);
	return true;
}


