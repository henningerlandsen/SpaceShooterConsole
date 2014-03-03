#pragma once
#include "IDrawable.h"
#include "Vector2d.h"
#include "FloatingObject.h"
#define PI 3.14159265

class Upgrade : public IDrawable
{
//	float _speed;
	float _rad;
	float _mRad;
	float _mPathRad;
	int _iterator;
	FloatingObject* _movementControl;
public:
	bool _active;
	bool _alive;
	Vector2d _position;
	Vector2d* _target;
	bool _lockOnTarget;
//	float _x;
//	float _y;
	Upgrade(DrawingAssistant* drw);
	~Upgrade();
	void draw();
	void move();
	void init(float _x, float _y);
	void pickUp();
	void setTarget(Vector2d* target);
};

Upgrade::Upgrade(DrawingAssistant *drw)
: IDrawable(drw)
{
	_movementControl		= new FloatingObject(_position);
	_movementControl->a		= 0.001;
	_movementControl->g		= Vector2d(0,-0.01);
	_movementControl->f		= 0.5;
	_movementControl->ts	= 0.02;
	_movementControl->setDirection(Vector2d(0,0));
	
	_lockOnTarget = false;
	_iterator	= 0;
//	_speed		= 0.01;
	_rad		= 0.1;
	_mRad		= 0.075;
	_mPathRad	= 0.075;
	_active		= false;
	_alive		= false;
}

void Upgrade::draw()
{
	glDepthFunc(GL_ALWAYS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	_drw->setTexture(DrawingAssistant::T_PARTICLE);
	glColor4f(0.01, 0.3, 1.0, 1.0);
	glTranslatef(_position._x, _position._y, 0.0);
	glRotatef(_iterator, 0,0,1);

	//Main Sphere
	_drw->drawTextured2dQuadCenter(0, 0, _rad, _rad);
	_drw->drawTextured2dQuadCenter(0,0,_mRad, _mRad);

	//Right Moon
	_drw->drawTextured2dQuadCenter(sin((float)_iterator/180*PI*1)*_mPathRad,0,cos((float)_iterator/180*PI*1)*_mPathRad,_mRad, _mRad);

	//Left Moon
	_drw->drawTextured2dQuadCenter(sin((float)_iterator/180*PI*1.5)*_mPathRad,0,-cos((float)_iterator/180*PI*1.5)*_mPathRad,_mRad, _mRad);

	//Top Moon
	_drw->drawTextured2dQuadCenter(0, sin((float)_iterator/180*PI*2)*_mPathRad,cos((float)_iterator/180*PI*2)*_mPathRad,_mRad, _mRad);

	//Bottom Moon
	_drw->drawTextured2dQuadCenter(0, -sin((float)_iterator/180*PI*2.5)*_mPathRad,-cos((float)_iterator/180*PI*2.5)*_mPathRad,_mRad, _mRad);

	glRotatef(-_iterator, 0,0,1);
	glTranslatef(-_position._x, -_position._y, 0.0);
}

void Upgrade::move()
{
	_iterator += 2;

	if(_lockOnTarget)
		_movementControl->setDirection(_target->subtract(_position));
	_movementControl->accelerate();
	_movementControl->move();
	
	if(!_active && _alive)
	{
		_rad		-= 0.002;
		_mRad		-= 0.0015;
		_mPathRad	+= 0.0015;
		if(_rad <= 0)
		{
			_alive = false;
		}
	}
	if(_position._y < -3)
	{
		_active = false;
		_alive	= false;
	}
}

void Upgrade::init(float x, float y)
{
	_active		= true;
	_alive		= true;
	_position._x= x;
	_position._y= y;
	_rad		= 0.1;
	_mRad		= 0.075;
	_mPathRad	= 0.075;
	_movementControl->F = Vector2d();
}

void Upgrade::pickUp()
{
	_active = false;
}

void Upgrade::setTarget(Vector2d* target)
{
	_target = target;
	_lockOnTarget = true;
	_movementControl->g = Vector2d(0,0);
	_movementControl->f = 0.995;
}