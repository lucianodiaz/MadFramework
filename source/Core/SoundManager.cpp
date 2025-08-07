#include <Core/SoundManager.h>
#include <Core/World.h>

SoundMananger::SoundMananger()
{
}

SoundMananger::~SoundMananger()
{
}

void SoundMananger::PlaySound(std::string soundName, bool loop)
{
	m_soundsBuffer[soundName] = World::GetWorld()->GetSound(soundName);

	sf::Sound sound(m_soundsBuffer[soundName]);
	m_sounds[soundName] = sound;
	m_sounds[soundName].setVolume(m_soundVolume);
	m_sounds[soundName].setLoop(loop);
	m_sounds[soundName].play();
}

void SoundMananger::PlayMusic(std::string musicName, bool loop)
{
	m_musics[musicName] = &World::GetWorld()->GetMusic(musicName);

	m_musics[musicName]->setVolume(m_musicVolume);
	m_musics[musicName]->setLoop(loop);
	m_musics[musicName]->play();

}

void SoundMananger::StopSound(std::string soundName)
{
	m_sounds[soundName].stop();
}

void SoundMananger::StopMusic(std::string musicName)
{
	m_musics[musicName]->stop();
}

void SoundMananger::SetMasterVolume(float volume)
{
	if (volume < 0) { std::cout << "YOU CAN'T PUT VOLUME UNDER ZERO VALUE" << std::endl; return; }
	m_masterVolume = std::clamp(volume, 0.0f, 100.0f);;
}

void SoundMananger::SetSoundVolume(float volume)
{
	if (volume < 0) { std::cout << "YOU CAN'T PUT VOLUME UNDER ZERO VALUE" << std::endl; return; }
	m_soundVolume = std::clamp(volume, 0.0f, m_masterVolume);;
}

void SoundMananger::SetMusicVolume(float volume)
{
	if (volume < 0) { std::cout << "YOU CAN'T PUT VOLUME UNDER ZERO VALUE" << std::endl; return; }

	m_musicVolume = std::clamp(volume, 0.0f, m_masterVolume);
}

void SoundMananger::Mute(bool mute)
{
	m_mute = mute;

	m_masterVolume = (m_mute ? 0.0f : m_masterVolume);

	for (auto& music : m_musics)
	{
		music.second->setVolume(m_mute ? 0.0f : m_musicVolume);
	}

	for (auto& sound : m_sounds)
	{
		sound.second.setVolume(m_mute ? 0.0f : m_musicVolume);
	}
}
