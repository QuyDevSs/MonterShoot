#include "MoveController.h"

USING_NS_CC;

//void MoveController::Move(Vec2 pos)
//{
//	auto distance = this->getPosition().distance(pos);
//	auto moveAction = MoveTo::create(distance / speed, pos);
//	this->runAction(moveAction);
//}
//
//void MoveController::Move(Vec2 pos, CallFunc* endMove)
//{
//	auto distance = this->getPosition().distance(pos);
//	auto moveAction = MoveTo::create(distance / speed, pos);
//	auto sequence = Sequence::create(moveAction, endMove, nullptr);
//	this->runAction(sequence);
//}

//void MoveController::Move(Vec2 pos, float range, CallFunc* endMove)
//{
//	Vec2 dir = (pos - this->getPosition()).getNormalized();
//	Vec2 v3 = this->getPosition() + dir * range;
//	auto moveAction = MoveTo::create(range / speed, v3);
//	auto sequence = Sequence::create(moveAction, endMove, nullptr);
//	this->runAction(sequence);
//}
//
//void MoveController::MoveRotate(Vec2 pos, float range, CallFunc* endMove)
//{
//	Vec2 dir = (pos - this->getPosition()).getNormalized();
//	Vec2 v3 = this->getPosition() + dir * range;
//	auto rotateAction = RepeatForever::create(RotateBy::create(1.0f, 180.0f));
//	auto moveAction = MoveTo::create(range / speed, v3);
//	auto sequence = Sequence::create(moveAction, endMove, nullptr);
//	this->runAction(sequence);
//}

void MoveController::MoveRotate(Vec2 dir, float deltaAngle, float range, CallFunc* endMove)
{
	auto physicsBody = getPhysicsBody();
	// Tính toán vị trí mới dựa trên khoảng cách range
	Vec2 newPosition = getPosition() + (dir * range);

	// Tính toán thời gian cần để di chuyển đến vị trí mới
	float distance = dir.length() * range;
	float moveTime = distance / speed;

	// Di chuyển đến vị trí mới trong khoảng thời gian moveTime
	MoveTo* moveAction = MoveTo::create(moveTime, newPosition);

	// Hành động xoay cố định một vòng mỗi giây
	RotateBy* rotateAction = RotateBy::create(moveTime, deltaAngle * moveTime);

	// Kết hợp hành động di chuyển và hành động xoay cố định
	Spawn* spawnAction = Spawn::create(moveAction, rotateAction, nullptr);

	// Tạo hành động sequence kết hợp di chuyển, xoay cố định và endMove
	if (endMove)
	{
		Sequence* sequence = Sequence::create(spawnAction, endMove, nullptr);
		runAction(sequence);
		//runAction(RepeatForever::create(sequence));
	}
	else
	{
		runAction(spawnAction);
		//runAction(RepeatForever::create(spawnAction));
	}
}

//void MoveController::Move(CallFunc* startMove, Vec2 pos)
//{
//	auto distance = this->getPosition().distance(pos);
//	auto moveAction = MoveTo::create(distance / speed, pos);
//	auto sequence = Sequence::create(startMove, moveAction, nullptr);
//	this->runAction(sequence);
//}

void MoveController::MoveInDir(Vec2 dir, float dt)
{
	float distance = dt * speed;
	Vec2 movement = dir.getNormalized() * distance;
	this->setPosition(getPosition() + movement);
}

void MoveController::MoveInDir(Vec2 dir, float range, CallFunc* endMove)
{
	// Tính toán vị trí mới dựa trên khoảng cách range
	Vec2 newPosition = getPosition() + (dir * range);

	// Tính toán thời gian cần để di chuyển đến vị trí mới
	float distance = dir.length() * range;
	float moveTime = distance / speed;

	// Di chuyển đến vị trí mới trong khoảng thời gian moveTime
	MoveTo* moveAction = MoveTo::create(moveTime, newPosition);

	// Kết hợp hành động di chuyển với hành động kết thúc
	if (endMove)
	{
		Sequence* sequence = Sequence::create(moveAction, endMove, nullptr);
		runAction(sequence);
	}
	else
	{
		runAction(moveAction);
	}
}

void MoveController::setSpeed(float _speed)
{
	this->speed = _speed;
	this->maxSpeed = _speed;
}