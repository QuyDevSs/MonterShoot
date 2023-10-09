#ifndef __EnemyController_H__
#define __EnemyController_H__

#include "EntityController.h"
#include "PlayerController.h"

class EnemyController : public EntityController
{
public:
	EntityController* entityTarget;

	EnemyController(const EntityData& data, string fileName);
	EnemyController(const EntityData& data, string filename, WeaponType weapon, BulletType bullet);
	//static EnemyController* create(const EntityData& data, string fileName);
	static EnemyController* create(const EntityData& data, string filename, WeaponType weapon, BulletType bullet);
	static EnemyController* create(const EntityData& data, EnemyType enemyType);
	void update(float dt);
	void EnemyMove(float dt);
};

#endif // __EnemyController_H__
