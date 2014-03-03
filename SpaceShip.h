#pragma once

#include "IDrawable.h"
#include "IUpgradable.h"
#include "Vector2d.h"
#include "Enemy.h"
#include "FloatingObject.h"
#include "SpecialFXManager.h"

#define AFTERBURNER_LENGTH 30

/////////////////////////////////////////////////////////////////////////////////////

class SpaceShip : public IDrawable, public IUpgradable
{
	bool _alive;
	float trueEnergyLimit;
	float energyAnimationCounter;
	float energyAnimationStop;
	float energyStartLevel;
	float energyDifference;
	float shieldAlpha;
	float shieldFade;

	SpecialFXManager* sfx;

public:
	struct Particle
	{
		float x;
		float y;
		float life;
	};

	Particle		afterburn[AFTERBURNER_LENGTH];
	Vector2d		shipPos;
	FloatingObject*	_movementControl;

	float energyLevel;
	float maxEnergyLevel;
	float energyRefillRate;
	float shieldStrength;

	SpaceShip(DrawingAssistant* drw);
	~SpaceShip();
	void draw();
	void move();
	float* const getEnergyPointer();
	float* const getEnergyLimitPointer();
	bool reduceEnergy(float value);
	bool moveEnergyLimit(float value);
	bool hitTest(Enemy* e);
	void setLevel(int level);
};

//////////////////////////////////////////////////////////////////////////////////////////////

#include "TweenControl.h"
#include <iostream>
#include <cmath>
using namespace std;

SpaceShip::SpaceShip(DrawingAssistant* drw)
	: IDrawable(drw)
{
	_alive			= true;

	_movementControl	= new FloatingObject(shipPos);
	_movementControl->a	= 0.0005;		// Acceleration
	_movementControl->ts= 0.02;			// Top Speed;
	_movementControl->f	= 0.995;		// Friction (Inverted 1 = No friction, 0 = Full friction);
	_movementControl->g	= Vector2d();	// Gravity vector;

	sfx				= new SpecialFXManager(_drw);
	energyLevel		= 100.0;
	maxEnergyLevel	= 100.0;
	trueEnergyLimit = 100.0;
	energyRefillRate= 0.5;
	shieldAlpha		= 0;
	shieldFade		= 0.04;
	shieldStrength	= 1.0;

	for(int i=0; i<AFTERBURNER_LENGTH; i++)
	{
		afterburn[i].life = (1.0/AFTERBURNER_LENGTH) * (i+1);
		afterburn[i].x = 0;
		afterburn[i].y = 0;
	}
}

void SpaceShip::draw()
{

	if(_alive)
	{
		//Draw Ship ----------------------------------------------------------------- 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		_drw->setTexture(DrawingAssistant::T_SPACESHIP);
		_drw->drawTextured2dQuadCenter(shipPos._x, shipPos._y, 0.2, 0.2);
		// ---------------------------------------------------------------- Draw Ship*/

		//Draw Afterburner ----------------------------------------------------------
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_TEXTURE_2D);

		for(int i=0; i<AFTERBURNER_LENGTH; i++)
		{
			if(afterburn[i].life > 0)
			{
				float size = pow(afterburn[i].life, 3) * 0.07;

				glColor4f(.7f, 0.7f, .7f, pow(afterburn[i].life, 3));

				_drw->setTexture(DrawingAssistant::T_ROCKETPARTICLE);
				_drw->drawTextured2dQuadCenter(afterburn[i].x, afterburn[i].y, size, size);
			}
		}
		//---------------------------------------------------------- Draw Afterburner 

		if(shieldAlpha > 0)
		{
			TweenControl t;
			float w = t.tween(0.2, 0.3, 2, 1-shieldAlpha);
			_drw->setTexture(DrawingAssistant::T_XPL_RING);
			glColor4f(0.0 , 1.0, 1.0, shieldAlpha);
			_drw->drawTextured2dQuadCenter(shipPos._x, shipPos._y, w, w);
		}
	}
	sfx->draw();
}

void SpaceShip::move()
{
	// Energy meter ----------------------------------------------------------------
	if(energyLevel < trueEnergyLimit)
	{
		energyLevel += energyRefillRate;
	}
	
	if(maxEnergyLevel != trueEnergyLimit) //Skal energimåleren flyttes?
	{
		energyAnimationCounter++;
		float ratio = energyAnimationCounter / energyAnimationStop;
		float currentDistance;
		if(ratio < .5)
		{
			currentDistance = energyStartLevel + pow(2*ratio,2)/2 * energyDifference;
		}
		else
		{
			float downRatio = 1-ratio;
			currentDistance = energyStartLevel + (1-pow(2*downRatio,2)/2) * energyDifference;
		}
		maxEnergyLevel = currentDistance;
	}

	if(energyLevel > maxEnergyLevel) energyLevel = trueEnergyLimit;

	// ---------------------------------------------------------------- Energy meter 

	// Afterburner -----------------------------------------------------------------
	for(int i=0; i<AFTERBURNER_LENGTH; i++)
	{
		afterburn[i].life -= 0.05;
		if(afterburn[i].life > 0)
		{
			float ratio = pow(afterburn[i].life, 2);
			afterburn[i].y -= ratio * 0.03;
		}
		else
		{
			afterburn[i].life = 1.0;
			afterburn[i].x = shipPos._x;
			afterburn[i].y = shipPos._y-0.19;
		}
	}
	// ----------------------------------------------------------------- Afterburner

	// Shield ----------------------------------------------------------------------

	if(shieldAlpha > 0)
	{
		shieldAlpha -= shieldFade;
	}

	// ---------------------------------------------------------------------- Shield

	sfx->move();
}

float* const SpaceShip::getEnergyPointer()
{
	return &energyLevel;
}

float* const SpaceShip::getEnergyLimitPointer()
{
	return &maxEnergyLevel;
}

bool SpaceShip::reduceEnergy(float value)
{
	if(energyLevel-value >= 0.0)
	{
		energyLevel -= value;
		return true;
	}
	else
	{
		return false;
	}
}

bool SpaceShip::hitTest(Enemy *e)
{
	if(_alive)
	{
		Vector2d v = e->_position.subtract(shipPos);
		v = v.normalize().multiply(Vector2d(0.2, 0.2));
		v = shipPos.add(v);
		return e->hitTest(v._x, v._y);
	}else return false;
}

bool SpaceShip::moveEnergyLimit(float value)
{
	if(value < 0) value *= shieldStrength;

	shieldAlpha = 1.0;
	if(trueEnergyLimit + value > 0)
	{
		trueEnergyLimit		   += value;
		_alive = true;
	}
	else if(_alive)
	{
		trueEnergyLimit = 0;
		sfx->ringOfGreen(shipPos._x, shipPos._y);
		_alive = false;
	}

	energyStartLevel		= maxEnergyLevel;
	energyAnimationCounter	= 0;
	energyAnimationStop		= 100;
	energyDifference		= trueEnergyLimit - maxEnergyLevel;
	return true;
}

void SpaceShip::setLevel(int level)
{
	switch(level)
	{
	case 1:
		_movementControl->a	= 0.0005;		// Acceleration
		_movementControl->ts= 0.02;			// Top Speed;
		break;
	case 2:
		_movementControl->a	= 0.001;		// Acceleration
		_movementControl->ts= 0.025;		// Top Speed;
		break;
	case 3:
		_movementControl->a	= 0.002;		// Acceleration
		_movementControl->ts= 0.03;		// Top Speed;
		_movementControl->f	= 0.99;
		break;
	case 4:
		_movementControl->a	= 0.003;		// Acceleration
		_movementControl->ts= 0.035;		// Top Speed;
		_movementControl->f	= 0.98;
		break;
	case 5:
		_movementControl->a	= 0.004;		// Acceleration
		_movementControl->ts= 0.04;			// Top Speed;
		_movementControl->f	= 0.95;
		break;
	}
}

SpaceShip::~SpaceShip()
{
	delete sfx;
	delete _movementControl;
}

