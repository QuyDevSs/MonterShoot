#ifndef __Enum_H__
#define __Enum_H__

enum Tag
{
	Bullet,
	Entity,
};

enum Effect
{
	FireEffect,
	IceEffect,
	LightEffect,
	NormalEffect
};

enum StatePlayer
{
	MovingUp,
	MovingDown,
	MovingLeft,
	MovingRight,
	IDLE,
	MovingUpRight,
	MovingUpLeft,
	MovingDownRight,
	MovingDownLeft
};

enum WeaponType
{
	USB,
	AWM,
	SHOTGUN,
	//USBENEMY,
	EMPTY
};

enum BulletType
{
	Fire,
	Ice,
	Wind,
	Light,
	Dark,
	EnemyBullet,
	Heal,
	Boom
};

enum EnemyType
{
	EnemyArcher,
	EnemyTanker,
	EnemyFaster,
	EnemyBoss
};
#endif // __Enum_H__
