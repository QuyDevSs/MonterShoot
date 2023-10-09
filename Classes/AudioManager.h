#ifndef __AudioManager_H__
#define __AudioManager_H__

#include "cocos2d.h"
#include "AudioEngine.h"

using namespace cocos2d;
using namespace std;

class AudioManager
{
public:
	static AudioManager* getInstance();

	void playSoundEffect(const std::string& soundEffectFileName);
	void playBackgroundMusic(const std::string& musicFileName, bool loop = true);
	void stopBackgroundMusic();
	void setSoundVolume(float volume);
	void setMusicVolume(float volume);
	float getSoundVolume();
	float getMusicVolume();
private:
	AudioManager();
	~AudioManager();

	int backgroundMusicID;
	float soundVolume;
	float musicVolume;
};

#endif // __AudioManager_H__