#pragma once

#include "IDrawable.h"
#include "Weapon1.h"
#include "Vector2d.h"
#include "DrawingAssistant.h"
#include "Enemy.h"
#include "SpaceShip.h"

#define MAX_MISSILES 10

class WeaponManager : public IDrawable
{
	enum MissileType { INACTIVE, LASER1, LASER2, ENDVAL };
	MissileType activeWeapon;
	GLuint texID;
	int fireTimer;
	float* energyPtr;
	DrawingAssistant* drw;
	
	struct Missile
	{
		float x;
		float y;
		int energySurge;	//Ammount of energy required to launch missile
		MissileType type;
	};

/*	Switches on id and returns a Missile of correct type */
	void setMissileType(MissileType type, Missile &m);

public:
	Missile missiles[MAX_MISSILES];

/*  Initializes missiles[] to INACTIVE missiles.*/
	WeaponManager(DrawingAssistant* drawingAssistantPtr, float* energyPointer);
	~WeaponManager();

/*	Sets activeMissile to next MissileType */
	void upgrade();

/*	Activates missile */
	bool launch(SpaceShip* const ship);

/*	Loops trough missiles[] and tests against enemy position. */
	bool hitTest(Enemy* const e);

/*	Loops trough missiles[] and switches on each MissileType.
	case INACTIVE/default: break;
	case !INACTIVE: draw corresponding weapon */
	void draw();
/*	Time-based operations */
	void move();
};

#include <iostream>
using namespace std;

WeaponManager::WeaponManager(DrawingAssistant* drawingAssistantPtr, float* energyPointer)
:IDrawable(drawingAssistantPtr)
{
	activeWeapon	= LASER1;
	drw				= drawingAssistantPtr;
	energyPtr		= energyPointer;

	for(int i=0; i<MAX_MISSILES; i++)
	{
		missiles[i].type = INACTIVE;
	}
}

WeaponManager::~WeaponManager(){}

void WeaponManager::setMissileType(MissileType type, Missile &m)
{
	m.type = type;
	switch(type)
	{
	case INACTIVE:
		break;
	case LASER1:
		m.energySurge = 25;
		break;
	case LASER2:
		m.energySurge = 20;
		break;
/*	case LASER3:
		m.energySurge = 5;
		break;*/

	}
}

bool WeaponManager::launch(SpaceShip* const ship)
{
	Missile temp;
	setMissileType(activeWeapon, temp);
	if(ship->reduceEnergy(temp.energySurge))
	{
		for(int i=0; i<MAX_MISSILES; i++)
		{
			if(missiles[i].type == INACTIVE)
			{
				setMissileType(activeWeapon, missiles[i]);
				missiles[i].x = ship->shipPos._x;
				missiles[i].y = ship->shipPos._y + 0.2;
				return true;
			}
		}
		return false;
	}
	else return false;
}

bool WeaponManager::hitTest(Enemy* const e)
{
	for(int i=0; i<MAX_MISSILES; i++)
	{
		if(missiles[i].type != INACTIVE && e->hitTest(missiles[i].x, missiles[i].y))
		{
			missiles[i].type = INACTIVE;
			return true;
		}
	}
	return false;
}

void WeaponManager::upgrade()
{
	if(activeWeapon +1 != ENDVAL)
	{
		activeWeapon = (MissileType)((int)activeWeapon + 1);
	}
}

void WeaponManager::draw()
{
	float xsize;
	float ysize;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);

	for(int i=0; i<MAX_MISSILES; i++)
	{		
		//Kill missile if off screen
		if(missiles[i].y > 2.8)
		{
			setMissileType(INACTIVE, missiles[i]);
			continue;
		}
		
		//Draw the active missiles
		switch(missiles[i].type)
		{
		case INACTIVE: 
			break;
		
		case LASER1:
			xsize			= 0.02;
			ysize			= 0.07;
			
			_drw->setTexture(DrawingAssistant::T_PARTICLE);
			glColor4f(0.0, 0.7, 1.0, 1.0);
			_drw->drawTextured2dQuadCenter(missiles[i].x, missiles[i].y, xsize, ysize);
			glColor4f(0.8, 0.8, 1.0, 1.0);
			_drw->drawTextured2dQuadCenter(missiles[i].x, missiles[i].y, xsize, xsize);
			break;
		
		case LASER2:
			xsize	= 0.05;
			ysize	= 0.05;

			_drw->setTexture(DrawingAssistant::T_PARTICLE);
			glColor3f(1.0, 0.0, 0.2);
			_drw->drawTextured2dQuadCenter(missiles[i].x, missiles[i].y, xsize, ysize);
			glColor3f(1.0, 0.5, 0.5);
			_drw->drawTextured2dQuadCenter(missiles[i].x, missiles[i].y, xsize/2, ysize/2);
			glColor3f(1.0, 0.5, 0.5);
			_drw->drawTextured2dQuadCenter(missiles[i].x, missiles[i].y, xsize/3, ysize/3);
			glColor3f(1.0, 0.5, 0.5);
			_drw->drawTextured2dQuadCenter(missiles[i].x, missiles[i].y-ysize/2, xsize/2, ysize/2);
			glColor3f(1.0, 0.5, 0.5);
			_drw->drawTextured2dQuadCenter(missiles[i].x, missiles[i].y-ysize, xsize/3, ysize/3);
			break;

		default:
			break;
		}
	}
}

void WeaponManager::move()
{
	for(int i=0; i<MAX_MISSILES; i++)
	{		
		
		//Draw the active missiles
		switch(missiles[i].type)
		{
		case INACTIVE: 
			break;
		
		case LASER1:
			missiles[i].y  += 0.02;
			break;
		case LASER2:
			missiles[i].y  += 0.04;

		default:
			break;
		}
	}
}