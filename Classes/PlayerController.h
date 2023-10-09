#ifndef __PlayerController_H__
#define __PlayerController_H__

#include "EntityController.h"

class PlayerController : public EntityController
{
private:
	bool isMovingUp = false;
	bool isMovingDown = false;
	bool isMovingRight = false;
	bool isMovingLeft = false;

	Sprite* crosshair;
	int score = 0;
public:
	PlayerController(const EntityData& data, string fileName);
	static PlayerController* create(const EntityData& data, string fileName);
	StatePlayer state;
	EventListenerKeyboard* keyboardListener;
	EventListenerMouse* mouseListener;
	void movePlayer(float dt);
	void updateState();
	void shoot(float dt) override;
	void update(float dt);
	void onKeyPressed(EventKeyboard::KeyCode key, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode key, Event* event);
	void updateCrosshair(Event *event);
	void takeDamage(float dmg) override;
	void nextWeapon();
	void onDie() override;
	void setScore(int _score);
	int getScore();

	/*void playerFiringUSB(float dt);
	void playerFiringAMW(float dt);
	void playerFiringShotGun(float dt);
	void shoot(float dt);*/
};

#endif // __PlayerController_H__
