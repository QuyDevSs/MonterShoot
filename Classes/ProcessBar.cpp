#include "ProcessBar.h"

ProcessBar* ProcessBar::create(const string& barFilename, const string& bgFilename)
{
	auto processBar = new ProcessBar();
	if (processBar && processBar->initWithFile(bgFilename)) {
		processBar->barSprite = LoadingBar::create(barFilename);
		processBar->barSprite->setPercent(processBar->percent);
		processBar->addChild(processBar->barSprite, 1);
		processBar->setAnchorPoint(Vec2(0, 0));
		processBar->barSprite->setAnchorPoint(Vec2(0, 0));
		processBar->autorelease();
		
		return processBar;
	}
	CC_SAFE_DELETE(processBar);
	return nullptr;
}

void ProcessBar::setPercentBarSprite(float rate)
{
	if (rate < 0)
	{
		rate = 0;
	}
	else if (rate > 100.0f)
	{
		rate = 100.0f;
	}

	percent = rate;
	barSprite->setPercent(percent);
}

float ProcessBar::getPercentBarSprite()
{
	return percent;
}
