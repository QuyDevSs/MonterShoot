#include "BulletController.h"

USING_NS_CC;

//BulletController::BulletController(const BulletData& data) : bulletData(data)
//{
//	bulletSprite = Sprite::create(bulletData.bulletFilename);
//	bulletSprite->setPosition(Vec2(0, 0));
//	//bulletSprite->setPosition(Vec2(50, 0));
//	this->addChild(bulletSprite);
//	bulletBody = PhysicsBody::createCircle(12);
//	/*bulletBody->setCollisionBitmask(bulletData.collisionBitmaskBullet);
//	bulletBody->setContactTestBitmask(bulletData.contactTestBitmaskBullet);*/
//
//	//bulletBody->setCategoryBitmask(0x02);
//	bulletBody->setCollisionBitmask(0x01);
//	bulletBody->setContactTestBitmask(0x01);
//	
//	this->setPhysicsBody(bulletBody);
//	this->setTag(bulletData.tagBullet);
//	this->speed = bulletData.speedBullet;
//}

//BulletController::BulletController(float _speed, const std::string& filename) : bulletSprite(Sprite::create())
//{
//	speed = _speed;
//
//}

//BulletController::BulletController(const BulletType& bullet)
//{
//	switch (bullet)
//	{
//	case Fire:
//		bulletSprite = Sprite::create("BulletFire.png");
//		this->addChild(bulletSprite);
//		bulletSprite->setTag(Tag::BulletFire);
//
//		bulletBody = PhysicsBody::createCircle(12);
//		//bulletBody->setCategoryBitmask(0x02);
//		bulletBody->setCollisionBitmask(0x00);
//		bulletBody->setContactTestBitmask(0x01);
//		bulletSprite->setPhysicsBody(bulletBody);
//		break;
//	case Ice:
//		bulletSprite = Sprite::create("BulletIce.png");
//		this->addChild(bulletSprite);
//		bulletSprite->setTag(Tag::BulletIce);
//
//		bulletBody = PhysicsBody::createCircle(12);
//		//bulletBody->setCategoryBitmask(0x02);
//		bulletBody->setCollisionBitmask(0x00);
//		bulletBody->setContactTestBitmask(0x01);
//		bulletSprite->setPhysicsBody(bulletBody);
//		break;
//	case Wind:
//		bulletSprite = Sprite::create("BulletWind.png");
//		this->addChild(bulletSprite);
//		bulletSprite->setTag(Tag::BulletWind);
//
//		bulletBody = PhysicsBody::createCircle(12);
//		//bulletBody->setCategoryBitmask(0x02);
//		bulletBody->setCollisionBitmask(0x00);
//		bulletBody->setContactTestBitmask(0x01);
//		bulletSprite->setPhysicsBody(bulletBody);
//
//		break;
//	case Light:
//		bulletSprite = Sprite::create("BulletLight.png");
//		this->addChild(bulletSprite);
//		bulletSprite->setTag(Tag::BulletLight);
//
//		bulletBody = PhysicsBody::createCircle(12);
//		//bulletBody->setCategoryBitmask(0x02);
//		bulletBody->setCollisionBitmask(0x00);
//		bulletBody->setContactTestBitmask(0x01);
//		bulletSprite->setPhysicsBody(bulletBody);
//		break;
//	case Dark:
//		bulletSprite = Sprite::create("BulletDark.png");
//		this->addChild(bulletSprite);
//		bulletSprite->setTag(Tag::BulletDark);
//
//		bulletBody = PhysicsBody::createCircle(12);
//		//bulletBody->setCategoryBitmask(0x02);
//		bulletBody->setCollisionBitmask(0x00);
//		bulletBody->setContactTestBitmask(0x01);
//		bulletSprite->setPhysicsBody(bulletBody);
//		break;
//	case EnemyBullet:
//		bulletSprite = Sprite::create("BulletEnemy.png");
//		this->addChild(bulletSprite);
//		bulletSprite->setTag(Tag::BulletEnemy);
//
//		bulletBody = PhysicsBody::createCircle(12);
//		//bulletBody->setCategoryBitmask(0x02);
//		bulletBody->setCollisionBitmask(0x00);
//		bulletBody->setContactTestBitmask(0x01);
//		bulletSprite->setPhysicsBody(bulletBody);
//		break;
//	default:
//		break;
//	}
//}


BulletController::BulletController(string fileName)
{
	auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//bulletSprite = Sprite::createWithSpriteFrameName(fileName);
	bulletSprite = Sprite::create(fileName);
	this->addChild(bulletSprite);

	auto bulletSize = bulletSprite->getContentSize();
	bulletSize = Size(bulletSize.width * visibleSize.width / frameSize.width,
		bulletSize.height * visibleSize.height / frameSize.height);

	auto bulletBody = PhysicsBody::createCircle(bulletSize.height / 2);
	this->setPhysicsBody(bulletBody);
	this->setTag(Tag::Bullet);
}

BulletController::BulletController(const BulletType& bullet)
{
	switch (bullet)
	{
	case Fire:
		bulletSprite = Sprite::create("BulletFire.png");
		break;
	case Ice:
		bulletSprite = Sprite::create("BulletIce.png");
		break;
	case Wind:
		bulletSprite = Sprite::create("BulletWind.png");
		break;
	case Light:
		bulletSprite = Sprite::create("BulletLight.png");
		break;
	case Dark:
		bulletSprite = Sprite::create("BulletDark.png");
		break;
	case EnemyBullet:
		bulletSprite = Sprite::create("BulletEnemy.png");
		break;
	default:
		break;
	}

	this->addChild(bulletSprite);
	auto bulletBody = PhysicsBody::createCircle(bulletSprite->getContentSize().height / 2);
	this->setPhysicsBody(bulletBody);
	this->setType(bullet);
	this->setTag(Tag::Bullet);
}

BulletController* BulletController::create(string fileName)
{
	BulletController* bulletController = new BulletController(fileName);
	if (bulletController && bulletController->init())
	{
		bulletController->autorelease();
		return bulletController;
	}
	else {
		delete bulletController;
		return nullptr;
	}
}

BulletController* BulletController::create(const BulletType& bullet)
{
	BulletController* bulletController = new BulletController(bullet);
	if (bulletController && bulletController->init())
	{
		bulletController->autorelease();
		return bulletController;
	}
	else {
		delete bulletController;
		return nullptr;
	}
}

void BulletController::setBitmask(int collision, int contactTest)
{
	auto physicsBody = bulletSprite->getPhysicsBody();
	if (physicsBody)
	{
		physicsBody->setCollisionBitmask(collision);
		physicsBody->setContactTestBitmask(contactTest);
	}

}

void BulletController::setBitmask(int collision, int contactTest, int category)
{
	auto physicsBody = this->getPhysicsBody();
	if (physicsBody)
	{
		physicsBody->setCategoryBitmask(category);
		physicsBody->setCollisionBitmask(collision);
		physicsBody->setContactTestBitmask(contactTest);
	}
}

//BulletController* BulletController::create(const BulletData& data) {
//	BulletController* bulletController = new BulletController(data);
//	if (bulletController && bulletController->init()) {
//		bulletController->autorelease();
//		return bulletController;
//	}
//	else {
//		delete bulletController;
//		return nullptr;
//	}
//}

//void BulletController::setbulletSpritePos(Vec2 pos)
//{
//	bulletSprite->setPosition(pos);
//}

void BulletController::bulletSpriteMove(Vec2 dir, float range, float speed)
{
	Vec2 newPosition = getPosition() + (dir * range);
	float distance = dir.length() * range;
	float moveTime = distance / speed;

	// Di chuyển bulletSprite
	MoveTo* moveAction = MoveTo::create(moveTime, newPosition);
	bulletSprite->runAction(moveAction);
}

//void BulletController::setTagBullet(int tag)
//{
//	this->setTag(tag);
//}
//
//void BulletController::setCollisionBitmaskBullet(int bitmask)
//{
//	bulletBody->setCollisionBitmask(bitmask);
//}
//
//void BulletController::setContactTestBitmaskBullet(int bitmask)
//{
//	bulletBody->setContactTestBitmask(bitmask);
//}
//
//void BulletController::setBulletType(BulletData bulletData)
//{
//	bulletSprite->setTag(bulletData.tagBullet);
//	bulletBody->setCollisionBitmask(bulletData.collisionBitmaskBullet);
//	bulletBody->setContactTestBitmask(bulletData.contactTestBitmaskBullet);
//	bulletSprite->setTexture(bulletData.bulletFilename);
//}

//BulletController* BulletController::create()
//{
//	BulletController * ret = new (std::nothrow) BulletController();
//	if (ret && ret->init())
//	{
//		ret->autorelease();
//	}
//	else
//	{
//		CC_SAFE_DELETE(ret);
//	}
//	return ret;
//}

//BulletModel* BulletController::createBulletModel(Vec2 pos)
//{
//	BulletModel* bulletModel = BulletModel::create(_fileNameBullet);
//	bulletModel->setPosition(pos);
//
//	auto targetBody = PhysicsBody::createCircle(bulletModel->getContentSize().width / 2);
//	targetBody->setCollisionBitmask(3);
//	targetBody->setContactTestBitmask(1);
//	//targetBody->setCategoryBitmask(2);
//	bulletModel->setPhysicsBody(targetBody);
//	bulletModel->setScale(bulletModel->ratio);
//	this->addChild(bulletModel);
//
//	return bulletModel;;
//}

//void BulletController::MoveStraight(Vec2 pos)
//{
//	createBulletModel(Vec2(0, 0));
//	auto removeAction = CallFunc::create([=]()
//	{
//		if (this != NULL)
//		{
//			this->stopAllActions();
//			this->removeFromParent();
//		}
//	});
//	this->Move(pos, removeAction);
//}
//
//void BulletController::MoveStraight(Vec2 targetPos, float range)
//{
//	auto removeAction = CallFunc::create([=]()
//	{
//		if (this != NULL)
//		{
//			this->stopAllActions();
//			this->removeFromParent();
//		}
//	});
//	this->Move(targetPos, range, removeAction);
//}

void BulletController::BulletMove(Vec2 dir, float range)
{
	auto removeAction = CallFunc::create([=]()
	{
		if (this != NULL)
		{
			this->destroy();
		}
	});
	this->MoveInDir(dir, range, removeAction);
}

//void BulletController::MoveStraightRotate(Vec2 targetPos, float range)
//{
//	auto removeAction = CallFunc::create([=]()
//	{
//		if (this != NULL)
//		{
//			this->stopAllActions();
//			this->removeFromParent();
//		}
//	});
//	this->MoveRotate(targetPos, range, removeAction);
//}

void BulletController::BulletMoveRotate(Vec2 dir, float deltaAngle, float range)
{
	auto removeAction = CallFunc::create([=]()
	{
		if (this != NULL)
		{
			this->destroy();
		}
	});
	this->MoveRotate(dir, deltaAngle, range, removeAction);
}

//
//void BulletController::MoveCircle(float r, int num)
//{
//	vector<Vec2> posSpriteBullet = SPCalculation::create_pos_bullet(Vec2(0, r), num);
//	auto pi = acos(-1.0);
//	auto perimeter = 2 * pi * r;
//	for (int i = 0; i < num; i++)
//	{
//		auto bulletModel = createBulletModel(posSpriteBullet[i]);
//		bulletModel->setTag(tag);
//		bulletModel->attack = this->attack;
//	}
//	auto action = RotateBy::create(perimeter / speed, 720);
//	auto removeAction = CallFunc::create([=]()
//	{
//		if (this != NULL)
//		{
//			this->stopAllActions();
//			this->removeFromParent();
//		}
//	});
//	auto sequence = Sequence::create(action, removeAction, nullptr);
//	this->runAction(sequence);
//}

void BulletController::setType(BulletType bullet)
{
	bulletType = bullet;
	switch (bullet)
	{
	case Fire:
		setBitmask(0, 0x0010, 0x0100);// CATEGORY_ENEMY
		break;
	case Ice:
		setBitmask(0, 0x0010, 0x0100);// CATEGORY_ENEMY
		break;
	case Wind:
		setBitmask(0, 0x0010, 0x0100);// CATEGORY_ENEMY
		break;
	case Light:
		setBitmask(0, 0x0010, 0x0100);// CATEGORY_ENEMY
		break;
	case Dark:
		setBitmask(0, 0x0010 | 0x1000, 0x0100);// CATEGORY_ENEMY
		break;
	case EnemyBullet:
		setBitmask(0, 0x0001 | 0x0100, 0x1000); //CATEGORY_PLAYER
		break;
	case Heal:
		setBitmask(0x10000, 0x0001 , 0x10000); //CATEGORY_HEAL
		//setBitmask(0x10000 | 0x0001, 0x0001 | 0x0100, 0x1000);
		break;
	case Boom:
		setBitmask(0, 0x0010, 0x0100);// CATEGORY_ENEMY
		break;
	default:
		break;
	}
}

BulletType BulletController::getType()
{
	return bulletType;
}

void BulletController::setDmg(float _dmg)
{
	dmg = _dmg;
}

float BulletController::getDmg()
{
	return dmg;
}

void BulletController::setAnimationBullet(string prefixName, int pFramesOrder, float delay, bool repeat = true)
{
	//bulletSprite->stopAllActions();
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
		bulletSprite->runAction(RepeatForever::create(animate));
	}
	else
	{
		bulletSprite->runAction(animate);
	}
}

void BulletController::destroy()
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
	this->removeFromParent();
}