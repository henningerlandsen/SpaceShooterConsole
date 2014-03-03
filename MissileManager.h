#ifndef MISSILEMANAGER_H
#define MISSILEMANAGER_H
#include "Vector2d.h";

class MissileManager
{
private:
	int MAX_MISSILES;
	
public:
	Vector2d[MAX_MISSILES] missiles;
	MissileManager(int maxMissleNum, float missileTravelLength);
	fireMissile(static Vector2d &shipPos);
	draw();
};
#endif //MISSILEMANAGER_H