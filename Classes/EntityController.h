#ifndef __EntityController_H__
#define __EntityController_H__

#include "cocos2d.h"
#include "MoveController.h"
#include "BulletController.h"
#include "Enum.h"
#include <math.h>
#include "ProcessBar.h"
#include "AudioManager.h"

//#include <functional>
struct EntityData
{
	float attack; // công
	float armor; // giáp
	float asdp; // tốc độ đánh
	float bulletSpeed; // tốc độ bay của đạn
	float health; // máu
	float currentHP; // máu hiện tại
	float speed;

	EntityData(float _attack, float _armor, float _asdp, float _bulletSpeed, float _health, float _currentHP, float _speed)
		: attack(_attack), armor(_armor), asdp(_asdp), bulletSpeed(_bulletSpeed), health(_health), currentHP(_currentHP), speed(_speed) {}
	EntityData(){}
};

class EntityController : public MoveController
{
protected:
	Sprite* weaponSprite;
	ProcessBar* healthBar = nullptr;
	Size entitySize;//= Size(49.2, 70.4);
	Vec2 weaponPosition;

	//function<void(float)> shootFunc;

	WeaponType weaponCurrent;
	BulletType bulletCurrent;
	float interval = 0.0f;
	Vec2 target;

	Effect effect = Effect::NormalEffect;
	//vector<Weapon> allWeapons = { Weapon::USB, Weapon::AWM, Weapon::SHOTGUN};
	//int weaponIndex = 0;
public:
	Sprite* entitySprite;
	EntityData entityData;
	EntityController(const EntityData& _data, string fileName);
	//Animation* createAnimation(string prefixName, int pFramesOrder, float delay);
	void setAnimationEntity(string prefixName, int pFramesOrder, float delay, bool repeat);

	void setBitmask(int collision, int contactTest);
	void setBitmask(int collision, int contactTest, int category);

	void setWeaponSprite(const std::string& filename);
	void setWeapon(WeaponType weaponType);
	//void setWeapon(int weaponIndex);
	//void nextWeapon();
	void setBullet(BulletType bulleType);
	
	/*void setBodyEntity();
	void setBodyEntity(const Vec2& offset);
	void setBodyEntity(const Size& size, const Vec2& offset);*/

	void firingUSB(float dt, bool audio);
	void firingAMW(float dt, bool audio);
	void firingShotGun(float dt, bool audio);
	virtual void shoot(float dt);
	void updateDirection();

	//void setSpeed(float _speed);
	float getSpeed();
	float getMaxSpeed();

	float getDmg();
	virtual void takeDamage(float dmg);
	void healing(float heal);
	void setEffect(Effect _effect);
	void removeEffect(Effect _effect);
	void setEffect(Effect _effect, float time);
	void setEffect(BulletType bullet, float time);
	void slow(float slowRate, float time);
	virtual void onDie();
	bool isDie();
	BulletController* Drop(float rate);
	void dropP3k(float rate);
	void explosive();
	void genericDeath();
	void showHealthBar();
	void updateHealthBar();
};

#endif // __EntityController_H__

