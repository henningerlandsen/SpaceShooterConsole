#pragma once

#include "Enemy.h"
#include "FloatingObject.h"

class EnemySaw : public Enemy
{
	float _rotateSpeed;
	const Vector2d* _ptrTarget;
public:
	GLuint _sawTID;
	GLuint _pivTID;
	EnemySaw(const Vector2d &target, DrawingAssistant* drw, SpecialFXManager* sfx/*, UpgradeManager* upg*/);
	~EnemySaw();
	void draw();
	void move();
	bool hitTest(float x, float y);
	void reset(float x, float y);
	void kill();
};

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


EnemySaw::EnemySaw(const Vector2d &target, DrawingAssistant* drw, SpecialFXManager* sfx/*, UpgradeManager* upg*/)
		: Enemy (100.0f, 0.2f, 0.2f, drw, sfx/*, upg*/)
{
	_position._y	= 2.5;
	_rotateSpeed	= 2.0;
	_ptrTarget		= &target;
	_active			= false;

	_movementControl->a		= 0.00008;
	_movementControl->ts	= 0.04;
	_movementControl->f		= 0.995;
	_movementControl->g		= Vector2d(0.0, -0.00008);
}

void EnemySaw::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//Set state
	glEnable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0);

	_drw->setTexture(DrawingAssistant::T_NME_SAW_BLADE);
	glTranslatef(_position._x, _position._y, 0.0);		// Move to center
	glRotatef(_rotation, 0.0, 0.0, 1.0);				// Rotate saw
	_drw->drawTextured2dQuadCenter(0, 0, _xrad, _yrad);	// Draw saw
	glRotatef(-_rotation, 0.0, 0.0, 1.0);				// Don't rotate pivot

	_drw->setTexture(DrawingAssistant::T_NME_SAW_INNER);
	_drw->drawTextured2dQuadCenter(0, 0, _xrad, _yrad);	//Draw pivot
	glTranslatef(-_position._x, -_position._y, 0.0);	// Move to position
	
}

void EnemySaw::move()
{
	_rotation += _rotateSpeed;							// Add rotation speed

	Vector2d direction = _ptrTarget->subtract(_position);
	_movementControl->setDirection(direction.normalize());
	_movementControl->accelerate();
	_movementControl->move();
}

bool EnemySaw::hitTest(float x, float y)
{
	Vector2d distance = _position.subtract( Vector2d( x, y ) );
	if(distance.magnitude() < _xrad)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void EnemySaw::reset(float x, float y)
{
	_movementControl->F = Vector2d(0,0);
	_position._x = x;
	_position._y = y;
	_active = true;
}

void EnemySaw::kill()
{
	Enemy::kill();
	_sfx->explosion(_position._x, _position._y);
	_active = false;
}

EnemySaw::~EnemySaw()
{
	delete _movementControl;
}
