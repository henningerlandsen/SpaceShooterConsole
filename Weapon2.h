#pragma once
#include "Weapon.h"

class Weapon2 : public Weapon
{
	float _pHeight;
	float _pWidth;

	void resetProjectile(Projectile &p);	// Resets a projectile to initial state.
public:
	Weapon2(DrawingAssistant *drw, Vector2d* position, bool *fireControl);
	~Weapon2();
	void draw();
};

Weapon2::Weapon2(DrawingAssistant *drw, Vector2d *position, bool *fireControl)
: Weapon(drw, position, fireControl)
{
	_energySurge	= 20;
	_pHeight		= 0.05;
	_pWidth			= 0.05;

	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		resetProjectile(_projectiles[i]);
	}
}

Weapon2::~Weapon2()
{}

// -------------------------------- Constructor & Destructor //

// Private member-functions -------------------------------- //

void Weapon2::resetProjectile(Weapon::Projectile &p)
{
	p.active = false;
	p.dir = Vector2d(0, 1);
	p.pos = Vector2d();
	p.speed = 0.04;
}

// -------------------------------- Private member-functions //

// Public member-functions --------------------------------- //

void Weapon2::draw()
{
	_drw->setTexture(DrawingAssistant::T_PARTICLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthFunc(GL_ALWAYS);

	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		if(_projectiles[i].active)
		{
			glColor3f(1.0, 0.0, 0.2);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y, _pWidth, _pHeight);
			glColor3f(1.0, 0.5, 0.5);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y, _pWidth/2, _pHeight/2);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y, _pWidth/3, _pHeight/3);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y-_pHeight/2, _pWidth/2, _pHeight/2);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y-_pHeight, _pWidth/3, _pHeight/3);
		}
	}

	glDepthFunc(GL_LEQUAL);
}

// --------------------------------- Public member-functions //
