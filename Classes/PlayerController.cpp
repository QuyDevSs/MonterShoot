#include "PlayerController.h"

USING_NS_CC;


PlayerController::PlayerController(const EntityData& data, string fileName) : EntityController(data, fileName)
{
	this->setAnimationEntity("walk_", 8, 0.2f, true);
	this->setBitmask(0x0010, 0x0010 | 0x1000 | 0x10000, 0x0001); // CATEGORY_ENEMY | CATEGORY_BULLET_ENEMY
	this->setTag(Tag::Entity);

	setWeapon(WeaponType::USB);
	setBullet(BulletType::Fire);

	crosshair = Sprite::create("crosshair.png");
	crosshair->setColor(Color3B::BLACK);
	this->addChild(crosshair, 2);

	showHealthBar();

	keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerController::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerController::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(PlayerController::updateCrosshair, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

PlayerController* PlayerController::create(const EntityData& data, string fileName)
{
	PlayerController* playerController = new PlayerController(data, fileName);
	if (playerController && playerController->init()) {
		playerController->autorelease();
		return playerController;
	}
	else {
		delete playerController;
		return nullptr;
	}
}

void PlayerController::movePlayer(float dt)
{
	switch (state)
	{
	case MovingUp:
		this->MoveInDir(Vec2(0, 1), dt);
		break;
	case MovingDown:
		this->MoveInDir(Vec2(0, -1), dt);
		break;
	case MovingLeft:
		this->MoveInDir(Vec2(-1, 0), dt);
		break;
	case MovingRight:
		this->MoveInDir(Vec2(1, 0), dt);
		break;
	case MovingUpRight:
		this->MoveInDir(Vec2(1, 1), dt);
		break;
	case MovingUpLeft:
		this->MoveInDir(Vec2(-1, 1), dt);
		break;
	case MovingDownRight:
		this->MoveInDir(Vec2(1, -1), dt);
		break;
	case MovingDownLeft:
		this->MoveInDir(Vec2(-1, -1), dt);
		break;
	case IDLE:
		/*isMovingUp = false;
		isMovingLeft = false;
		isMovingRight = false;
		isMovingDown = false;*/
		break;
	default:
		break;
	}
}

void PlayerController::updateState()
{
	if (isMovingUp && isMovingLeft)
	{
		state = StatePlayer::MovingUpLeft;
	}
	else if (isMovingUp && isMovingRight)
	{
		state = StatePlayer::MovingUpRight;
	}
	else if (isMovingDown && isMovingLeft)
	{
		state = StatePlayer::MovingDownLeft;
	}
	else if (isMovingDown && isMovingRight)
	{
		state = StatePlayer::MovingDownRight;
	}
	else if (isMovingUp)
	{
		state = StatePlayer::MovingUp;
	}
	else if (isMovingDown)
	{
		state = StatePlayer::MovingDown;
	}
	else if (isMovingLeft)
	{
		state = StatePlayer::MovingLeft;
	}
	else if (isMovingRight)
	{
		state = StatePlayer::MovingRight;
	}
	else
	{
		state = StatePlayer::IDLE;
	}
}

void PlayerController::update(float dt)
{
	if (isDie())
	{
		onDie();
	}
	else
	{
		target = crosshair->getPosition() + this->getPosition();
		updateDirection();
		//shootFunc(dt);
		shoot(dt);
		updateState();
		movePlayer(dt);
	}
}

void PlayerController::onDie()
{
	auto layer = this->getParent();

	const auto& children = layer->getChildren();
	for (const auto& child : children) {
		child->unscheduleUpdate();
		//child->stopAllActions();
	}
	layer->unscheduleUpdate();
	this->setWeapon(WeaponType::EMPTY);
	this->setAnimationEntity("death_", 10, 0.2f, false);
}

void PlayerController::onKeyPressed(EventKeyboard::KeyCode key, Event* event)
{
	switch (key)
	{
	case EventKeyboard::KeyCode::KEY_A:
		isMovingLeft = true;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		isMovingRight = true;
		break;
	case EventKeyboard::KeyCode::KEY_W:
		isMovingUp = true;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		isMovingDown = true;
		break;
	// Đổi súng
	case EventKeyboard::KeyCode::KEY_1:
		setBullet(BulletType::Fire);
		break;
	case EventKeyboard::KeyCode::KEY_2:
		setBullet(BulletType::Ice);
		break;
	case EventKeyboard::KeyCode::KEY_3:
		setBullet(BulletType::Wind);
		break;
	case EventKeyboard::KeyCode::KEY_4:
		setBullet(BulletType::Light);
		break;
	case EventKeyboard::KeyCode::KEY_5:
		setBullet(BulletType::Dark);
		break;
	case EventKeyboard::KeyCode::KEY_TAB:
		nextWeapon();
		break;
	default:
		break;
	}
}

void PlayerController::onKeyReleased(EventKeyboard::KeyCode key, Event* event)
{
	switch (key)
	{
	case EventKeyboard::KeyCode::KEY_A:
		isMovingLeft = false;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		isMovingRight = false;
		break;
	case EventKeyboard::KeyCode::KEY_W:
		isMovingUp = false;
		break;
	case EventKeyboard::KeyCode::KEY_S:
		isMovingDown = false;
		break;
	default:
		break;
	}
}

void PlayerController::shoot(float dt)
{
	switch (weaponCurrent)
	{
	case USB:

		firingUSB(dt, true);
		break;
	case AWM:
		firingAMW(dt, true);
		break;
	case SHOTGUN:
		firingShotGun(dt, true);
		break;
	case EMPTY:
		break;
	default:
		break;
	}
}

void PlayerController::updateCrosshair(Event *event)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto ratio = this->getScale();

	auto mouseEvent = dynamic_cast<EventMouse*>(event);
	auto cursorPos = mouseEvent->getLocation();
	cursorPos = Director::getInstance()->convertToGL(cursorPos);
	
	auto centerPos = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

	auto adjustedPos = (cursorPos - centerPos) * (1/ratio);
	crosshair->setPosition(adjustedPos);

	/*auto weaponPosToGL = weaponPosition + centerPos;

	float angle = atan2(cursorPos.y - weaponPosToGL.y, cursorPos.x - weaponPosToGL.x);
	float angleDegrees = CC_RADIANS_TO_DEGREES(angle);
	if (angleDegrees >= -90 && angleDegrees <= 90)
	{
		entitySprite->setScaleX(1);
		weaponSprite->setScaleX(1);
		weaponSprite->setRotation(-angleDegrees);
	}
	else
	{
		entitySprite->setScaleX(-1);
		weaponSprite->setScaleX(-1);
		weaponSprite->setRotation(-angleDegrees + 180);
	}*/
}

void PlayerController::nextWeapon()
{
	switch (weaponCurrent)
	{
	case USB:
		setWeapon(WeaponType::AWM);
		break;
	case AWM:
		setWeapon(WeaponType::SHOTGUN);
		break;
	case SHOTGUN:
		setWeapon(WeaponType::USB);
		break;
	default:
		break;
	}
}

void PlayerController::takeDamage(float dmg)
{
	auto dmgTaken = dmg - entityData.armor;
	entityData.currentHP -= dmgTaken;
	entitySprite->setColor(Color3B::RED);
	scheduleOnce([=](float dt)
	{
		entitySprite->setColor(Color3B::WHITE);
	}, 0.2f, "resumeColor");

	updateHealthBar();
	
	auto dmgText = to_string(-(int)dmgTaken);
	Label* dmgLable = Label::createWithTTF(dmgText, "fonts/Marker Felt.ttf", 24);
	dmgLable->setPosition(Vec2(entitySize.width / 2, entitySize.height / 2 + 20));
	dmgLable->setColor(Color3B::RED);
	this->addChild(dmgLable, 2);

	DelayTime *pause = DelayTime::create(2.0f);
	auto hideDamage = CallFunc::create([=]()
	{
		dmgLable->removeFromParent();
	});
	auto move = MoveBy::create(2.0f, Vec2(0, 100));
	auto sequence = Sequence::create(move, hideDamage, nullptr);
	dmgLable->runAction(sequence);
}

void PlayerController::setScore(int _score)
{
	score = _score;
}

int PlayerController::getScore()
{
	return score;

}

//void  PlayerController::playerFiringUSB(float dt)
//{
//	auto asdpIncreased = 2.0f;
//	if (interval >= 1 / (this->entityData.asdp * (1 + asdpIncreased)))
//	{
//		firingUSB();
//		interval = 0.0f;
//	}
//	else
//	{
//		interval += dt;
//	}
//}
//void  PlayerController::playerFiringAMW(float dt)
//{
//	auto asdpIncreased = -0.0f;
//	if (interval >= 1 / (this->entityData.asdp * (1 + asdpIncreased)))
//	{
//		firingAMW();
//		interval = 0.0f;
//	}
//	else
//	{
//		interval += dt;
//	}
//}
//
//void  PlayerController::playerFiringShotGun(float dt)
//{
//	auto asdpIncreased = -0.0f;
//	if (interval >= 1 / (this->entityData.asdp * (1 + asdpIncreased)))
//	{
//		firingShotGun();
//		interval = 0.0f;
//	}
//	else
//	{
//		interval += dt;
//	}
//}
//
//void PlayerController::shoot(float dt)
//{
//	switch (weaponCurrent)
//	{
//	case USB:
//		playerFiringUSB(dt);
//		break;
//	case AWM:
//		playerFiringAMW(dt);
//		break;
//	case SHOTGUN:
//		playerFiringShotGun(dt);
//		break;
//	case EMPTY:
//		break;
//	default:
//		break;
//	}
//}

//void PlayerController::UpdateTarget()
//{
//	auto enemy = this->getParent()->getChildByTag(2);
//	if (enemy)
//	{
//		target = enemy;
//	}
//	else
//	{
//		target = nullptr;
//	}
//}
//
//int PlayerController::getexpPlayer()
//{
//	return expPlayer;
//}
//int PlayerController::ExtraExp(int _extraexp)
//{
//	int count = 0;
//	auto _exp = expPlayer + _extraexp;
//	while (_exp >= getexpMax())
//	{
//		_exp -= getexpMax();
//		Lever++;
//		count++;
//	}
//
//	expPlayer = _exp;
//	return count;
//}
//
//int PlayerController::getexpMax()
//{
//	return expMax + Lever * 20;
//}

