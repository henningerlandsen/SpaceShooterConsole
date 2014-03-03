#pragma once

#include "Enemy.h"
#include "EnemySaw.h"
#include "EnemyMine.h"
#include "IDrawable.h"
#include "Vector2d.h"
#include "WeaponManager.h"
#include "SpaceShip.h"
#include "SpecialFXManager.h"
#include "SoundManager.h"
#include "UpgradeManager.h"
#include "EnemyDeathListener.h"

#define MAX_ENEMIES 40

class EnemyManager : public IDrawable
{
private:
	Enemy* enemies[MAX_ENEMIES];
	SpecialFXManager* sfx;
	SoundManager* snd;
	UpgradeManager* upg;
	const Vector2d* _shipPosition;

	int frameCount;
	int waveInterval;
public:
	typedef enum EnemyType {
		SAW,
		MINE,
		BOSS };

	typedef struct Deployment {
		Vector2d	position;
		Vector2d	force;
		EnemyType	type;
	};


public:
	EnemyManager(const Vector2d &shipPosition, DrawingAssistant* drw, SoundManager* sound, UpgradeManager* upgrade);
	~EnemyManager();
	void draw();
	void move();
	void hitTest(WeaponManager* wm);
	void hitTest(SpaceShip* ship);
	void hitTest();
	void deploy(Deployment const &deployment);
};

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

EnemyManager::EnemyManager(const Vector2d &shipPosition, DrawingAssistant* drw, SoundManager* sound, UpgradeManager* upgrade)
	: IDrawable(drw)
{
	sfx = new SpecialFXManager(drw);
	snd = sound;
	upg = upgrade;
	_shipPosition = &shipPosition;
	srand(time(0));

	frameCount		= 0;
	waveInterval	= 500;

	for(int i=0; i<MAX_ENEMIES; i++)
	{
		if(i<5)	enemies[i]		= new EnemySaw(shipPosition, drw, sfx);
		else enemies[i]			= new EnemyMine(drw, sfx);
		enemies[i]->_rotation	= (float)rand()/RAND_MAX*50;
	}
}

EnemyManager::~EnemyManager()
{
	for(int i=0; i<MAX_ENEMIES; i++)
	{
		delete enemies[i];
	}
}

void EnemyManager::draw()
{
	for(int i=0; i<MAX_ENEMIES; i++)
	{
		if(enemies[i]->_active)	enemies[i]->draw();
	}
	sfx->draw();
}

void EnemyManager::move()
{
	for(int i=0; i<MAX_ENEMIES; i++)
	{
		if(enemies[i]->_active)
		{
			enemies[i]->move();
			if(enemies[i]->_position._y < -3)
			{
				enemies[i]->_active = false;
			}
		}
	}
	sfx->move();

}

void EnemyManager::hitTest(WeaponManager* wm)
{
	for(int i=0; i<MAX_ENEMIES; i++)
	{
		if(enemies[i]->_active)
		{
			if(wm->hitTest(enemies[i]))
			{
				enemies[i]->kill();
				snd->play(SoundManager::SAW_HIT);
				//upg->spawn(enemies[i]->_position._x, enemies[i]->_position._y);
			}
		}
	}
}

void EnemyManager::hitTest(SpaceShip *ship)
{
	for(int i=0; i<MAX_ENEMIES; i++)
	{
		if(enemies[i]->_active)
		{
			if(ship->hitTest(enemies[i]))
			{
				Vector2d v = ship->shipPos.subtract(enemies[i]->_position);
				v = v.normalize();
				float f = ship->_movementControl->F.subtract(enemies[i]->_movementControl->F).magnitude() * 1.1;
				v = v.multiply(Vector2d(f, f));
				enemies[i]->_movementControl->F = enemies[i]->_movementControl->F.subtract(v);
				ship->moveEnergyLimit(-25);
			}
		}
	}
}

void EnemyManager::hitTest()
{
	for(int i=0; i<MAX_ENEMIES; i++)
	{
		for(int j=0; j<MAX_ENEMIES; j++)
		{
			if(i!=j)
				enemies[i]->hitTest(enemies[j]);
		}
	}
}

void EnemyManager::deploy(const EnemyManager::Deployment &deployment)
{
//	cout << "Deploying" << endl;
	for(int i=0; i<MAX_ENEMIES; i++)
	{
		if(!enemies[i]->_active)
		{
			delete enemies[i];			//Remove inactive enemy from memory
			switch(deployment.type)		//Create new enemy
			{
			case SAW:
				enemies[i] = new EnemySaw(*_shipPosition, _drw, sfx/*, upg*/);
				break;
			case MINE:
				enemies[i] = new EnemyMine(_drw, sfx/*, upg*/);
				break;
			default:
				enemies[i] = new EnemySaw(*_shipPosition, _drw, sfx/*, upg*/);
				break;
			}							//Set initial movement
			enemies[i]->_position = deployment.position;
			enemies[i]->_movementControl->F = deployment.force;
			enemies[i]->_active = true;
			enemies[i]->addListener((AbstractListener*)new EnemyDeathListener(upg));
			return;
		}
	}
}