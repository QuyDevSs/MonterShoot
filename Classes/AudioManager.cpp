// AudioManager.cpp

#include "AudioManager.h"

USING_NS_CC;

AudioManager::AudioManager()
{
	// Khởi tạo giá trị mặc định cho âm lượng âm thanh và nhạc
	soundVolume = 1.0f; // Âm lượng âm thanh mặc định là 100%
	musicVolume = 1.0f; // Âm lượng nhạc mặc định là 100%
	backgroundMusicID = AudioEngine::INVALID_AUDIO_ID; // Khởi tạo backgroundMusicID với giá trị không hợp lệ
}

AudioManager::~AudioManager() {}

AudioManager* AudioManager::getInstance()
{
	static AudioManager instance;
	return &instance;
}

void AudioManager::playSoundEffect(const std::string& soundEffectFileName)
{
	AudioEngine::play2d(soundEffectFileName, false, soundVolume);

}

void AudioManager::playBackgroundMusic(const std::string& musicFileName, bool loop)
{
	if (backgroundMusicID != AudioEngine::INVALID_AUDIO_ID)
	{
		// Dừng nhạc nền trước khi phát nhạc mới
		AudioEngine::stop(backgroundMusicID);
	}
	// Phát nhạc nền từ file có tên musicFileName
	backgroundMusicID = AudioEngine::play2d(musicFileName, loop, musicVolume);
}

void AudioManager::stopBackgroundMusic() 
{
	if (backgroundMusicID != AudioEngine::INVALID_AUDIO_ID) {
		// Dừng nhạc nền nếu đang phát
		AudioEngine::stop(backgroundMusicID);
		backgroundMusicID = AudioEngine::INVALID_AUDIO_ID;
	}
}

void AudioManager::setSoundVolume(float volume)
{
	if (volume < 0.0f)
	{
		volume = 0.0f;
	}
	else if (volume > 1.0f) 
	{
		volume = 1.0f;
	}
	soundVolume = volume;
}

void AudioManager::setMusicVolume(float volume)
{
	if (volume < 0.0f)
	{
		volume = 0.0f;
	}
	else if (volume > 1.0f)
	{
		volume = 1.0f;
	}
	musicVolume = volume;
	if (backgroundMusicID != AudioEngine::INVALID_AUDIO_ID)
	{
		AudioEngine::setVolume(backgroundMusicID, musicVolume);
	}
}

float AudioManager::getSoundVolume()
{
	return soundVolume;
}

float AudioManager::getMusicVolume()
{
	return musicVolume;
}