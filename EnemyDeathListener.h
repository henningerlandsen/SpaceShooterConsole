#pragma once
#include "AbstractListener.h"
#include "UpgradeManager.h"

class EnemyDeathListener : public AbstractListener
{
private:
	UpgradeManager* _um;
public:
	EnemyDeathListener(UpgradeManager* um);
	~EnemyDeathListener();
	void trigger(float x, float y);
};

EnemyDeathListener::EnemyDeathListener(UpgradeManager* um)
:_um(um)
{}

void EnemyDeathListener::trigger(float x, float y)
{
	_um->spawn(x, y);
}