#ifndef __BulletController_H__
#define __BulletController_H__

#include "MoveController.h"
#include "Enum.h"

//struct BulletData
//{
//	int tagBullet;
//	int collisionBitmaskBullet;
//	int contactTestBitmaskBullet;
//	string bulletFilename;
//	float speedBullet;
//	float attack;
//
//	BulletData(int _tag, int _collisionBitmask, int _contactTestBitmask, const std::string& _bulletFilename, float _speed, float _attack)
//		: tagBullet(_tag), collisionBitmaskBullet(_collisionBitmask), contactTestBitmaskBullet(_contactTestBitmask), bulletFilename(_bulletFilename), speedBullet(_speed) , attack(_attack) {}
//	BulletData() : tagBullet(0), collisionBitmaskBullet(0), contactTestBitmaskBullet(0), bulletFilename("Bullet.png"), speedBullet(1000.0f) , attack(10) {}
//};

class BulletController : public MoveController
{
protected:
	BulletType bulletType;
	float dmg = 0;
public:
	Sprite* bulletSprite;
	PhysicsBody* bulletBody;

	void setBitmask(int collision, int contactTest);
	void setBitmask(int collision, int contactTest, int category);
	//BulletData bulletData;
	//BulletController(const BulletData& data);
	//BulletController(float _speed, const std::string& filename);
	BulletController(string fileName);
	BulletController(const BulletType& bullet);
	static BulletController* create(string fileName);
	static BulletController* create(const BulletType& bullet);
	//static BulletController* create(const BulletData& data);
	//void setbulletSpritePos(Vec2 pos);
	void bulletSpriteMove(Vec2 dir, float range, float speed);
	//void setTagBullet(int tag);
	//void setCollisionBitmaskBullet(int bitmask);
	//void setContactTestBitmaskBullet(int bitmask);
	//void setBulletType(BulletData bulletData);
	//void MoveStraight(Vec2 targetPos, float range);
	void BulletMove(Vec2 dir, float range);
	//void MoveStraightRotate(Vec2 targetPos, float range);
	void BulletMoveRotate(Vec2 dir, float deltaAngle, float range);

	void setType(BulletType bullet);
	BulletType getType();
	void setDmg(float _dmg);
	float getDmg();

	void setAnimationBullet(string prefixName, int pFramesOrder, float delay, bool repeat);
	void destroy();
};

#endif // __BulletController_H__

