#pragma once
#include "Vector2d.h"
#include "IDrawable.h"
#include "Enemy.h"

#define MAX_PROJECTILES 20

class Weapon : public IDrawable
{
public:

	struct Projectile
	{
		Vector2d	pos;
		Vector2d	dir;
		float		speed;
		bool		active;
	};

	Projectile	_projectiles[MAX_PROJECTILES];
	Vector2d*	_launchPosition;
	Vector2d	_launchOffset;

	float	_energySurge;
	int		_refireRate;
	bool	_autoFire;
	bool	_readyToFire;
	bool	_active;
	bool	_enabled;
	bool*	_fireControl;

private:

public:
	Weapon(DrawingAssistant *drw, Vector2d* position, bool* fireControl);
	~Weapon();
	virtual void draw();		//Run at max FPS.
	virtual void move();		//Run at constant FPS.
	virtual bool hitTest(Enemy* e);		//Check for collision.
	virtual void deployProjectile();
	virtual void resetProjectile(Projectile &p);	// Resets a projectile to initial state.
};

// Constructor & Destructor -------------------------------- //

Weapon::Weapon(DrawingAssistant *drw, Vector2d* position, bool* fireControl)
: IDrawable(drw)
{
	_launchPosition = position;

	_launchOffset	= Vector2d(0, 0.2);
	_active			= false;
	_enabled		= false;
	_fireControl	= fireControl;
	_readyToFire	= true;
}

Weapon::~Weapon()
{}

// -------------------------------- Constructor & Destructor //

// Private member-functions -------------------------------- //

void Weapon::resetProjectile(Weapon::Projectile &p)
{
	p.active = false;
	p.speed = 0;
}

// -------------------------------- Private member-functions //

// Public member-functions --------------------------------- //

void Weapon::draw()
{}

void Weapon::move()
{
	bool anyMissilesActive = false;

	for(int i=0; i<MAX_PROJECTILES; i++)
		if(_projectiles[i].active)
		{
			_projectiles[i].pos = _projectiles[i].pos.add(_projectiles[i].dir.multiply(Vector2d(_projectiles[i].speed, _projectiles[i].speed)));
			anyMissilesActive = true;
			if(_projectiles[i].pos._y > 3) _projectiles[i].active = false;
		}
	_active = anyMissilesActive;
}

bool Weapon::hitTest(Enemy* e)
{
	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		if(_projectiles[i].active && e->hitTest(_projectiles[i].pos._x, _projectiles[i].pos._y))
		{
			_projectiles[i].active = false;
			return true;
		}
	}
	return false;
}

void Weapon::deployProjectile()
{
	for(int i=0; i<MAX_PROJECTILES; i++)
	{
		cout << _projectiles[i].active;
		if(!_projectiles[i].active)
		{
			_projectiles[i].active = true;
			_projectiles[i].pos._x = _launchPosition->add(_launchOffset)._x;
			_projectiles[i].pos._y = _launchPosition->add(_launchOffset)._y;
			cout << _projectiles[i].pos._x << " - " << _projectiles[i].pos._y << endl;
			return;
		}
	}
}

// --------------------------------- Public member-functions //