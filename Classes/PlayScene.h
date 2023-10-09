#ifndef __PlayScene_H__
#define __PlayScene_H__

#include "cocos2d.h"
#include "MainMenu.h"
#include "PlayerController.h"
#include "EnemyController.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "AudioManager.h"

using namespace cocos2d;
using namespace std;
using namespace ui;
class PlayScene : public Layer
{
private:
	TMXTiledMap* currentTilemap;
	// UI
	Node* NodeUI;
	Label* timeLabel;
	float timer = 0;
	int minutes;
	int seconds;
	Label* scoreLabel;
	void updateTimeLabel();
	void updateScoreLabel();
	//Window
	Sprite* pausedSprite;
	Sprite* optionsSprite;
	Sprite* clearSprite;
	Sprite* failedSprite;

	void pausedWindow(Ref* sender);
	void optionsWindow(Ref* sender);
	void clearWindow(Ref* sender);
	void failedWindow(Ref* sender);

	// Button
	Button* pauseButton;
	void pauseGame(Ref* sender);
	void resumeGame(Ref* sender);
	void homePage(Ref* sender);
	void replay(Ref* sender);
	void nextLever(Ref* sender);
	
	float CDEnemyTanker = 15.0f;
	float CDEnemyFaster = 10.0f;
	float CDEnemyArcher = 5.0f;
	float CDEnemyBoss = 0.0f;
	int num = 5;
	int totalEnemies = 0;
	float timerEnd = 363.0f;
public:
	EntityData playerData;
	EntityData enemyData;
	static Scene* createScene();
	virtual bool init();
	/*bool initWithGameData(EntityData _playerData, EntityData _enemyData);
	static PlayScene* createWithGameData(EntityData _playerData, EntityData _enemyData);
	void createPlayer(EntityData playerData);*/
	PlayerController* player;
	//void update(float dt);
	//void menuCloseCallback(Ref* pSender);
	void update(float dt);
	bool onContactBegin(PhysicsContact& contact);
	void createEnemy(EntityData ennemyData, EnemyType enemyType);
	void createEnemies(EntityData ennemyData, EnemyType enemyType, int quantity);
	bool isWinner();
	
	CREATE_FUNC(PlayScene);
};

#endif // __PlayScene_H__
