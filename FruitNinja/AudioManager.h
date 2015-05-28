#pragma once
#include "fmodex\fmod.hpp"
#include <glm/glm.hpp>
#include <map>

class AudioManager
{
private:
	static AudioManager *amInstance;

	FMOD::System *system;
	FMOD::Sound *ambientSound;
	FMOD::Channel *ambientChannel;
	std::map<std::string, FMOD::Sound*> soundMap;

	void FMODErrorCheck(FMOD_RESULT result);
	AudioManager();
public:
	static AudioManager* instance();

	void playAmbient(std::string filename);
	void updateListener(glm::vec3 position, glm::vec3 forward, glm::vec3 up);
	FMOD::Channel *play3DLoop(std::string filename, glm::vec3 position, bool loop);
	void updateChannelPosition(FMOD::Channel *channel, glm::vec3 position);
};