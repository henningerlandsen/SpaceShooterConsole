#pragma once
#include "IUpgradable.h"
#include "Upgrade.h"
#include "Vector2d.h"
#include "WeaponManager.h"
#include "UpgradableShield.h"
#include "SpaceShip.h"

#define MAX_BLOBS 20
#define UPGRADE_LEVEL_GROWTH_PERCENTAGE 120
#define SUPER_LEVEL 1

class UpgradeManager : public IDrawable
{
	enum UpgradableSectors {
		US_WEAPONS,
		US_MOVEMENT,
		US_SHIELD,
		MAX_UPGRADABLES
	};

public:
	struct Upgradable {
		IUpgradable* p;
		int level;
		int limit;
		int points;
		DrawingAssistant::TextureNum texture;
	};					

	Upgradable uSectors[MAX_UPGRADABLES];
	Upgrade* blobs[MAX_BLOBS];
	UpgradableSectors activeSector;

	SoundManager* _snd;
	Vector2d* const _shipPosition;

	void addPoint();
public:
	UpgradeManager(DrawingAssistant* drw, WeaponManager* wpn, SpaceShip* ship, UpgradableShield* shield, SoundManager* snd);
	~UpgradeManager();
	void draw();
	void move();
	bool hitTest(float x, float y);
	void spawn(float x, float y);
	UpgradeManager::Upgradable* getActiveSector();
	void switchUpgrade();
};

UpgradeManager::UpgradeManager(DrawingAssistant *drw, WeaponManager* wpn, SpaceShip* ship, UpgradableShield* shield, SoundManager* snd)
: IDrawable(drw), _shipPosition(&ship->shipPos)
{
	uSectors[US_WEAPONS].p			= (IUpgradable*)wpn;
	uSectors[US_WEAPONS].texture	= DrawingAssistant::T_UP_WEAPONS;
	uSectors[US_SHIELD].p			= (IUpgradable*)shield;
	uSectors[US_SHIELD].texture		= DrawingAssistant::T_UP_SHIELD;
	uSectors[US_MOVEMENT].p			= (IUpgradable*)ship;
	uSectors[US_MOVEMENT].texture	= DrawingAssistant::T_UP_MOVE;
	_snd = snd;

	activeSector = US_WEAPONS;

	for(int i=0; i<MAX_UPGRADABLES; i++)
	{
		uSectors[i].level	= 1;
		uSectors[i].points	= 0;
		uSectors[i].limit	= 10;
		uSectors[i].p->setLevel(1);
	}

	for(int i=0; i<MAX_BLOBS; i++)
	{
		blobs[i] = new Upgrade(drw);
	}
}

void UpgradeManager::draw()
{
	for(int i=0; i<MAX_BLOBS; i++)
	{
		if(blobs[i]->_alive)
			blobs[i]->draw();
	}
}

void UpgradeManager::move()
{
	for(int i=0; i<MAX_BLOBS; i++)
	{
		if(blobs[i]->_alive)
			blobs[i]->move();
	}
}

bool UpgradeManager::hitTest(float x, float y)
{
	for(int i=0; i<MAX_BLOBS; i++)
	{
		if(blobs[i]->_active)
		{
			Vector2d v(blobs[i]->_position._x-x, blobs[i]->_position._y-y);
			if(v.magnitude() < 0.15)
			{
				blobs[i]->pickUp();
				_snd->play(SoundManager::BLOB_PICKUP);
				addPoint();
				return true;
			}
		}
	}
	return false;
}

void UpgradeManager::spawn(float x, float y)
{
	for(int i=0; i<MAX_BLOBS; i++)
	{
		if(!blobs[i]->_alive)
		{
			blobs[i]->init(x, y);
			if(uSectors[US_MOVEMENT].level >= SUPER_LEVEL)
				blobs[i]->setTarget(_shipPosition);
			return;
		}
	}
}

void UpgradeManager::addPoint()
{
	uSectors[activeSector].points++;

	if(uSectors[activeSector].points >= uSectors[activeSector].limit)
	{
		uSectors[activeSector].level++;
		uSectors[activeSector].p->setLevel(uSectors[activeSector].level);
		uSectors[activeSector].points = 0;
		uSectors[activeSector].limit = uSectors[activeSector].limit * (UPGRADE_LEVEL_GROWTH_PERCENTAGE/100.0);
	}
}

UpgradeManager::Upgradable* UpgradeManager::getActiveSector()
{
	return &uSectors[activeSector];
}

void UpgradeManager::switchUpgrade()
{
	activeSector = (UpgradableSectors)((int)activeSector+1);
	if(activeSector >= MAX_UPGRADABLES) activeSector = (UpgradableSectors)0;
}