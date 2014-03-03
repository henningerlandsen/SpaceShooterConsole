#pragma once
#include "Weapon.h"

class WpnAutoFire : public Weapon
{
	float	_pHeight;
	float	_pWidth;
	int		_counter;

	void resetProjectile(Projectile &p);	// Resets a projectile to initial state.
public:
	WpnAutoFire(DrawingAssistant *drw, Vector2d* position, bool *fireControl);
	~WpnAutoFire();
	void draw();
	void move();
	void deployProjectile();
};

WpnAutoFire::WpnAutoFire(DrawingAssistant *drw, Vector2d *position, bool *fireControl)
: Weapon(drw, position, fireControl)
{
	_energySurge	= 5;
	_pHeight		= 0.05;
	_pWidth			= 0.05;
	_autoFire		= true;
	_refireRate		= 5;
	_counter		= 0;

	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		resetProjectile(_projectiles[i]);
	}
}

WpnAutoFire::~WpnAutoFire()
{}

// -------------------------------- Constructor & Destructor //

// Private member-functions -------------------------------- //

void WpnAutoFire::resetProjectile(Weapon::Projectile &p)
{
	p.active = false;
	p.dir = Vector2d(0, 1);
	p.pos = Vector2d();
	p.speed = 0.07;
}

// -------------------------------- Private member-functions //

// Public member-functions --------------------------------- //

void WpnAutoFire::draw()
{
	_drw->setTexture(DrawingAssistant::T_PARTICLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthFunc(GL_ALWAYS);

	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		if(_projectiles[i].active)
		{
			for(int j=0; j<10; j++)
			{
				float factor = j/10.0;
				glColor4f(0.5, 0.7, 1.0, 1.0-factor);
				_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y - factor*0.3, _pWidth*(1-factor), _pHeight*(1-factor));
			
			}
			/*glColor4f(0.0, 0.7, 1.0, 1.0);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y, _pWidth, _pHeight);
			glColor4f(0.0, 0.7, 1.0, .8);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y-0.03, _pWidth/1.2, _pHeight/1.2);
			glColor4f(0.0, 0.7, 1.0, .6);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y-0.06, _pWidth/1.5, _pHeight/1.5);
			glColor4f(0.0, 0.7, 1.0, .4);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y-0.09, _pWidth/1.7, _pHeight/1.7);
			glColor4f(0.0, 0.7, 1.0, .2);
			_drw->drawTextured2dQuadCenter(_projectiles[i].pos._x, _projectiles[i].pos._y-0.11, _pWidth/2, _pHeight/2);*/
		}
	}

	glDepthFunc(GL_LEQUAL);
}

void WpnAutoFire::deployProjectile()
{
	_counter = 0;
}

void WpnAutoFire::move()
{
	cout << *_fireControl << endl;
	if(*_fireControl && _counter%_refireRate == 0)
	{
		_launchOffset = Vector2d(-0.05, 0.02);
		Weapon::deployProjectile();
		_launchOffset = Vector2d(0.05, 0.02);
		Weapon::deployProjectile();
	}
	_counter++;

	Weapon::move();
}

// --------------------------------- Public member-functions //
