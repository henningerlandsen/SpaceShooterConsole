#pragma once

#include "IDrawable.h"
#include "IUpgradable.h"
#include "Weapon1.h"
#include "Weapon2.h"
#include "WpnTriple.h"
#include "WpnAutoFire.h"
#include "Vector2d.h"
#include "DrawingAssistant.h"
#include "Enemy.h"
#include "SpaceShip.h"

class WeaponManager : public IDrawable, public IUpgradable
{
	enum Weapons {	WPN_WEAPON1,
					WPN_WEAPON2,
					WPN_TRIPLE,
					WPN_BEAM,
					MAX_WEAPONS
					};

	Weapon*		_weapons[MAX_WEAPONS];
	SpaceShip*	_ship;
	int			_level;
	bool		_fireControl;

public:

	WeaponManager(DrawingAssistant* drw, SpaceShip* ship);
	~WeaponManager();
	/*** IDrawable ***/
	void draw();
	void move();
	/*** Member functions ***/
	bool fireStart();
	void fireStop();
	void setLevel(int level);
	bool hitTest(Enemy* const e);
};

#include <iostream>
using namespace std;

WeaponManager::WeaponManager(DrawingAssistant* drw, SpaceShip* ship)
:IDrawable(drw)
{
	_ship = ship;
	_fireControl = false;

	_weapons[WPN_WEAPON1]	= new Weapon1(drw, &ship->shipPos, &_fireControl);
	_weapons[WPN_WEAPON2]	= new Weapon2(drw, &ship->shipPos, &_fireControl);
	_weapons[WPN_TRIPLE]	= new WpnTriple(drw, &ship->shipPos, &_fireControl);
	_weapons[WPN_BEAM]		= new WpnAutoFire(drw, &ship->shipPos, &_fireControl);
}

WeaponManager::~WeaponManager(){}


void WeaponManager::draw()
{
	for(int i=0; i<MAX_WEAPONS; i++)
	{
		if(_weapons[i]->_active)
			_weapons[i]->draw();
	}
}

void WeaponManager::move()
{
	for(int i=0; i<MAX_WEAPONS; i++)
	{
		if(_weapons[i]->_active)
			_weapons[i]->move();
	}

}

bool WeaponManager::hitTest(Enemy* const e)
{
	for(int i=0; i<MAX_WEAPONS; i++)
	{
		if(_weapons[i]->_active && _weapons[i]->hitTest(e)) return true;
	}
	return false;
}

void WeaponManager::setLevel(int level)
{
	for(int i=0; i<MAX_WEAPONS && level<=MAX_WEAPONS; i++)
	{
		_weapons[i]->_enabled = (i==level-1);
		if(_weapons[i]->_enabled)
		cout << "Weapon " << i << " enabled" << endl;
	}
}

bool WeaponManager::fireStart()
{
	bool hasFired = false;
	_fireControl = true;
	for(int i=0; i<MAX_WEAPONS; i++)
	{
		if(_weapons[i]->_enabled && _weapons[i]->_readyToFire && _ship->reduceEnergy(_weapons[i]->_energySurge))
		{
			cout << "Fireing weapon " << i << endl;
			_weapons[i]->_active = true;
			_weapons[i]->deployProjectile();
			hasFired = true;
		}
	}
	return hasFired;
}

void WeaponManager::fireStop()
{
	_fireControl = false;
}
