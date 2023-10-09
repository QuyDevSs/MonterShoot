#include "EnemyController.h"

USING_NS_CC;

EnemyController::EnemyController(const EntityData& data, string fileName) : EntityController(data, fileName), entityTarget(nullptr)
{
}

EnemyController::EnemyController(const EntityData& data, string filename, WeaponType weapon, BulletType bullet) : EntityController(data, filename), entityTarget(nullptr)
{
	auto prefixName = filename.erase(filename.length() - 5, 5);
	this->setAnimationEntity(prefixName, 8, 0.2f, true);
	this->setWeapon(weapon);
	this->setBullet(bullet);
	this->setTag(Tag::Entity);
	this->setBitmask(0x0001 | 0x0010, 0x0001 | 0x0100, 0x0010); // CATEGORY_PLAYER | CATEGORY_BULLET_PLAYER

	this->showHealthBar();
}

//EnemyController* EnemyController::create(const EntityData& data)
//{
//	EnemyController* enemyController = new EnemyController(data);
//	if (enemyController && enemyController->init()) {
//		enemyController->autorelease();
//		return enemyController;
//	}
//	else {
//		delete enemyController;
//		return nullptr;
//	}
//}

EnemyController* EnemyController::create(const EntityData& data, string filename, WeaponType weapon, BulletType bullet)
{
	EnemyController* enemyController = new EnemyController(data, filename, weapon, bullet);
	
	if (enemyController && enemyController->init())
	{
		enemyController->autorelease();
		return enemyController;
	}
	else
	{
		delete enemyController;
		return nullptr;
	}
}

EnemyController* EnemyController::create(const EntityData& data, EnemyType enemyType)
{
	EnemyController* enemyController;
	auto newData = data;
	newData.currentHP = data.currentHP *1.5f;
	newData.health = data.health *1.5f;

	auto dataFaster = data;
	dataFaster.speed = data.speed * 1.5f;

	auto dataBoss = data;
	dataBoss.currentHP = data.currentHP *10.0f;
	dataBoss.health = data.health *10.0f;
	dataBoss.asdp = data.asdp * 10.0f;
	
	switch (enemyType)
	{
	case EnemyArcher:
		enemyController = new EnemyController(data, "Enemy1_walk_0.png", WeaponType::USB, BulletType::EnemyBullet);
		break;
	case EnemyTanker:
		enemyController = new EnemyController(newData, "Enemy2_walk_0.png", WeaponType::EMPTY, BulletType::EnemyBullet);
		break;
	case EnemyFaster:
		enemyController = new EnemyController(dataFaster, "Enemy3_walk_0.png", WeaponType::EMPTY, BulletType::EnemyBullet);
		break;
	case EnemyBoss:
		enemyController = new EnemyController(dataBoss, "Enemy4_walk_0.png", WeaponType::SHOTGUN, BulletType::EnemyBullet);
		enemyController->showHealthBar();
		enemyController->setScale(3.0f);
		break;
	default:
		break;
	}
	if (enemyController && enemyController->init())
	{
		enemyController->autorelease();
		return enemyController;
	}
	else
	{
		delete enemyController;
		return nullptr;
	}

}

void EnemyController::update(float dt)
{
	if (isDie())
	{
		switch (effect)
		{
		case FireEffect:
			explosive();
			break;
		case IceEffect:
			break;
		case LightEffect:
			dropP3k(50.0f);
			break;
		case NormalEffect:
			break;
		default:
			break;
		}
		PlayerController* player = (PlayerController*)entityTarget;
		player->setScore(player->getScore() + 1);
		genericDeath();
		onDie();
	}
	else
	{
		if (entityTarget)
		{
			target = entityTarget->getPosition();
		}
		updateDirection();
		shoot(dt);
		EnemyMove(dt);
	}
	
	/*static float interval1 = 0;
	if (interval1 >= 1 )
	{
		auto pos = this->getPosition();
		CCLOG("posenemy: %f, %f", pos.x, pos.y);
		interval1 = 0.0f;
	}
	else
	{
		interval1 += dt;
	}*/
	
}

void EnemyController::EnemyMove(float dt)
{
	if (entityTarget)
	{
		//// Tính toán vector hướng từ enemy đến người chơi
		Vec2 direction = target - getPosition();
		//direction.normalize();

		//// Tính toán vị trí mới cho enemy dựa trên vector hướng
		//Vec2 newPosition = getPosition() + direction * speed * dt;

		//// Áp dụng vị trí mới cho enemy
		//setPosition(newPosition);
		MoveInDir(direction, dt);
	}
}

//void  EnemyController::enemyFiringUSB(float dt)
//{
//	auto asdpIncreased = 2.0f;
//	if (interval >= 1 / (this->entityData.asdp * (1 + asdpIncreased)))
//	{
//		firingUSB();
//		CCLOG("shoot");
//		interval = 0.0f;
//	}
//	else
//	{
//		interval += dt;
//	}
//}
//
//void  EnemyController::enemyFiringAMW(float dt)
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
//void  EnemyController::enemyFiringShotGun(float dt)
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
//void EnemyController::shoot(float dt)
//{
//	switch (weaponCurrent)
//	{
//	case USB:
//		enemyFiringUSB(dt);
//		break;
//	case AWM:
//		enemyFiringAMW(dt);
//		break;
//	case SHOTGUN:
//		enemyFiringShotGun(dt);
//		break;
//	case EMPTY:
//		break;
//	default:
//		break;
//	}
//}
