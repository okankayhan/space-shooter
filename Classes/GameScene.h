#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class Game : public cocos2d::Scene
{
public:
	cocos2d::Sprite* player;
	static cocos2d::Scene* createScene();
	virtual bool init() override;
	CREATE_FUNC(Game);
	void update(float) override;
	void despawnEnemy(cocos2d::Sprite* enemy);
	void endGame();
	float limitY;
private:
	void spawnEnemy(int amount);
	void spawnPlayer();
	void initLimit();
	cocos2d::Vector<cocos2d::Sprite*> enemyPool;
	cocos2d::Vector<cocos2d::Sprite*> spawnedEnemies;
	std::vector<std::vector<cocos2d::Vec2>> grid;
	cocos2d::Vector<cocos2d::Sprite*> hearts;
	int score;
	float enemySpawnInterval=2000;
	float enemySpawnTimer;
	cocos2d::Label * scoreLabel;
	cocos2d::Label * initScoreBoard();
	void initBackground();
	void addScore(int scoreToAdd);
	void initializeEnemyPool(int);
	void initializeGrid();
	bool onContactBegin(cocos2d::PhysicsContact & contact);
};

#endif // __GAME_SCENE_H__
