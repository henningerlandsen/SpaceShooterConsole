#pragma once
#include <iostream>
#include "SoundManager.h"
using namespace std;

class MenuActions
{
	static SoundManager* _snd;
public:
	//MenuItem functions
	static void mitStartGame_onSelect();
	static void mitSoundVolume_onUp();
	static void mitSoundVolume_onDown();
	static void mitMusicVolume_onUp();
	static void mitMusicVolume_onDown();
	static void registerSoundManager(SoundManager* ptr);
};

SoundManager* MenuActions:: _snd = 0;

void MenuActions::mitSoundVolume_onDown()
{
	if(_snd != 0)
		_snd->setSoundLevel(_snd->soundLevel-1);
}

void MenuActions::mitSoundVolume_onUp()
{
	if(_snd != 0)
		_snd->setSoundLevel(_snd->soundLevel+1);
}

void MenuActions::mitMusicVolume_onDown()
{
	if(_snd != 0)
		_snd->setMusicLevel(_snd->musicLevel-1);
}

void MenuActions::mitMusicVolume_onUp()
{
	if(_snd != 0)
		_snd->setMusicLevel(_snd->musicLevel+1);
}

void MenuActions::mitStartGame_onSelect()
{
	cout << "Start game" << endl;
}

void MenuActions::registerSoundManager(SoundManager *ptr)
{
	_snd = ptr;
}
