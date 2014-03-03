#pragma once

#include "IDrawable.h"
#include "Vector2d.h"
#include "FloatingObject.h"
#include "SpecialFXManager.h"
#include "AbstractListener.h"

class Enemy : public IDrawable
{
public:
	float		_health;
	Vector2d	_position;
	float		_xrad;
	float		_yrad;
	float		_rotation;
	bool		_active;

	AbstractListener* _listener;
	SpecialFXManager* _sfx;
	FloatingObject* _movementControl;
	//UpgradeManager* _upgrade; 

	Enemy(float health, float xRadius, float yRadius, DrawingAssistant* drw, SpecialFXManager* sfx);//, UpgradeManager* upg);
	~Enemy();
	virtual void draw();// = 0;
	virtual bool hitTest(float x, float y);// = 0;
	virtual bool hitTest(Enemy* e);
	virtual void onEnemyHit(Enemy* e);
	virtual void move();// = 0;
	virtual void reset(float x, float y);// = 0;
	virtual void kill();// = 0;
	virtual void addListener(AbstractListener* listener);
};

Enemy::Enemy(float health, float xRadius, float yRadius, DrawingAssistant* drw, SpecialFXManager* sfx)//, UpgradeManager* upg)
	: IDrawable(drw)
{
	_listener = new AbstractListener();
	_movementControl = new FloatingObject(_position);
	_sfx		= sfx;
	//_upgrade	= upg;
	_health		= health;
	_xrad		= xRadius;
	_yrad		= yRadius;
	_rotation	= 0.0;
	_active		= false;
}

Enemy::~Enemy()
{
	delete _movementControl;
}

void Enemy::draw(){}
bool Enemy::hitTest(float x, float y){return false;}
bool Enemy::hitTest(Enemy* e)
{
	if(e->_active)
	{
		if(_position.subtract(e->_position).magnitude()<_xrad + e->_xrad)
		{
			onEnemyHit(e);
			return true;
		}
	}
	return false;
}

void Enemy::onEnemyHit(Enemy* e){}
void Enemy::move(){}
void Enemy::reset(float x, float y){}
void Enemy::kill()
{
	_listener->trigger(_position._x, _position._y);
}

void Enemy::addListener(AbstractListener* listener)
{
	delete _listener;
	_listener = listener;
}