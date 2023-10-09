#include "MainMenu.h"

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
bool MainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto logo = Label::createWithTTF("Monter Shoot", "fonts/Marker Felt.ttf", 148);
	logo->setColor(Color3B::RED);
    if (logo == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
		logo->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height/2 + logo->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(logo, 1);
    }

    // add background splash screen"
    auto background = Sprite::create("ground2_white.png");
    if (background == nullptr)
    {
        problemLoading("'ground2_white.png'");
    }
    else
    {
        // position the sprite on the center of the screen
		background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(background, 0);
    }

	auto player = Sprite::create("Player.png");
	auto enemy = Sprite::create("Enemy.png");
	player->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height * 2/ 3 + origin.y));
	enemy->setPosition(Vec2(visibleSize.width * 3/ 4 + origin.x, visibleSize.height * 2/ 3 + origin.y));
	
	player->setScale(0.3f);

	enemy->setScaleX(-0.3f);
	enemy->setScaleY(0.3f);
	this->addChild(player, 1);
	this->addChild(enemy, 1);

	// Menu Play
	// Tạo một Label với một chuỗi văn bản
	auto playlabel = Label::createWithTTF("Play", "fonts/Marker Felt.ttf", 96);
	playlabel->setColor(Color3B::BLACK);

	// Tạo một MenuItemLabel với Label đã tạo
	auto menuPlaylabel = MenuItemLabel::create(playlabel, [&](Ref* sender) {
		auto scene = PlayScene::createScene();
		Director::getInstance()->replaceScene(scene);
	});
	// Thêm MenuItemLabel vào một Menu
	auto menuPlay = Menu::create(menuPlaylabel, nullptr);
	menuPlay->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(menuPlay, 1);

	// Menu Exit
	auto exitlabel = Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 96);
	exitlabel->setColor(Color3B::BLACK);
	auto menuExitlabel = MenuItemLabel::create(exitlabel, CC_CALLBACK_1(MainMenu::menuCloseCallback, this));
	auto menuExit = Menu::create(menuExitlabel, nullptr);
	menuExit->setPosition(Vec2(origin.x + visibleSize.width / 2,origin.y + visibleSize.height / 2 - exitlabel->getContentSize().height));
	this->addChild(menuExit, 1);

    return true;
}


void MainMenu::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
