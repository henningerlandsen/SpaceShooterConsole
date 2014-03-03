#pragma once
#include "Weapon.h"

class WpnTriple : public Weapon
{
	float _pHeight;
	float _pWidth;

	void resetProjectile(Projectile &p);	// Resets a projectile to initial state.
public:
	WpnTriple(DrawingAssistant *drw, Vector2d* position, bool *fireControl);
	~WpnTriple();
	void draw();
	void deployProjectile();
};

WpnTriple::WpnTriple(DrawingAssistant *drw, Vector2d *position, bool *fireControl)
: Weapon(drw, position, fireControl)
{
	_energySurge	= 20;
	_pHeight		= 0.06;
	_pWidth			= 0.06;

	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		resetProjectile(_projectiles[i]);
	}
}

WpnTriple::~WpnTriple()
{}

// -------------------------------- Constructor & Destructor //

// Private member-functions -------------------------------- //

void WpnTriple::resetProjectile(Weapon::Projectile &p)
{
	p.active = false;
	p.dir = Vector2d(0, 1);
	p.pos = Vector2d();
	p.speed = 0.04;
}

// -------------------------------- Private member-functions //

// Public member-functions --------------------------------- //

void WpnTriple::draw()
{
	_drw->setTexture(DrawingAssistant::T_PARTICLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthFunc(GL_ALWAYS);

	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		if(_projectiles[i].active)
		{
			glColor4f(0.3, 1.0, 0.0, 1.0);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y, _pWidth, _pHeight);
			glColor4f(1.0, 0.0, 0.0, 1.0);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y, _pWidth/2, _pHeight/2);
		}
	}

	glDepthFunc(GL_LEQUAL);
}

void WpnTriple::deployProjectile()
{
	int pnum = 0;
	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		Vector2d v;
		switch(pnum)
		{
		case 0:
			v = Vector2d(0, 1);
			break;
		case 1:
			v = Vector2d(0.2, 1);
			break;
		case 2:
			v = Vector2d(-0.2, 1);
			break;
		case 3:
			return;
			break;
		}

		if(!_projectiles[i].active)
		{
			_projectiles[i].active = true;
			_projectiles[i].pos._x = _launchPosition->add(_launchOffset)._x;
			_projectiles[i].pos._y = _launchPosition->add(_launchOffset)._y;
			_projectiles[i].dir = v;
			pnum++;
		}
	}
}


// --------------------------------- Public member-functions //
