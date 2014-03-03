#pragma once
#include "enemy.h"
#include "EnemySpike.h"

#define SPIKES 3

class EnemyMine :
	public Enemy
{	
	bool		_destroyed;
	float		_rotateSpeed;
	EnemySpike*	_spikes[SPIKES];


public:
	EnemyMine(DrawingAssistant* drw, SpecialFXManager* sfx/*, UpgradeManager* upg*/);
	~EnemyMine(void);
	void draw();
	bool hitTest(float x, float y);
	bool hitTest(Enemy* e);
	void move();
	void reset(float x, float y);
	void kill();
};

EnemyMine::EnemyMine(DrawingAssistant* drw, SpecialFXManager* sfx/*, UpgradeManager* upg*/)
	: Enemy(100, 0.2f, 0.2f, drw, sfx/*, upg*/)
{
	for(int i=0; i<3; i++)
		_spikes[i] = new EnemySpike(drw, sfx, 0/*, upg*/);
	_position._y	= 2.5;
	_position._x	= 1.0;
	_rotateSpeed	= 0.2;
	_destroyed		= false;
	_active			= false;
	
	_movementControl->a		= 0.00003;
	_movementControl->ts	= 0.02;
	_movementControl->f		= 0.995;
	_movementControl->g		= Vector2d(0.0, -0.00008);
}

#include <cmath>
using namespace std;

void EnemyMine::draw()
{
	if(!_destroyed)
	{
		glTranslatef(_position._x, _position._y, 0.0);		//Move to center
		glRotatef(-_rotation, 0.0, 0.0, 1.0);				//Rotate world			

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);					//Set state
		glColor4f(0.0, 1.0, 0.0, 1.0);
		_drw->setTexture(DrawingAssistant::T_XPL_BALL);
		_drw->drawTextured2dQuadCenter(0,0, _xrad/2, _yrad/2);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//Set state
		glColor4f(1.0, 1.0, 1.0, 1.0);

		_drw->setTexture(DrawingAssistant::T_NME_MINE);
		_drw->drawTextured2dQuadCenter(0, 0, _xrad, _yrad);

		_drw->setTexture(DrawingAssistant::T_NME_MINE_SPIKE);

		for(int i=0; i<3; i++)
		{
			glRotatef(-120*i, 0, 0, 1);
			glTranslatef(0, 0.16, 0);
			_drw->drawTextured2dQuadCenter(0, 0, 0.075, 0.075);
			glTranslatef(0, -0.16, 0);
			glRotatef(120*i, 0, 0, 1);
		}

		glRotatef(_rotation, 0.0, 0.0, 1.0);				//Rotate back
		glTranslatef(-_position._x, -_position._y, 0.0);	//Move to position

	}
	else
		for(int i=0; i<SPIKES; i++)
			_spikes[i]->draw();
}

bool EnemyMine::hitTest(float x, float y)
{
	if(!_destroyed)
	{
		Vector2d distance = _position.subtract( Vector2d( x, y ) );
		return distance.magnitude() < _xrad;
	}
	else
	{
		for(int i=0; i<SPIKES; i++)
		{
			if(_spikes[i]->hitTest(x, y))
			{
				_spikes[i]->_active = false;
				return true;
			}
		}
		return false;
	}
}

bool EnemyMine::hitTest(Enemy* e)
{
	if(e->_active)
	{
		if(!_destroyed)
		{
			if(_position.subtract(e->_position).magnitude()<_xrad + e->_xrad)
			{
				onEnemyHit(e);
				return true;
			}
		}
		else
		{
			for(int i=0; i<SPIKES; i++)
			{
				if(_spikes[i]->_active)
				{
					_spikes[i]->Enemy::hitTest(e);
				}
			}
		}
	}
	return false;
}

void EnemyMine::move()
{
	if(!_destroyed)
	{
		_movementControl->setDirection(Vector2d(0,0));
		_movementControl->accelerate();
		_movementControl->move();
		_rotation += _rotateSpeed;
	}
	else if(_spikes[0]->_life > 0)
		for(int i=0; i<SPIKES; i++)
			_spikes[i]->move();
	else
		_active = false;
}

void EnemyMine::reset(float x, float y)
{
	_movementControl->F = Vector2d(0,0);
	_position._x = x;
	_position._y = y;
	_active = true;
	_destroyed = false;
}

void EnemyMine::kill()
{
	if(!_destroyed)
	{
		Enemy::kill();
		_destroyed = true;
		for(int i=0; i<SPIKES; i++)
		{
			_spikes[i]->reset(_position._x, _position._y);
 			_spikes[i]->setDirection(_rotation + (i*120));
		}

		_sfx->ringOfFire(_position._x, _position._y);
	}
}

EnemyMine::~EnemyMine()
{
	for ( int i = 0; i < SPIKES; ++i )
		delete _spikes[i];
}