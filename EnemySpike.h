#pragma once
#include "Enemy.h"

#define PI 3.14159265
#define SPIKE_PARTICLES 70

class EnemySpike : public Enemy 
{
	struct Particle{
		float x;
		float y;
		float life;
	};
	Particle	particles[100];
	float		particleSize;
	int			frameCount;
	int			particleInterval;

public:
	float		_xdir;
	float		_ydir;
	float		_speed;
	float		_life;


	EnemySpike(DrawingAssistant *drw, SpecialFXManager* sfx, float direction/*, UpgradeManager* upg*/);
	~EnemySpike();
	void draw();
	void setDirection(float direction);
	bool hitTest(float x, float y);
	void move();
	void reset(float x, float y);
	void kill();
	void onEnemyHit(Enemy* e);
};

EnemySpike::EnemySpike(DrawingAssistant *drw, SpecialFXManager* sfx, float direction/*, UpgradeManager* upg*/)
: Enemy(100, .075, .075, drw, sfx/*, upg*/)
{
	_speed = 0.03;
	setDirection(direction);

	for(int i=0; i<SPIKE_PARTICLES; i++)
	{
		particles[i].life = 0;
	}

	frameCount = 0;
	particleInterval = 1.5;
	particleSize = 0.05;
}

void EnemySpike::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	_drw->setTexture(DrawingAssistant::T_PARTICLE);
	for(int i=0; i<SPIKE_PARTICLES; i++)
	{
		if(particles[i].life > 0)
		{
			glColor4f(1.0, particles[i].life, 0.0, particles[i].life);
 			_drw->drawTextured2dQuadCenter(particles[i].x, particles[i].y, particleSize, particleSize);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	_drw->setTexture(DrawingAssistant::T_NME_MINE_SPIKE);

	glTranslatef(_position._x, _position._y, 0);
	glRotatef(-_rotation, 0, 0, 1);
	_drw->drawTextured2dQuadCenter(0, 0, _xrad, _yrad);
	glRotatef(_rotation, 0, 0, 1);
	glTranslatef(-_position._x, -_position._y, 0);

}

void EnemySpike::setDirection(float direction)
{
	_xdir = sin(direction*PI/180) * _speed;
	_ydir = cos(direction*PI/180) * _speed;
	_rotation = direction;
}

bool EnemySpike::hitTest(float x, float y)
{
	Vector2d distance = _position.subtract( Vector2d( x, y ) );
	return distance.magnitude() < _xrad;
}

void EnemySpike::move()
{
	_position._x += _xdir;
	_position._y += _ydir;

	frameCount++;

	for(int i=0; i<SPIKE_PARTICLES; i++)
	{
		if(particles[i].life > 0)
		{
			particles[i].life -= 0.015;
		}
	}

	if(frameCount%particleInterval == 0)
	{
		for(int i=0; i<SPIKE_PARTICLES; i++)
		{
			if(particles[i].life <= 0)
			{
				particles[i].life = 1.0;
				particles[i].x	  = _position._x;
				particles[i].y	  = _position._y;
				break;
			}
		}
	}

	_life -= 0.3;
}

void EnemySpike::reset(float x, float y)
{
	_position._x = x;
	_position._y = y;
	_active = true;
	_life	= 100.0;
}

void EnemySpike::kill()
{
	_active = false;
}

void EnemySpike::onEnemyHit(Enemy* e)
{
	e->kill();
}

EnemySpike::~EnemySpike()
{
}