#ifndef __MOVECONTROLLER_H__
#define __MOVECONTROLLER_H__

#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class MoveController : public Node
{
protected:
	float speed;
	float maxSpeed;
	//void Move(Vec2 pos); //Di chuyển đến pos
	//void Move(Vec2 pos, CallFunc* endMove); // Di chuyển đến pos và thực hiện endMove
	//void Move(Vec2 pos, float range, CallFunc* endMove);
	//void MoveRotate(Vec2 pos, float range, CallFunc* endMove);
	//void Move(CallFunc* startMove, Vec2 pos);
	
	void MoveRotate(Vec2 dir, float  deltaAngle, float range, CallFunc* endMove);
	void MoveInDir(Vec2 dir, float dt); // di chuyển theo hướng
	void MoveInDir(Vec2 dir, float range, CallFunc* endMove); // di chuyển theo hướng đủ range và thực hiện endMove
public:
	void setSpeed(float _speed);
};

#endif // __MOVECONTROLLER_H__
