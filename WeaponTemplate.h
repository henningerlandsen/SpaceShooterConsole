#pragma once
#include "Weapon.h"

class Weapon0 : public Weapon
{
public:
	Weapon0(DrawingAssistant *drw, Vector2d* position);
	~Weapon0();
	void draw();		//Run at max FPS.
	void move();		//Run at constant FPS.
	bool hitTest();		//Check for collision.
	void fireStart();	//When player presses fire-key.
	void fireStop();	//When player releases fire-key.
};

Weapon0::Weapon0(DrawingAssistant *drw, Vector2d *position)
: Weapon(drw, position)
{

}

Weapon0::~Weapon0()
{}

// -------------------------------- Constructor & Destructor //

// Private member-functions -------------------------------- //

void Weapon0::resetProjectile(Weapon::Projectile &p)
{

}

// -------------------------------- Private member-functions //

// Public member-functions --------------------------------- //

void Weapon0::draw()
{}

void Weapon0::move()
{}

bool Weapon0::hitTest()
{}

void Weapon0::fireStart()
{}

void Weapon0::fireStop()
{}
// --------------------------------- Public member-functions //
