#include "EntityController.h"

USING_NS_CC;

EntityController::EntityController(const EntityData& _data, string fileName) : entityData(_data), weaponSprite(Sprite::create())
{
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto ratio = frameSize.width / visibleSize.width >= frameSize.height / visibleSize.height ? frameSize.width / visibleSize.width : frameSize.height / visibleSize.height;

	setSpeed(_data.speed);
	entitySprite = Sprite::createWithSpriteFrameName(fileName);
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(fileName);

	entitySize = frame->getRectInPixels().size;
	entitySize = Size(entitySize.width * visibleSize.width / frameSize.width,
		entitySize.height * visibleSize.height / frameSize.height);

	auto offset = frame->getOffset();
	entitySprite->setPosition(-offset);

	/*CCLOG("%f, %f", ratiox, ratioy);
	this->setScaleX(ratiox);
	this->setScaleX(ratioy);*/

	weaponPosition = Vec2(0, -0.2f * entitySize.height);
	this->addChild(entitySprite);
	auto body = PhysicsBody::createCircle(entitySize.height / 2);
	this->setPhysicsBody(body);
}

void EntityController::setAnimationEntity(string prefixName, int pFramesOrder, float delay, bool repeat = true)
{
	entitySprite->stopAllActions();
	Vector<SpriteFrame*> animFrames;

	for (int i = 0; i < pFramesOrder; i++)
	{
		char buffer[20] = { 0 };
		sprintf(buffer, "%d.png", i);
		std::string str = prefixName + buffer;
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(animFrames, delay);
	auto animate = Animate::create(animation);

	if (repeat)
	{
		entitySprite->runAction(RepeatForever::create(animate));
	}
	else
	{
		entitySprite->runAction(animate);
	}
}

void EntityController::setBitmask(int collision, int contactTest)
{
	auto physicsBody = this->getPhysicsBody();
	if (physicsBody)
	{
		physicsBody->setCollisionBitmask(collision);
		physicsBody->setContactTestBitmask(contactTest);
	}
	
}

void EntityController::setBitmask(int collision, int contactTest, int category)
{
	auto physicsBody = this->getPhysicsBody();
	if (physicsBody)
	{
		physicsBody->setCategoryBitmask(category);
		physicsBody->setCollisionBitmask(collision);
		physicsBody->setContactTestBitmask(contactTest);
	}
}

void EntityController::setWeaponSprite(const std::string& filename = "")
{
	if (weaponSprite)
	{
		removeChild(weaponSprite, true);
		weaponSprite = nullptr;
	}

	if (filename == "")
	{
		weaponSprite = Sprite::create();
	}
	else
	{
		weaponSprite = Sprite::create(filename);
	}
	
	if (weaponSprite)
	{
		weaponSprite->setPosition(weaponPosition);
		addChild(weaponSprite);
	}
}

void EntityController::setWeapon(WeaponType weapon)
{
	weaponCurrent = weapon;
	switch (weapon)
	{
	case USB:
		this->setWeaponSprite("USB.png");
		break;
	case AWM:
		this->setWeaponSprite("AMW.png");
		break;
	case SHOTGUN:
		this->setWeaponSprite("SHOTGUN.png");
		break;
	case EMPTY:
		this->setWeaponSprite();
		break;
	default:
		break;
	}
}

//void EntityController::setWeapon(Weapon weapon)
//{
//	switch (weapon)
//	{
//	case USB:
//		this->setWeaponSprite("USB.png");
//		shootFunc = std::bind(&EntityController::firingUSB, this, std::placeholders::_1);
//		break;
//	case AWM:
//		this->setWeaponSprite("AMW.png");
//		shootFunc = std::bind(&EntityController::firingAMW, this, std::placeholders::_1);
//		break;
//	case SHOTGUN:
//		this->setWeaponSprite("SHOTGUN.png");
//		shootFunc = std::bind(&EntityController::firingShotGun, this, std::placeholders::_1);
//		break;
//	case USBENEMY:
//		this->setWeaponSprite("USB.png");
//		shootFunc = std::bind(&EntityController::enemyFiringUSB, this, std::placeholders::_1);
//		break;
//	case EMPTY:
//		shootFunc = std::bind(&EntityController::stopFiring, this, std::placeholders::_1);
//		break;
//	default:
//		break;
//	}
//}

//void EntityController::setWeapon(int weaponIndex)
//{
//	setWeapon(allWeapons[weaponIndex]);
//}
//
//void EntityController::nextWeapon()
//{
//	if (weaponIndex < allWeapons.size() - 1)
//	{
//		weaponIndex++;
//	}
//	else
//	{
//		weaponIndex = 0;
//	}
//	setWeapon(weaponIndex);
//}

void EntityController::setBullet(BulletType bulleType)
{
	bulletCurrent = bulleType;
}

//void EntityController::setBullet(BulletType bulleType)
//{
//	bulletCurrent = bulleType;
//	switch (bulleType)
//	{
//	case Fire:
//		bulletCurrent = { Tag::BulletFire, 0, 1, "Bullet1.png", 100.0f , entityData.attack };
//		break;
//	case Ice:
//		bulletCurrent = { Tag::BulletIce, 0, 1, "Bullet3.png", 1000.0f , entityData.attack };
//		break;
//	case Wind:
//		bulletCurrent = { Tag::BulletWind, 0, 1, "Bullet4.png", 1000.0f , entityData.attack };
//		break;
//	case Light:
//		bulletCurrent = { Tag::BulletLight, 0, 1, "Bullet5.png", 1000.0f , entityData.attack };
//		break;
//	case Dark:
//		bulletCurrent = { Tag::BulletDark, 0, 1, "Bullet2.png", 1000.0f , entityData.attack };
//		break;
//		//case Enemy:
//			//bulletCurrent = { Tag::Enemy, 0, 1, "Bullet.png", 1000.0f , entityData.attack };
//			//break;
//	default:
//		break;
//	}
//
//}

//void EntityController::setBodyEntity()
//{
//	//auto targetBody = PhysicsBody::createBox(entitySize);
//	auto targetBody = PhysicsBody::createCircle(entitySize.width);
//	//targetBody->setPositionOffset(Vec2(2, -38));
//
//	//targetBody->setCategoryBitmask(0x01);
//	targetBody->setCollisionBitmask(0x02);
//	targetBody->setContactTestBitmask(0x01);
//	
//	this->setPhysicsBody(targetBody);
//}

//void EntityController::setBodyEntity(const Vec2& offset)
//{
//	auto targetBody = PhysicsBody::createBox(entitySize);
//	targetBody->setPositionOffset(offset);
//	this->setPhysicsBody(targetBody);
//}

//void EntityController::setBodyEntity(const Size& size, const Vec2& offset)
//{
//	auto targetBody = PhysicsBody::createBox(size);
//	targetBody->setPositionOffset(offset);
//	this->setPhysicsBody(targetBody);
//}

void EntityController::firingUSB(float dt, bool audio = false)
{
	auto asdpIncreased = 4.0f;
	if (interval >= 1 / (this->entityData.asdp * (1 + asdpIncreased)))
	{
		if (audio)
		{
			//AudioManager::getInstance()->playSoundEffect("USB.mp3");
		}
		auto speedTemp = entityData.bulletSpeed;
		auto bullet = BulletController::create(bulletCurrent);
		bullet->setSpeed(speedTemp);
		bullet->setDmg(entityData.attack);

		Vec2 dir = (target - (this->getPosition() + weaponPosition)).getNormalized();
		Vec2 startPosition = dir * (weaponSprite->getContentSize().width / 2);
		
		bullet->setPosition(this->getPosition() + weaponPosition + startPosition);
		this->getParent()->addChild(bullet);
		bullet->BulletMove(dir, 900.0f);

		interval = 0.0f;
	}
	else
	{
		interval += dt;
	}
}

void EntityController::firingAMW(float dt, bool audio = false)
{
	auto asdpIncreased = -0.0f;
	if (interval >= 1 / (this->entityData.asdp * (1 + asdpIncreased)))
	{
		if (audio)
		{
			AudioManager::getInstance()->playSoundEffect("AMW.mp3");
		}

		auto speedReduce = 0.5f;
		auto speedTemp = entityData.bulletSpeed * (1 - speedReduce);

		auto bullet = BulletController::create(bulletCurrent);
		bullet->setSpeed(speedTemp);
		bullet->setDmg(entityData.attack);
		Vec2 dir = (target - (this->getPosition() + weaponPosition)).getNormalized();
		Vec2 startPosition = dir * (weaponSprite->getContentSize().width / 2);

		bullet->setPosition(this->getPosition() + weaponPosition + startPosition);
		this->getParent()->addChild(bullet);
		
		float angle = 120.0f; // Góc giữa các đường thẳng
		float angleInRadians = CC_DEGREES_TO_RADIANS(angle);
		for (int i = 0; i < 3; i++)
		{
			auto exactAngle = angleInRadians + CC_DEGREES_TO_RADIANS(angle) * i;
			auto bullet1 = BulletController::create(bulletCurrent);
			bullet1->setSpeed(150.0f);
			bullet1->setDmg(entityData.attack);
			
			Vec2 dirbulletSprite = Vec2(cos(exactAngle), sin(exactAngle)).getNormalized();
			bullet1->BulletMove(dirbulletSprite, 300.0f);
			
			Vec2 dir = (target - (this->getPosition() + weaponPosition)).getNormalized();
			Vec2 startPosition = dir * (weaponSprite->getContentSize().width / 2);
			bullet->addChild(bullet1);
		}
		bullet->BulletMoveRotate(dir, 360.0f, 900.0f);

		interval = 0.0f;
	}
	else
	{
		interval += dt;
	}
}

void EntityController::firingShotGun(float dt, bool audio = false)
{
	auto asdpIncreased = -0.0f;
	if (interval >= 1 / (this->entityData.asdp * (1 + asdpIncreased)))
	{
		if (audio)
		{
			AudioManager::getInstance()->playSoundEffect("Gun.mp3");
		}
		auto speedReduce = 0.0f;
		auto speedTemp = entityData.bulletSpeed * (1 - speedReduce);

		float angle = 20.0f; // Góc giữa các đường thẳng
		float angleInRadians = CC_DEGREES_TO_RADIANS(angle);
		for (int i = 0; i < 5; i++)
		{
			auto bullet = BulletController::create(bulletCurrent);
			bullet->setSpeed(speedTemp);
			bullet->setDmg(entityData.attack);

			Vec2 dir = (target - (this->getPosition() + weaponPosition)).getNormalized();
			Vec2 startPosition = dir * (weaponSprite->getContentSize().width / 2);
			bullet->setPosition(this->getPosition() + weaponPosition + startPosition);
			this->getParent()->addChild(bullet);
			
			auto exactAngle = CC_DEGREES_TO_RADIANS(angle) * (i - 2);

			float cosAngle = cos(exactAngle);
			float sinAngle = sin(exactAngle);

			Vec2 newDir = Vec2(dir.x * cosAngle - dir.y * sinAngle, dir.x * sinAngle + dir.y * cosAngle);
			bullet->BulletMove(newDir, 900.0f);
		}

		interval = 0.0f;
	}
	else
	{
		interval += dt;
	}
}

void EntityController::shoot(float dt)
{
	switch (weaponCurrent)
	{
	case USB:
		firingUSB(dt);
		break;
	case AWM:
		firingAMW(dt);
		break;
	case SHOTGUN:
		firingShotGun(dt);
		break;
	case EMPTY:
		break;
	default:
		break;
	}
}

void EntityController::updateDirection()
{
	auto weaponPosToWorld = weaponPosition + this->getPosition();

	Vec2 targetDirection = target - weaponPosToWorld;
	float angleRadians = atan2(targetDirection.y, targetDirection.x);
	float angleDegrees = CC_RADIANS_TO_DEGREES(angleRadians);

	if (angleDegrees >= -90 && angleDegrees <= 90)
	{
		entitySprite->setScaleX(1);
		if (weaponCurrent != WeaponType::EMPTY)
		{
			weaponSprite->setScaleX(1);
			weaponSprite->setRotation(-angleDegrees);
		}
	}
	else
	{
		entitySprite->setScaleX(-1);
		if (weaponCurrent != WeaponType::EMPTY)
		{
			weaponSprite->setScaleX(-1);
			weaponSprite->setRotation(-angleDegrees + 180);
		}
	}
}

//void EntityController::setSpeed(float _speed)
//{
//	speed = _speed;
//}

float EntityController::getSpeed()
{
	return speed;
}

float EntityController::getMaxSpeed()
{
	return maxSpeed;
}

void EntityController::takeDamage(float dmg)
{
	auto dmgTaken = dmg - entityData.armor;
	entityData.currentHP -= dmgTaken;
	auto color = entitySprite->getColor();
	entitySprite->setColor(Color3B::RED);

	updateHealthBar();

	scheduleOnce([=](float dt)
	{
		if (entitySprite->getColor() == Color3B::RED)
		{
			entitySprite->setColor(color);
		}
	}, 0.2f, "resumeColor");
	/*auto dmgText = to_string(-(int)dmgTaken);
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
	dmgLable->runAction(sequence);*/
}

void EntityController::healing(float heal)
{
	if (entityData.currentHP + heal >= entityData.health)
	{
		heal = entityData.health - entityData.currentHP;
	}
	entityData.currentHP += heal;

	updateHealthBar();

	auto healText = "+" + to_string((int)heal);

	Label* dmgLable = Label::createWithTTF(healText, "fonts/Marker Felt.ttf", 24);
	dmgLable->setPosition(Vec2(entitySize.width / 2, entitySize.height / 2 + 20));
	dmgLable->setColor(Color3B::GREEN);
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

void EntityController::setEffect(Effect _effect)
{
	effect = _effect;
}

void EntityController::removeEffect(Effect _effect)
{
	if (effect == _effect)
	{
		effect = Effect::NormalEffect;
	}
}

void EntityController::setEffect(Effect _effect, float time)
{
	effect = _effect;
	scheduleOnce([=](float dt)
	{
		removeEffect(_effect);
	}, time, "RemoveEffect");
}

void EntityController::setEffect(BulletType bullet, float time)
{
	switch (bullet)
	{
	case Fire:
		setEffect(Effect::FireEffect, time);
		break;
	case Boom:
		setEffect(Effect::FireEffect, time);
		break;
	case Light:
		setEffect(Effect::LightEffect, time);
		break;
	default:
		setEffect(Effect::NormalEffect);
		break;
	}
}

void EntityController::slow(float slowRate, float time)
{
	auto color = entitySprite->getColor();
	speed = maxSpeed * (1.0f - slowRate);
	entitySprite->setColor(Color3B::BLUE);
	scheduleOnce([=](float dt)
	{
		speed = maxSpeed;
		entitySprite->setColor(Color3B::WHITE);
	}, time, "slow");
}

void EntityController::onDie()
{
	this->unscheduleUpdate();
	this->stopAllActions();
	auto physicsBody = this->getPhysicsBody();
	if (physicsBody)
	{
		physicsBody->setCollisionBitmask(0);
		physicsBody->setContactTestBitmask(0);
		this->removeComponent(physicsBody);
	}
	removeFromParent();
	/*this->unscheduleUpdate();
	stopAllActions();
	entitySprite->setColor(Color3B::WHITE);
	setWeapon(WeaponType::EMPTY);
	auto parent = this->getParent();
	auto pos = this->getPosition();

	auto physicsBody = this->getPhysicsBody();
	if (physicsBody)
	{
		this->removeComponent(physicsBody);
	}

	setAnimationEntity("death_", 3, 0.2f, false);
	scheduleOnce([=](float dt)
	{
		removeFromParent();
	}, 0.6f, "Die");*/
}

bool EntityController::isDie()
{
	if (this->entityData.currentHP <= 0)
	{
		return true;
	}
	return false;
}

void EntityController::genericDeath()
{
	AudioManager::getInstance()->playSoundEffect("EnemyDie.mp3");
	auto sprite = Sprite::createWithSpriteFrameName("Enemy_death_0.png");
	sprite->setPosition(this->getPosition());

	Vector<SpriteFrame*> animFrames;
	auto frame1 = SpriteFrameCache::getInstance()->getSpriteFrameByName("Enemy_death_0.png");
	auto frame2 = SpriteFrameCache::getInstance()->getSpriteFrameByName("Enemy_death_1.png");
	auto frame3 = SpriteFrameCache::getInstance()->getSpriteFrameByName("Enemy_death_2.png");
	animFrames.pushBack(frame1);
	animFrames.pushBack(frame2);
	animFrames.pushBack(frame3);

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	auto animate = Animate::create(animation);
	sprite->runAction(animate);

	this->getParent()->addChild(sprite, 2);

	DelayTime* pause = DelayTime::create(0.6f);
	auto removeBullet = CallFunc::create([=]()
	{
		sprite->stopAllActions();
		sprite->removeFromParent();
	});

	auto sequence = Sequence::create(pause, removeBullet, nullptr);
	sprite->runAction(sequence);
}

void EntityController::explosive()
{
	AudioManager::getInstance()->playSoundEffect("boom.mp3");
	auto bullet = BulletController::create("explosion_2.png");
	bullet->setPosition(this->getPosition());
	auto dmg = this->entityData.health * 0.1f;
	bullet->setDmg(dmg);
	bullet->setAnimationBullet("explosion_", 3, 0.2f, false);
	bullet->setType(BulletType::Boom);
	
	this->getParent()->addChild(bullet, 3);

	DelayTime* pause = DelayTime::create(0.6f);
	auto removeBullet = CallFunc::create([=]()
	{
		bullet->destroy();
	});

	auto sequence = Sequence::create(pause, removeBullet, nullptr);
	bullet->runAction(sequence);
}

void EntityController::dropP3k(float rate)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 99);

	int randomNum = dist(gen);
	float flRandomNum = (float)randomNum;
	if (flRandomNum < rate)
	{
		BulletController* item = BulletController::create("p3k.png");
		item->setType(BulletType::Heal);
		item->setPosition(this->getPosition());
		item->setDmg(this->entityData.health * 0.4f);
		this->getParent()->addChild(item, 2);
	}
}

void EntityController::showHealthBar()
{
	healthBar = ProcessBar::create("barHealth.png", "bgHealth.png");
	healthBar->setPosition(Vec2(-entitySize.width/2, entitySize.height / 2));
	updateHealthBar();
	this->addChild(healthBar);
	healthBar->setVisible(false);
}

void EntityController::updateHealthBar()
{
	healthBar->setVisible(true);
	if (healthBar != nullptr)
	{
		auto percent = 100 * entityData.currentHP / entityData.health;
		healthBar->setPercentBarSprite(percent);
	}

	scheduleOnce([=](float dt)
	{
		healthBar->setVisible(false);
	}, 2.0f, "hideHealthBar");
}

float EntityController::getDmg()
{
	return entityData.attack;
}

