#pragma once
#include "Weapon.h"

class Weapon1 : public Weapon
{
	float _pHeight;
	float _pWidth;

	void resetProjectile(Projectile &p);	// Resets a projectile to initial state.
public:
	Weapon1(DrawingAssistant *drw, Vector2d* position, bool *fireControl);
	~Weapon1();
	void draw();
};

Weapon1::Weapon1(DrawingAssistant *drw, Vector2d *position, bool *fireControl)
: Weapon(drw, position, fireControl)
{
	_energySurge	= 25;
	_pHeight		= 0.07;
	_pWidth			= 0.02;

	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		resetProjectile(_projectiles[i]);
	}
}

Weapon1::~Weapon1()
{}

// -------------------------------- Constructor & Destructor //

// Private member-functions -------------------------------- //

void Weapon1::resetProjectile(Weapon::Projectile &p)
{
	p.active = false;
	p.dir = Vector2d(0, 1);
	p.pos = Vector2d();
	p.speed = 0.02;
}

// -------------------------------- Private member-functions //

// Public member-functions --------------------------------- //

void Weapon1::draw()
{
	_drw->setTexture(DrawingAssistant::T_PARTICLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthFunc(GL_ALWAYS);

	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		if(_projectiles[i].active)
		{
			glColor4f(0.0, 0.7, 1.0, 1.0);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y, _pWidth, _pHeight);
			glColor4f(0.8, 0.8, 1.0, 1.0);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y, _pWidth, _pWidth);
		}
	}

	glDepthFunc(GL_LEQUAL);
}

// --------------------------------- Public member-functions //
