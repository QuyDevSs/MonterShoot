#ifndef __ProcessBar_H__
#define __ProcessBar_H__

#include "cocos2d.h"
#include "ui/UILoadingBar.h"

using namespace cocos2d;
using namespace std;
using namespace ui;

class ProcessBar : public Sprite
{
private:
	float percent = 100.0f;
	LoadingBar* barSprite;
public:
	static ProcessBar* create(const string& barFilename, const string& bgFilename);
	void setPercentBarSprite(float rate);
	float getPercentBarSprite();
};

#endif // __ProcessBar_H__

