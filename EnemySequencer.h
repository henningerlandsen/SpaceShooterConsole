#include "EnemyManager.h"
#include "Vector2d.h"
#include <iostream>

#define MAX_EVENTS 200

	float bounds_left		= -2.7;
	float bounds_right		= 2.7;
	float bounds_top		= 2.3;
	float bounds_bottom		= -2.3;

class EnemySequencer
{
private:

	int _counter;
	int _eventIterator;
	int _lastEvent;
	EnemyManager* _enemyManager;

	typedef struct Event {
		int time;
		EnemyManager::Deployment deploy;
	};

	Event _events[MAX_EVENTS];

private:
	void newEvent(float seconds, int type, float px, float py, float fx, float fy);
	void newEventI(int time,  EnemyManager::EnemyType type, float px, float py, float fx, float fy);
	void repeatEvent(int repetitions, float interval, bool mirrored);

public:
	EnemySequencer(EnemyManager* eMngr);
	~EnemySequencer();
	void tick();
};

EnemySequencer::EnemySequencer(EnemyManager *eMngr)
{
	_enemyManager	= eMngr;
	_counter		= 0;
	_eventIterator	= 1;
	_lastEvent		= 0;

	_events[0].time = 0;

	cout << "Building level";

	//Mine Triangel
	newEvent(3, 1, 
		0, bounds_top, 
		0.0, 0.0);
	newEvent(1, 1,
		0.5, bounds_top,
		0.0, 0.0);
	repeatEvent(0, 0, true);

	//Side saws
	newEvent(2, 0,
		2.0, bounds_top,
		0.0, 0.0);
	repeatEvent(1, 1.0, true);

	repeatEvent(1, 3.0, true);
	repeatEvent(1, 1.0, true);

	
	//Mine Triangel
	newEvent(3, 1, 
		0, bounds_top, 
		0.0, 0.0);
	newEvent(1, 1,
		0.5, bounds_top,
		0.0, 0.0);
	repeatEvent(0, 0, true);

	//Side saws
	newEvent(2, 0,
		2.0, bounds_bottom,
		0.1, 0.7);
	repeatEvent(1, 1.0, true);

	repeatEvent(1, 3.0, true);
	repeatEvent(1, 1.0, true);

	//-------------------------------

	//Saw wave of four from top
	newEvent(4, 0,
		2, bounds_top,
		-0.005, -0.001);
	repeatEvent(3, 0.7, true);

	//Second saw wave of four from top
	newEvent(4, 0,
		2, bounds_top,
		-0.005, -0.001);
	repeatEvent(3, 0.7, true);

	//Saw wave from bottom
	newEvent(4, 0,
		2, bounds_bottom,
		-0.005, 0.04);
		repeatEvent(3, 0.7, true);

	//-----------------------------

	//Five parralell mines from top
	newEvent(7, 1,
		0, bounds_top,
		0,0);
	newEvent(0, 1,
		1, bounds_top,
		0,0);
		repeatEvent(0, 0, true);
	newEvent(0, 1,
		2, bounds_top,
		0,0);
		repeatEvent(0, 0, true);

	//Five parralell mines from top
	newEvent(2, 1,
		0, bounds_top,
		0,0);
	newEvent(0, 1,
		1, bounds_top,
		0,0);
		repeatEvent(0, 0, true);
	newEvent(0, 1,
		2, bounds_top,
		0,0);
		repeatEvent(0, 0, true);
	
		//------------------------

	//Five parralell mines from top
	newEvent(7, 1,
		0, bounds_top,
		0,0);
	newEvent(0, 1,
		1, bounds_top,
		0,0);
		repeatEvent(0, 0, true);
	newEvent(0, 1,
		2, bounds_top,
		0,0);
		repeatEvent(0, 0, true);

	//Five parralell mines from top
	newEvent(2, 1,
		0, bounds_top,
		0,0);
	newEvent(0, 1,
		1, bounds_top,
		0,0);
		repeatEvent(0, 0, true);
	newEvent(0, 1,
		2, bounds_top,
		0,0);
		repeatEvent(0, 0, true);
	
	//Four inline mines
	newEvent(5, 1,
		0, bounds_top,
		0,-0.01);
	repeatEvent(3, 0.5,false);

	//Four double-spreading saws
	newEvent(0, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);
	newEvent(0.5, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);
	newEvent(0.5, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);
	newEvent(0.5, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);
	newEvent(0.5, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);

	//-----------------------------
	
	//Four inline mines
	newEvent(5, 1,
		0, bounds_top,
		0,-0.01);
	repeatEvent(3, 0.5,false);

	//Four double-spreading saws
	newEvent(0, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);
	newEvent(0.5, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);
	newEvent(0.5, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);
	newEvent(0.5, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);
	newEvent(0.5, 0,
		0, bounds_top,
		0.01, -0.01);
	repeatEvent(0, 0, true);

	//-----------------------------

	//Dual saw lines on side
	newEvent(5, 0,
		2, bounds_top,
		-0.01, -0.01);
	repeatEvent(4, 0.5, true);


	cout << "[" << _lastEvent+1 << "/" << MAX_EVENTS << "]" << endl;
}

void EnemySequencer::tick()
{
	while(_eventIterator <= _lastEvent && _counter == _events[_eventIterator].time)
	{
		_enemyManager->deploy(_events[_eventIterator].deploy);

		// when done, restart sequence (keep game going forever! )
		if ( _eventIterator == _lastEvent )
		{
			_eventIterator = 1;
			_counter = -1;
		}
		else
			_eventIterator++;
	}
	_counter++;
}

void EnemySequencer::newEvent(float seconds, int type, float px, float py, float fx, float fy)
{
	newEventI((int)(seconds * FPS), (EnemyManager::EnemyType)type, px, py, fx, fy);
}

void EnemySequencer::newEventI(int time, EnemyManager::EnemyType type, float px, float py, float fx, float fy)
{
	cout << ".";
	time = _events[_lastEvent].time + time;
	if(_lastEvent < MAX_EVENTS-1) 
	{
		_lastEvent++;
		_events[_lastEvent].deploy.type		= type;
		_events[_lastEvent].deploy.position	= Vector2d(px, py);
		_events[_lastEvent].deploy.force	= Vector2d(fx, fy);
	
		_events[_lastEvent].time = time;
	}
}

void EnemySequencer::repeatEvent(int repetitions, float interval, bool mirrored)
{
	Event tmp = _events[_lastEvent];
	if(mirrored)
	{
		newEventI(
			0,
			tmp.deploy.type,
			-tmp.deploy.position._x,	tmp.deploy.position._y,
			-tmp.deploy.force._x,		tmp.deploy.force._y
			);
	}
	for(int i=0; i<repetitions; i++)
	{
		tmp = _events[_lastEvent];
		newEventI(
			tmp.time = interval*FPS ,
			tmp.deploy.type,
			tmp.deploy.position._x,	tmp.deploy.position._y,
			tmp.deploy.force._x,		tmp.deploy.force._y
			);
		
		if(mirrored)
		{
			newEventI(
				0,
				tmp.deploy.type,
				-tmp.deploy.position._x,	tmp.deploy.position._y,
				-tmp.deploy.force._x,		tmp.deploy.force._y
				);
		}
	}
}
