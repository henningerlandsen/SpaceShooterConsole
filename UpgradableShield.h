#pragma once;
#include "IUpgradable.h"
#include "SpaceShip.h"

class UpgradableShield : public IUpgradable
{
	SpaceShip* _ship;
public:
	UpgradableShield(SpaceShip* ship);
	~UpgradableShield();
	void setLevel(int level);
};

UpgradableShield::UpgradableShield(SpaceShip* ship) : _ship(ship)
{}

UpgradableShield::~UpgradableShield()
{}

void UpgradableShield::setLevel(int level)
{
	_ship->shieldStrength = 1.0 - (level/10.0);
}
