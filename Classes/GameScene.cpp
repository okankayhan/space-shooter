#include "GameScene.h"
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include "Background.h"
#include "MainMenuScene.h"
#include "GameOverScene.h"
USING_NS_CC;

Scene* Game::createScene() {
	
	auto scene = Game::create();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_SHAPE); //debug physicsbody
	return scene;
}

bool Game::init() {

	if (!Scene::initWithPhysics()) {
		return false;
	}
	this->score = 0;
	
	/**initialize enemy pool
	 * value can be alterad to a dynamic one eg. whats the max #of enemies there can be on the screen. 
	*/
	this->initializeEnemyPool(160);
	this->initializeGrid();
	//initialize gameobjects
	this->initBackground();
	this->initLimit();
	this->spawnPlayer();
	this->spawnEnemy(5);
	this->scoreLabel = initScoreBoard();
	
	//contact event
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	
	//update loop
	this->scheduleUpdate();

	log("Game scene initialized");
	return true;
}

void Game::update(float delta) {
	enemySpawnTimer += delta;
	auto enemy = (Enemy*)enemyPool.at(0); //get any instance of enemy
	if (enemySpawnTimer >= enemy->delay+enemy->animationTime) {
		spawnEnemy(1); //cannot be larger than grid row size
		enemySpawnTimer = 0; //reset the timer
	}
}

/**
* Set up a limit line as a game over condition
*/
void Game::initLimit() {
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	auto lineStartX = origin.x;
	auto lineEndX = origin.x + visibleSize.width;
	auto lineY = origin.y + visibleSize.height*0.15f;
	auto lineWidth = 10;
	auto draw = DrawNode::create(); 
	auto size = Size(visibleSize.width, lineWidth);
	this->addChild(draw);
	draw->drawLine(Vec2(lineStartX,lineY), Vec2(lineEndX,lineY),Color4F(Color3B::WHITE,0.5f));
	auto node = Node::create();
	node->setPosition(300, 300);
	auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(0.1f, 1.0f, 0.0f));
	node->setContentSize(Size(abs(lineEndX - lineStartX), lineWidth));
	node->setPosition(Vec2(origin.x + visibleSize.width / 2, lineY));
	node->setAnchorPoint(Vec2(0.5f,0.5f));
	physicsBody->setCategoryBitmask(0x3);
	physicsBody->setContactTestBitmask(0x1);
	physicsBody->setDynamic(false);
	node->addComponent(physicsBody);
	this->addChild(node);
	this->limitY = lineY+lineWidth/2;
}

void Game::addScore(int scoreToAdd) {
	this->score += scoreToAdd;
	this->scoreLabel->setString("Score: " + std::to_string(this->score));
}

void Game::initializeEnemyPool(int amount) {
	for (int i = 0; i < amount; i++) {
		auto enemy = Enemy::createSprite();
		enemy->setVisible(false);
		auto name = "enemy" + std::to_string(i);
		enemy->setName(name);
		this->enemyPool.pushBack(enemy);
	}
}

/**
 * Spawn row(s) of enemies
*/
void Game::spawnEnemy(int amountOfRow) {
	//log("%d %d", enemyPool.size(), spawnedEnemies.size()); //debug poolsize
	for (int r = 0; r < amountOfRow; r++){
		for (int c = 0; c < 10; c++) {
			auto enemy = (Enemy*)enemyPool.back();
			spawnedEnemies.pushBack(enemy);
			enemyPool.popBack();
			auto pos = this->grid.at(r).at(c);
			enemy->setPosition(pos);
			this->addChild(enemy);
			enemy->spawn();
		}
	}
}

void Game::despawnEnemy(Sprite* enemy) {
	auto e = (Enemy*)enemy;
	enemyPool.pushBack(e);
	spawnedEnemies.eraseObject(e);
	e->despawn();
}

void Game::spawnPlayer() {
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	this->player = Player::createSprite();
	this->player->setPosition(origin.x+(visibleSize.width/2), origin.y+this->player->getBoundingBox().size.height/2);
	this->addChild(this->player);
}

bool Game::onContactBegin(cocos2d::PhysicsContact &contact) {
	PhysicsBody* A = contact.getShapeA()->getBody();
	PhysicsBody* B = contact.getShapeB()->getBody();
	
	if (A->getCategoryBitmask() == 0x2 && B->getCategoryBitmask() == 0x1) {//bullet hits enemy
		auto bullet = A->getNode();
		auto enemy = (Enemy*)B->getNode();
		this->despawnEnemy(enemy);
		bullet->setVisible(false);
		bullet->removeFromParent();
		this->addScore(100);
	}
	else if (A->getCategoryBitmask() == 0x1 && B->getCategoryBitmask() == 0x2) {//enemy hits bullet ?!
		auto enemy = (Enemy*)A->getNode();
		auto bullet = B->getNode();
		this->despawnEnemy(enemy);
		bullet->setVisible(false);
		bullet->removeFromParent();
		this->addScore(100);
	}
	else if (A->getCategoryBitmask() == 0x1 && B->getCategoryBitmask() == 0x3) {//enemy hits limit
		auto enemy = (Enemy*)A->getNode();
		this->endGame();
	}
	return true;
}

cocos2d::Label* Game::initScoreBoard(){
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	auto scoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
	auto x = origin.x + visibleSize.width ;
	auto y = origin.y + visibleSize.height - scoreLabel->getBoundingBox().size.height / 2;
	scoreLabel->setPosition(x, y);
	scoreLabel->setAnchorPoint(Vec2(1, 0.5));
	this->addChild(scoreLabel);
	return scoreLabel;
}

void Game::initBackground() {
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();
	auto background = (Background*)Background::createSprite();
	auto background2 = (Background*)Background::createSprite();
	auto pos = Vec2(origin.x, origin.y);
	auto pos2 = Vec2(origin.x, origin.y + 
		background2->getBoundingBox().size.height);
	background->setPosition(pos);
	background2->setPosition(pos2);
	this->addChild(background);
	this->addChild(background2);
}

void Game::endGame() {
	auto director = Director::getInstance();
	auto gameOverScene = GameOver::createScene(this->score);
	director->replaceScene(gameOverScene);
}

void Game::initializeGrid() {
	//5x10 grid
	const int col = 10;
	const int row = 5;

	auto enemy = (Enemy*)enemyPool.at(0);
	auto enemySize = enemy->getBoundingBox().size;

	this->grid.resize(row);
	auto director = Director::getInstance();
	auto visibleSize = director->getVisibleSize();
	Vec2 origin = director->getVisibleOrigin();

	auto enemySpawnMaxX = origin.x + visibleSize.width - enemySize.width;
	auto enemySpawnMinX = origin.x + enemySize.width;

	auto enemySpawnMaxY = origin.y + visibleSize.height - enemySize.height;
	auto enemySpawnMinY = (origin.y + visibleSize.height) * 0.4f; //20% of screen top

	auto spawnAreaWidth = abs(enemySpawnMaxX - enemySpawnMinX);
	auto spawnAreaHeight = abs(enemySpawnMaxY - enemySpawnMinY);

	auto cellSize = Vec2(spawnAreaWidth / col, enemySize.height*2);

	for (int r = 0; r < row; r++)
	{
		for (int c = 0; c < col; c++)
		{
			auto pos = Vec2(enemySpawnMinX + (cellSize.x*(c)+(cellSize.x/2)), enemySpawnMaxY - (cellSize.y*(r)+(cellSize.y/2)));
			this->grid.at(r).push_back(pos);
		}
	}

	log("grid initialized");
}