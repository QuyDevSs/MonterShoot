#include "PlayScene.h"

USING_NS_CC;

Scene* PlayScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vect(0.0f, 0.0f));
	// 'layer' is an autorelease object
	auto layer = PlayScene::create();

	auto followNode = Follow::create(layer->player);
	layer->runAction(followNode);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
	//return MyScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PlayScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	auto ratio = frameSize.width / visibleSize.width >= frameSize.height / visibleSize.height ? frameSize.width / visibleSize.width : frameSize.height / visibleSize.height;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player1-0.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Enemies1.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy1-0.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy2-0.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy3-0.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy4-0.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GenericDeath.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("explosion.plist");

	this->scheduleUpdate();

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(PlayScene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	EntityData playerData(10.0f, 2.0f, 4.0f, 1300.0f, 200.0f, 200.0f, 500.0f);

	player = PlayerController::create(playerData, "walk_0.png");
	player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(player, 3);
	
	NodeUI = Node::create();

	timeLabel = Label::createWithTTF("00:00", "fonts/Marker Felt.ttf", 24);
	timeLabel->setPosition(Vec2(0, visibleSize.height* 0.45f));
	timeLabel->setColor(Color3B::BLACK);
	NodeUI->addChild(timeLabel);

	scoreLabel = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
	scoreLabel->setPosition(Vec2(- visibleSize.width * 0.45f, visibleSize.height* 0.45f));
	scoreLabel->setColor(Color3B::BLACK);
	NodeUI->addChild(scoreLabel);

	pauseButton = ui::Button::create("pauseButton.png", "pauseButton.png");
	pauseButton->setPosition(Vec2(visibleSize.width * 0.45f, visibleSize.height* 0.45f));
	pauseButton->addClickEventListener(CC_CALLBACK_1(PlayScene::pausedWindow, this));
	NodeUI->addChild(pauseButton);

	this->addChild(NodeUI, 4);

	AudioManager::getInstance()->playBackgroundMusic("BackgroundMusic.mp3");

	currentTilemap = TMXTiledMap::create("map0.tmx");
	currentTilemap->setPosition(player->getPosition());
	this->addChild(currentTilemap, -1);
	currentTilemap->setAnchorPoint(Vec2(0.5f, 0.5f));
	currentTilemap->setScale(ratio*2.0f);

	return true;
}

bool PlayScene::onContactBegin(PhysicsContact& contact)
{
	auto tag1 = 0;
	auto tag2 = 0;

	auto obj1 = contact.getShapeA()->getBody()->getNode();
	if (obj1 != nullptr)
	{
		tag1 = obj1->getTag();
	}
	auto obj2 = contact.getShapeB()->getBody()->getNode();
	if (obj2 != nullptr)
	{
		tag2 = obj2->getTag();
	}
	//Va chạm xảy ra giữa đạn và thực thể
	if ((tag1 == Tag::Entity && tag2 == Tag::Bullet) || (tag1 == Tag::Bullet && tag2 == Tag::Entity))
	{
		BulletController* bullet = (tag1 == Tag::Bullet) ? (BulletController *)obj1 : (BulletController *)obj2;
		EntityController* entity = (tag1 == Tag::Entity) ? (EntityController *)obj1 : (EntityController *)obj2;
		
		if (bullet)
		{
			auto damage = bullet->getDmg();
			BulletType bulletType = bullet->getType();
			entity->setEffect(bulletType, 3.0f);

			if (bulletType == BulletType::Ice)
			{
				entity->slow(0.5f, 3.0f);
			}

			if (bulletType != BulletType::Heal)
			{
				entity->takeDamage(damage);
			}
			else
			{
				entity->healing(damage);
			}

			if (bulletType != BulletType::Wind && bulletType != BulletType::Boom)
			{
				bullet->destroy();
			}
		}
		else
		{
			CCLOG("error: %p", entity);
		}
	}

	//Va chạm xảy ra giữa thực thể và thực thể
	if (tag1 == Tag::Entity && tag2 == Tag::Entity)
	{
		EntityController* entity1 = (EntityController*)obj1;
		EntityController* entity2 = (EntityController*)obj2;

		auto damage1 = entity1->getDmg();
		auto damage2 = entity2->getDmg();

		entity1->takeDamage(damage2);
		entity2->takeDamage(damage1);
	}

	//Va chạm xảy ra giữa đạn và đạn
	if (tag1 == Tag::Bullet && tag2 == Tag::Bullet)
	{
		BulletController* bullet1 = (BulletController*)obj1;
		BulletController* bullet2 = (BulletController *)obj2;

		if (bullet1->getType() == BulletType::EnemyBullet)
		{
			bullet1->destroy();
		}
		else
		{
			bullet2->destroy();
		}
	}

	return true; // Phải trả lại giá trị true
}

void PlayScene::createEnemy(EntityData ennemyData, EnemyType enemyType)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto ratePosx = random<float>(-1.0f, 1.0f);
	float ratePosy;
	if (ratePosx > 0.9f || ratePosx < -0.9f)
	{
		ratePosy = random<float>(-1.0f, 1.0f);
	}
	else
	{
		auto ratePosy1 = random<float>(0.9f, 1.0f);
		auto ratePosy2 = random<float>(-1.0f, -0.9f);
		ratePosy = (random(1, 2) % 2 == 0) ? ratePosy1 : ratePosy2;
	}
	auto xcenter = visibleSize.width / 2;
	auto ycenter = visibleSize.height / 2;
	
	auto pos = Vec2(ratePosx * xcenter, ratePosy * ycenter);
	auto enemy = EnemyController::create(ennemyData, enemyType);
	enemy->setPosition(player->getPosition() + pos);
	enemy->entityTarget = player;
	this->addChild(enemy, 2);
}

void PlayScene::createEnemies(EntityData ennemyData, EnemyType enemyType, int quantity)
{
	for (int i = 0; i < quantity; i++)
	{
		createEnemy(ennemyData, enemyType);
	}
}

void PlayScene::update(float dt)
{
	timer += dt;
	minutes = static_cast<int>(timer) / 60;
	seconds = static_cast<int>(timer) % 60;
	updateTimeLabel();
	updateScoreLabel();

	NodeUI->setPosition(player->getPosition());

	if (player->isDie())
	{
		AudioManager::getInstance()->stopBackgroundMusic();
		AudioManager::getInstance()->playSoundEffect("PlayerDie.mp3");

		scheduleOnce([=](float dt)
		{
			Ref* sender = nullptr;
			failedWindow(sender);
		}, 1.8f, "failedWindow");
		
	}

	if (isWinner())
	{
		this->unscheduleUpdate();
		Ref* sender = nullptr;
		clearWindow(sender);
	}
	
	EntityData ennemyData(10.0f, 1.0f, 0.1f, 600.0f, 50.0f, 50.0f, 200.0f);
	if (CDEnemyTanker >= 15.0f && timer <= timerEnd)
	{
		createEnemies(ennemyData, EnemyType::EnemyTanker, num);
		totalEnemies += num;
		CDEnemyTanker = 0;
	}
	else if (CDEnemyFaster >= 15.0f && timer <= timerEnd)
	{
		createEnemies(ennemyData, EnemyType::EnemyFaster, num);
		totalEnemies += num;
		CDEnemyFaster = 0;
	}
	else if (CDEnemyArcher >= 15.0f && timer <= timerEnd)
	{
		createEnemies(ennemyData, EnemyType::EnemyArcher, num);
		totalEnemies += num;
		CDEnemyArcher = 0;
		num += 3;
	}
	else if (CDEnemyBoss >= 120.0f && timer <= timerEnd)
	{
		createEnemies(ennemyData, EnemyType::EnemyBoss, 1);
		CDEnemyBoss = 0;
		totalEnemies += 1;
	}
	else
	{
		CDEnemyTanker += dt;
		CDEnemyFaster += dt;
		CDEnemyArcher += dt;
		CDEnemyBoss += dt;
	}

	auto mapSize = currentTilemap->getContentSize() * currentTilemap->getScale();
	auto playerPosx = player->getPosition().x;
	auto playerPosy = player->getPosition().y;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if (playerPosx >= currentTilemap->getPositionX() + mapSize.width/4)
	{
		auto posx = currentTilemap->getPositionX();
		currentTilemap->setPositionX(posx + mapSize.width/2);
	}
	else if (playerPosx <= currentTilemap->getPositionX() - mapSize.width / 4)
	{
		auto posx = currentTilemap->getPositionX();
		currentTilemap->setPositionX(posx - mapSize.width / 2);
	}
	
	if (playerPosy >= currentTilemap->getPositionY() + mapSize.height/4)
	{
		auto posy = currentTilemap->getPositionY();
		currentTilemap->setPositionY(posy + mapSize.height / 2);
	}
	else if (playerPosy <= currentTilemap->getPositionY() - mapSize.height / 4)
	{
		auto posy = currentTilemap->getPositionY();
		currentTilemap->setPositionY(posy - mapSize.height / 2);
	}
}

void PlayScene::updateTimeLabel()
{
	// Định dạng chuỗi hiển thị thời gian theo "phút : giây"
	std::string timeString = std::to_string(minutes) + ":";

	// Nếu giây nhỏ hơn 10, thêm số 0 vào trước
	if (seconds < 10)
	{
		timeString += "0" + std::to_string(seconds);
	}
	else 
	{
		timeString += std::to_string(seconds);
	}

	// Cập nhật hiển thị trên Label
	timeLabel->setString(timeString);
}

void PlayScene::updateScoreLabel()
{
	std::string scoreString = "Score: " + std::to_string(player->getScore());

	scoreLabel->setString(scoreString);
}

//bool isPaused = false;
//void PlayScene::pauseButton(Ref* sender)
//{
//	AudioEngine::play2d("Click.mp3", false, 1.0f);
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//	if (isPaused)
//	{
//		Director::getInstance()->resume();
//		Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1.0f);
//		player->mouseListener->setEnabled(true);
//		_eventDispatcher->resumeEventListenersForTarget(this, true);
//		isPaused = false;
//	}
//	else
//	{
//		_eventDispatcher->pauseEventListenersForTarget(this);
//		player->mouseListener->setEnabled(false);
//		Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0.0f);
//		Director::getInstance()->pause();
//		isPaused = true;
//	}
//}

void PlayScene::pausedWindow(Ref* sender)
{
	pauseGame(sender);

	if (pausedSprite)
	{
		pausedSprite->setVisible(true);
	}
	else
	{
		pausedSprite = Sprite::create("pausedWindow.png");
		pausedSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		auto dialogWidth = pausedSprite->getContentSize().width;
		auto dialogHeight = pausedSprite->getContentSize().height;

		auto resumeButton = ui::Button::create("resumeButton.png", "resumeButtonSelected.png");
		resumeButton->setPosition(Vec2(dialogWidth * 5 / 16, dialogHeight * 11 / 16));
		resumeButton->addClickEventListener(CC_CALLBACK_1(PlayScene::resumeGame, this));

		auto replayButton = ui::Button::create("retryButton.png", "retryButtonSelected.png");
		replayButton->setPosition(Vec2(dialogWidth * 11 / 16, dialogHeight * 11 / 16));
		replayButton->addClickEventListener(CC_CALLBACK_1(PlayScene::replay, this));

		auto homeButton = ui::Button::create("MenuButton.png", "MenuButtonSelected.png");
		homeButton->setPosition(Vec2(dialogWidth * 11 / 16, dialogHeight * 6 / 16));
		homeButton->addClickEventListener(CC_CALLBACK_1(PlayScene::homePage, this));

		auto optionButton = ui::Button::create("optionButton.png", "optionButtonSelected.png");
		optionButton->setPosition(Vec2(dialogWidth * 5 / 16, dialogHeight * 6 / 16));
		optionButton->addClickEventListener(CC_CALLBACK_1(PlayScene::optionsWindow, this));

		auto closeButton = ui::Button::create("closeButton.png", "closeButtonSelected.png");
		closeButton->setPosition(Vec2(dialogWidth * 14.5 / 16, dialogHeight * 14.5 / 16));
		closeButton->addClickEventListener(CC_CALLBACK_1(PlayScene::resumeGame, this));
		
		pausedSprite->addChild(resumeButton);
		pausedSprite->addChild(replayButton);
		pausedSprite->addChild(homeButton);
		pausedSprite->addChild(optionButton);
		pausedSprite->addChild(closeButton);

		NodeUI->addChild(pausedSprite);
	}
}

//void PlayScene::pauseButton(Ref* sender)
//{
//	AudioEngine::play2d("Click.mp3", false, 1.0f);
//	auto visibleSize = Director::getInstance()->getVisibleSize();
//	//Vec2 origin = Director::getInstance()->getVisibleOrigin();
//
//	_eventDispatcher->pauseEventListenersForTarget(this);
//	player->mouseListener->setEnabled(false);
//	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0.0f);
//	Director::getInstance()->pause();
//
//	auto dialogBox = Sprite::create("clearWindowTest.png");
//	dialogBox->setTag(Tag::dialogBox);
//	dialogBox->setAnchorPoint(Vec2(0.5f, 0.5f));
//	auto dialogWidth = dialogBox->getContentSize().width;
//	auto dialogHeight = dialogBox->getContentSize().height;
//
//	auto scoreLabel2 = Label::createWithTTF("9999", "fonts/arial.ttf", 22);
//	scoreLabel2->setPosition(Vec2(dialogWidth * 8 / 16, dialogHeight * 8.85 / 16));
//	scoreLabel2->enableBold();
//	dialogBox->addChild(scoreLabel2);
//	NodeUI->addChild(dialogBox);
//}

void PlayScene::pauseGame(Ref* sender)
{
	AudioManager::getInstance()->playSoundEffect("Click.mp3");
	AudioManager::getInstance()->stopBackgroundMusic();

	_eventDispatcher->pauseEventListenersForTarget(this);
	player->mouseListener->setEnabled(false);
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0.0f);
	Director::getInstance()->pause();

	pauseButton->setEnabled(false);
}

void PlayScene::resumeGame(Ref* sender)
{
	AudioManager::getInstance()->playSoundEffect("Click.mp3");
	AudioManager::getInstance()->playBackgroundMusic("BackgroundMusic.mp3");
	if (optionsSprite)
	{
		optionsSprite->setVisible(false);
	}
	if (pausedSprite)
	{
		pausedSprite->setVisible(false);
	}
	if (clearSprite)
	{
		clearSprite->setVisible(false);
	}

	Director::getInstance()->resume();
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1.0f);
	player->mouseListener->setEnabled(true);
	_eventDispatcher->resumeEventListenersForTarget(this, true);
	pauseButton->setEnabled(true);
}

void PlayScene::homePage(Ref* sender)
{
	resumeGame(sender);
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}

void PlayScene::replay(Ref* sender)
{
	resumeGame(sender);
	auto scene = PlayScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void PlayScene::nextLever(Ref* sender)
{
	resumeGame(sender);
	auto scene = MainMenu::createScene();
	Director::getInstance()->replaceScene(scene);
}

void PlayScene::optionsWindow(Ref* sender)
{
	AudioManager::getInstance()->playSoundEffect("Click.mp3");
	if (pausedSprite)
	{
		pausedSprite->setVisible(false);
	}
	if (optionsSprite)
	{
		optionsSprite->setVisible(true);
	}
	else
	{
		optionsSprite = Sprite::create("optionsWindow.png");
		optionsSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		auto dialogWidth = optionsSprite->getContentSize().width;
		auto dialogHeight = optionsSprite->getContentSize().height;

		auto musicBar = ProcessBar::create("barMusic.png", "bgMusic.png");
		musicBar->setPosition(Vec2(dialogWidth * 5 / 16, dialogHeight * 10 / 16 - musicBar->getContentSize().height/2));
		auto soundBar = ProcessBar::create("barMusic.png", "bgMusic.png");
		soundBar->setPosition(Vec2(dialogWidth * 5 / 16, dialogHeight * 5 / 16 - soundBar->getContentSize().height / 2));

		auto incMusicButton = ui::Button::create("increaseButton.png", "increaseButton.png");
		incMusicButton->setPosition(Vec2(dialogWidth * 11 / 16, dialogHeight * 10 / 16));
		incMusicButton->addClickEventListener([=](cocos2d::Ref* sender) {
			auto musicVolume = AudioManager::getInstance()->getMusicVolume();
			AudioManager::getInstance()->setMusicVolume(musicVolume + 0.1f);
			musicBar->setPercentBarSprite(musicBar->getPercentBarSprite() + 10.0f);
		});

		auto decMusicButton = ui::Button::create("decreaseButton.png", "decreaseButton.png");
		decMusicButton->setPosition(Vec2(dialogWidth * 5 / 16, dialogHeight * 10 / 16));
		decMusicButton->addClickEventListener([=](cocos2d::Ref* sender) {
			auto musicVolume = AudioManager::getInstance()->getMusicVolume();
			AudioManager::getInstance()->setMusicVolume(musicVolume - 0.1f);
			musicBar->setPercentBarSprite(musicBar->getPercentBarSprite() - 10.0f);
		});

		auto incSoundButton = ui::Button::create("increaseButton.png", "increaseButton.png");
		incSoundButton->setPosition(Vec2(dialogWidth * 11 / 16, dialogHeight * 5 / 16));
		incSoundButton->addClickEventListener([=](cocos2d::Ref* sender) {
			auto soundVolume = AudioManager::getInstance()->getSoundVolume();
			AudioManager::getInstance()->setSoundVolume(soundVolume + 0.1f);
			soundBar->setPercentBarSprite(soundBar->getPercentBarSprite() + 10.0f);
		});

		auto decSoundButton = ui::Button::create("decreaseButton.png", "decreaseButton.png");
		decSoundButton->setPosition(Vec2(dialogWidth * 5 / 16, dialogHeight * 5 / 16));
		decSoundButton->addClickEventListener([=](cocos2d::Ref* sender) {
			auto soundVolume = AudioManager::getInstance()->getSoundVolume();
			AudioManager::getInstance()->setSoundVolume(soundVolume - 0.1f);
			soundBar->setPercentBarSprite(soundBar->getPercentBarSprite() - 10.0f);
		});

		auto closeButton = ui::Button::create("closeButton.png", "closeButtonSelected.png");
		closeButton->setPosition(Vec2(dialogWidth * 14.5 / 16, dialogHeight * 14.5 / 16));
		closeButton->addClickEventListener(CC_CALLBACK_1(PlayScene::resumeGame, this));

		optionsSprite->addChild(incMusicButton, 1);
		optionsSprite->addChild(decMusicButton, 1);
		optionsSprite->addChild(incSoundButton, 1);
		optionsSprite->addChild(decSoundButton, 1);
		optionsSprite->addChild(musicBar);
		optionsSprite->addChild(soundBar);
		optionsSprite->addChild(closeButton);

		NodeUI->addChild(optionsSprite, 1);
	}
}

void PlayScene::clearWindow(Ref* sender)
{
	pauseGame(sender);
	AudioManager::getInstance()->playSoundEffect("winner.mp3");
	CCLOG("dasd");
	if (clearSprite)
	{
		clearSprite->setVisible(true);
	}
	else
	{
		clearSprite = Sprite::create("clearWindow.png");
		clearSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		auto dialogWidth = clearSprite->getContentSize().width;
		auto dialogHeight = clearSprite->getContentSize().height;

		std::string scoreString = std::to_string(player->getScore());
		auto scoreLabel2 = Label::createWithTTF(scoreString, "fonts/arial.ttf", 22);
		scoreLabel2->setPosition(Vec2(dialogWidth * 8 / 16, dialogHeight * 8.85 / 16));
		scoreLabel2->enableBold();

		auto replayButton = ui::Button::create("retryButton.png", "retryButtonSelected.png");
		replayButton->setPosition(Vec2(dialogWidth * 4 / 16, dialogHeight * 6 / 16));
		replayButton->addClickEventListener(CC_CALLBACK_1(PlayScene::replay, this));

		auto nextButton = ui::Button::create("nextButton.png", "nextButtonSelected.png");
		nextButton->setPosition(Vec2(dialogWidth * 8 / 16, dialogHeight * 6 / 16));
		nextButton->addClickEventListener(CC_CALLBACK_1(PlayScene::nextLever, this));

		auto homeButton = ui::Button::create("MenuButton.png", "MenuButtonSelected.png");
		homeButton->setPosition(Vec2(dialogWidth * 12 / 16, dialogHeight * 6 / 16));
		homeButton->addClickEventListener(CC_CALLBACK_1(PlayScene::homePage, this));

		clearSprite->addChild(scoreLabel2);
		clearSprite->addChild(replayButton);
		clearSprite->addChild(nextButton);
		clearSprite->addChild(homeButton);

		NodeUI->addChild(clearSprite);
	}
}

void PlayScene::failedWindow(Ref* sender)
{
	pauseButton->setEnabled(false);
	if (failedSprite)
	{
		failedSprite->setVisible(true);
	}
	else
	{
		failedSprite = Sprite::create("failedWindow.png");
		failedSprite->setAnchorPoint(Vec2(0.5f, 0.5f));
		auto dialogWidth = failedSprite->getContentSize().width;
		auto dialogHeight = failedSprite->getContentSize().height;

		std::string scoreString = std::to_string(player->getScore());
		auto scoreLabel2 = Label::createWithTTF(scoreString, "fonts/arial.ttf", 22);
		scoreLabel2->setPosition(Vec2(dialogWidth * 8 / 16, dialogHeight * 8.85 / 16));
		scoreLabel2->enableBold();

		auto replayButton = ui::Button::create("retryButton.png", "retryButtonSelected.png");
		replayButton->setPosition(Vec2(dialogWidth * 5 / 16, dialogHeight * 6 / 16));
		replayButton->addClickEventListener(CC_CALLBACK_1(PlayScene::replay, this));

		auto homeButton = ui::Button::create("MenuButton.png", "MenuButtonSelected.png");
		homeButton->setPosition(Vec2(dialogWidth * 11 / 16, dialogHeight * 6 / 16));
		homeButton->addClickEventListener(CC_CALLBACK_1(PlayScene::homePage, this));

		failedSprite->addChild(scoreLabel2);
		failedSprite->addChild(replayButton);
		failedSprite->addChild(homeButton);

		NodeUI->addChild(failedSprite);
	}
}

bool PlayScene::isWinner()
{
	if (timer > timerEnd && player->getScore() >= totalEnemies)
	{
		return true;
	}

	return false;
}

// thêm âm thanh va chạm với máu
// máu tự mất khi không ăn sau 15s
// thêm âm thanh đổi súng
// hàm isWiner
// clearWindow
// thêm màn chơi
// thêm vật phẩm, tăng tốc độ đánh, dame, armor
