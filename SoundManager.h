#pragma once
#include "audiere/include/audiere.h"
#include <iostream>
#include <stdio.h>
using namespace audiere;

#define SOUND_OVERLAP 3

class SoundManager
{
public:
	typedef enum SoundID{
		SHOOT,
		SAW_HIT,
		BLOB_PICKUP,
		MAX_SOUNDS
	};
	int soundLevel;
	int musicLevel;

private:	
	char* soundfiles[MAX_SOUNDS];

	AudioDevicePtr* device;
	OutputStreamPtr* stream;
	OutputStreamPtr* sound[MAX_SOUNDS][SOUND_OVERLAP];

public:
	SoundManager();
	bool music();
	bool play(SoundID id);
	char* getSoundLevel();
	void setSoundLevel(int level);
	char* getMusicLevel();
	void setMusicLevel(int level);
};

SoundManager::SoundManager()
{
	musicLevel = 5;
	soundLevel = 10;
	soundfiles[SHOOT]		= "sound/shoot.mp3";
	soundfiles[SAW_HIT]		= "sound/explode.wav";
	soundfiles[BLOB_PICKUP] = "sound/Blob Pickup.wav";

	cout << "Initializing soundsystem:";
	device = new AudioDevicePtr(OpenDevice());
	
	if(*device)
	{
		cout << "OK" << endl << "Loading sounds";
		for(int i=0; i<MAX_SOUNDS; i++)
		{
			for(int j=0; j<SOUND_OVERLAP; j++)
			{
				sound[i][j] = new OutputStreamPtr(OpenSound(*device, soundfiles[i], false));
				cout << ".";
			}
		}
		cout << endl;
	}
	else
	{
		cout << "Failed!";
	}
}

bool SoundManager::music()
{
	if(!*device)
	{
		return false;
	}
	stream = new OutputStreamPtr(OpenSound(*device, "sound/intromusic.mp3", false));
	if (!*stream) {
		return false;
	}
	(*stream)->setRepeat(true);
	(*stream)->setVolume(0.1f * musicLevel); // 50% volume
	(*stream)->play();

	return true;
}

bool SoundManager::play(SoundID id)
{
	if(*device)
	{
		for(int i=0; i<SOUND_OVERLAP; i++)
		{
			if(*sound[id][i])
			{
				if((*sound[id][i])->isPlaying()) continue;
				else (*sound[id][i])->play();
				break;
			}
		}
	}
	return true;
}

char* SoundManager::getMusicLevel()
{
	char* c1 = new char[8];
	sprintf(c1, "MUSIC %d", musicLevel);
	return c1;
}

char* SoundManager::getSoundLevel()
{
	char* c1 = new char[8];
	sprintf(c1, "SOUND %d", soundLevel);
	return c1;
}

void SoundManager::setMusicLevel(int level)
{
	if(level >= 0 && level <= 10)
		musicLevel = level;
	(*stream)->setVolume(0.1 * musicLevel);
}

void SoundManager::setSoundLevel(int level)
{
	if(level >= 0 && level <= 10)
		soundLevel = level;
	for(int i=0; i<MAX_SOUNDS; i++)
	{
		for(int j=0; j<SOUND_OVERLAP; j++)
		{
			(*sound[i][j])->setVolume(0.1 * soundLevel);
		}
	}
}


