#pragma once
#include <string>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <MadFrameworkExport.h>

class MAD_API SoundMananger
{
public:
	SoundMananger();

	~SoundMananger();

	void PlaySound(std::string soundName, bool loop = false);

	void PlayMusic(std::string musicName,bool loop=false);

	void StopSound(std::string soundName);

	void StopMusic(std::string musicName);

	void SetMasterVolume(float volume);

	void SetSoundVolume(float volume);

	void SetMusicVolume(float volume);

	void Mute(bool mute);

private:

	std::unordered_map<std::string, sf::SoundBuffer> m_soundsBuffer;
	std::unordered_map<std::string, sf::Sound> m_sounds;

	std::unordered_map<std::string,sf::Music*> m_musics;

	float m_masterVolume = 100.0f;
	float m_soundVolume = 100.0f;
	float m_musicVolume = 100.0f;

	bool m_mute = false;
};