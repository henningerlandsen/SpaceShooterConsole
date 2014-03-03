#ifndef FLOATING_OBJECT_H
#define FLOATING_OBJECT_H
#include "Vector2d.h"
#include <iostream>
using namespace std;

// By Henning
// Calculates movement in a non-solid enviroment.

class FloatingObject
{
public:
	Vector2d g;	//Gravity
	float f;	//Friction
	float a;	//Acceleration
	float ts;	//Top speed
	Vector2d dir;	//Direction vector
	Vector2d F;	//Force
	Vector2d* pos; //The position vector to manipulate

	FloatingObject(Vector2d &v);
	void setDirection(Vector2d v);
	void accelerate();
	void move();
};

FloatingObject::FloatingObject(Vector2d &v)
{
	pos = &v;
	dir = Vector2d(0,0);
	F = Vector2d(0,0);
	g = Vector2d(0, 0.05);
	f	= 0.9;
	a 	= 2;
	ts 	= 20;
}

void FloatingObject::setDirection(Vector2d v)
{
	dir = v;
}

void FloatingObject::accelerate()
{
	F	= F.add(g);						//Invoke gravity
	F	= F.multiply(Vector2d(f, f));	//Add friction
	dir = dir.normalize();				//Unify direction
	Vector2d F2 = dir.multiply(Vector2d(a,a));	//Calculate accelerating force
	F	= F.add(F2);					//Add acceleration to current force
	if(F.magnitude() > ts)				//If top speed is exeeded
	{
		F = F.normalize().multiply(Vector2d(ts, ts));	//Unify force and set to top speed
	}
}

void FloatingObject::move()
{
	(*pos)._x += F._x;
	(*pos)._y += F._y;
}

#endif //FLOATING_OBJECT_H