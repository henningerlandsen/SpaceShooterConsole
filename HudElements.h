#pragma once

#include "IDrawable.h"
#include "Vector2d.h"
#include "DrawingAssistant.h"
#include "UpgradeManager.h"
#include <iostream>

#define ENERGY_DIVIDENT 100
#define UPGRADE_DIVIDENT 10

class HudElements : public IDrawable
{
	struct PosSize
	{
		float x;
		float y;
		float height;
		float width;
	};

	PosSize _energyMeterBottom;
	PosSize _energyMeterTop;
	PosSize _energyMeterTube;
	PosSize _energyMeter;
	PosSize _upgradePanel;
	PosSize _upgradeMeterTube;
	PosSize _upgradeTop;
	PosSize _upgradeOnscreen;
	PosSize _upgradeLights;
	PosSize _upgradeMeter;

	float* const _energyMax;
	float* const _energyLevel;

	DrawingAssistant*			_drw;
	UpgradeManager*				_upgrd;
	UpgradeManager::Upgradable*	_currentUpgrade;

	float _counter;
	float _increment;

public:
	HudElements(float* const shipEnergyLimit, float* const shipEnergyLevel,  DrawingAssistant* drw, UpgradeManager* upgrd);
	~HudElements();
	void draw();
	void move();
};

HudElements::HudElements(float* const shipEnergyLimit, float* const shipEnergyLevel, DrawingAssistant* drw, UpgradeManager* upgrd) 
: IDrawable(drw), _energyLevel(shipEnergyLevel), _energyMax(shipEnergyLimit)
{
	_counter		= 0;
	_increment		= 0.1;
	_drw			= drw;
	_upgrd			= upgrd;
	_currentUpgrade = _upgrd->getActiveSector();

	_energyMeterBottom.x		= -2.77;
	_energyMeterBottom.y		= -2.0;
	_energyMeterBottom.height	= 0.3;
	_energyMeterBottom.width	= 0.3;

	_energyMeterTop.x			= -2.77;
	_energyMeterTop.y			= 0;		// Algoritme: EnergymeterPos.y + (ShipEnergyLimit / Faktor)
	_energyMeterTop.height		= 0.3;
	_energyMeterTop.width		= 0.3;

	_energyMeterTube.x			= -2.545;
	_energyMeterTube.y			= _energyMeterBottom.y + _energyMeterBottom.height;
	_energyMeterTube.width		= 0.05;
	_energyMeterTube.height		= 0;		// Algoritme: ShipEnergyLimit / Faktor

	_energyMeter.x				= -2.545;
	_energyMeter.y				= _energyMeterTube.y;
	_energyMeter.width			= 0.05;
	_energyMeter.height			= 0;		// Algoritme: ShipEnergyLevel / Faktor

	_upgradePanel.x				= 2.47;
	_upgradePanel.y				= -1.75;
	_upgradePanel.width			= 0.3;
	_upgradePanel.height		= 0.3;

	_upgradeMeterTube.x			= 2.65;
	_upgradeMeterTube.y			= -1.476;
	_upgradeMeterTube.width		= _energyMeterTube.width;
	_upgradeMeterTube.height	= 1;

	_upgradeMeter.x				= 2.65;
	_upgradeMeter.y				= -1.476;
	_upgradeMeter.width			= _energyMeterTube.width;
	_upgradeMeter.height		= 0;

	_upgradeTop.x				= 2.47;
	_upgradeTop.y				= -0.475;//-0.55;
	_upgradeTop.width			= 0.3;
	_upgradeTop.height			= 0.3;

	_upgradeOnscreen.x			= 2.405;
	_upgradeOnscreen.y			= -1.75;
	_upgradeOnscreen.width		= 0.15;
	_upgradeOnscreen.height		= 0.15;

	_upgradeLights.x			= 2.678;
	_upgradeLights.y			= -1.915;
	_upgradeLights.width		= 0.07;
	_upgradeLights.height		= 0.42;

}

HudElements::~HudElements()
{

}

void HudElements::draw()
{
	_energyMeterTop.y		= _energyMeter.y + (*_energyMax / ENERGY_DIVIDENT);
	_energyMeterTube.height = *_energyMax / ENERGY_DIVIDENT;
	_energyMeter.height		= *_energyLevel / ENERGY_DIVIDENT;

	_upgradeMeterTube.height	= (float)_currentUpgrade->limit / UPGRADE_DIVIDENT;
	_upgradeMeter.height		= (float)_currentUpgrade->points / UPGRADE_DIVIDENT;
	_upgradeTop.y				= _upgradeMeterTube.y + _upgradeMeterTube.height;

	/*** Energy Meter Top Box ***/
	_drw->setTexture(DrawingAssistant::T_EM_UPPER);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor3f(1.0, 1.0, 1.0);
	_drw->drawTextured2dQuadBottomLeft(_energyMeterTop.x, _energyMeterTop.y, _energyMeterTop.width, _energyMeterTop.height);

	/*** Energy Meter Lower Box ***/
	_drw->setTexture(DrawingAssistant::T_EM_LOWER);
	_drw->drawTextured2dQuadBottomLeft(_energyMeterBottom.x, _energyMeterBottom.y, _energyMeterBottom.width, _energyMeterBottom.height);
	
	/*** Energy Bar (Lion bar) ***/
	_drw->setTexture(DrawingAssistant::T_EM_LASER);
	glColor4f(0.8, 0.8, 1.0, 0.75 + sin(_counter)/4);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	_drw->drawTextured2dQuadBottomLeft(_energyMeter.x, _energyMeter.y, _energyMeter.width, _energyMeter.height);

	/*** Tube ***/
	_drw->setTexture(DrawingAssistant::T_EM_TUBE);	
	glColor3f(1.0, 1.0, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_drw->drawTextured2dQuadBottomLeft(_energyMeterTube.x, _energyMeterTube.y, _energyMeterTube.width, _energyMeterTube.height);

	/*** Upgrade Panel ***/
	_drw->setTexture(DrawingAssistant::T_UPGRADE_PANEL);
	_drw->drawTextured2dQuadCenter(_upgradePanel.x, _upgradePanel.y, _upgradePanel.width, _upgradePanel.height);
	
	/*** Upgrade Meter ***/
	_drw->setTexture(DrawingAssistant::T_EM_LASER);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glColor4f(0.8, 1.0, 0.8, 0.75 + sin(_counter)/4);
	_drw->drawTextured2dQuadBottomLeft(_upgradeMeter.x, _upgradeMeter.y, _upgradeMeter.width, _upgradeMeter.height);

	/*** Tube ***/
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_drw->setTexture(DrawingAssistant::T_EM_TUBE);
	_drw->drawTextured2dQuadBottomLeft(_upgradeMeterTube.x, _upgradeMeterTube.y, _upgradeMeterTube.width, _upgradeMeterTube.height);
	
	/*** Top Box ***/
	_drw->setTexture(DrawingAssistant::T_UPGRADE_TOP);
	_drw->drawTextured2dQuadBottomLeft(_upgradeTop.x, _upgradeTop.y, _upgradeTop.width, _upgradeTop.height);
	
	/*** Upgrade Icon ***/
	_drw->setTexture(_currentUpgrade->texture);
	glColor4f(1.0, 1.0, 1.0, 0.6);
	_drw->drawTextured2dQuadCenter(_upgradeOnscreen.x, _upgradeOnscreen.y, _upgradeOnscreen.width, _upgradeOnscreen.height);

	/*** Lights ***/
	_drw->setTexture(DrawingAssistant::T_UP_ON);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	for(int i=0; i<5; i++)
	{
		if(i >= _currentUpgrade->level) _drw->setTexture(DrawingAssistant::T_UP_OFF);
		_drw->drawTextured2dQuadCenter(_upgradeLights.x, _upgradeLights.y+_upgradeLights.height/5*i, _upgradeLights.width, _upgradeLights.width);
	}

}

void HudElements::move()
{
	_counter += _increment;
	_currentUpgrade = _upgrd->getActiveSector();
}