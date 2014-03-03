//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )  
#pragma once

#define FPS 60

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include "DrawingAssistant.h"
#include "SpaceShip.h"
#include "FloatingObject.h"
#include "Vector2d.h"
#include "StarField.h"
#include "WeaponManager.h"
#include "EnemyManager.h"
#include "HudElements.h"
#include "SoundManager.h"
#include "Upgrade.h"
#include "UpgradableShield.h"
#include "EnemySequencer.h"
#include "MenuControl.h"
#include "GL\glaux.h"
#include "GL\glut.h"
#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

enum Drawables {
	STARFIELD,
	SPACESHIP,
	ENEMIES, 
	UPGRADE,
	WEAPONS, 
	HUD,
	MAX_DRAWABLES};

	IDrawable* entity[MAX_DRAWABLES];

bool isLeft;
bool isRight;
bool isUp;
bool isDown;
bool isPaused;
int timeStamp;
int msPerFrame;
int counter;
int gamespeed;
int now;
int timebase;
char s[30];

FloatingObject* shipMovement;
SoundManager	sound;
UpgradableShield* shieldUpgrader;
EnemySequencer* events;


DrawingAssistant drw;
MenuControl mainMenu(&drw, &sound);

// Render Scene -------------------------------------------------------------------
void renderScene(void)
{

	// Draw -----------------------------------------------------------------------
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i<MAX_DRAWABLES; i++)
	{
		entity[i]->draw();
	}
	mainMenu.draw();
	glutSwapBuffers();

	// ----------------------------------------------------------------------- Draw

	// Animate --------------------------------------------------------------------

	int interval = glutGet(GLUT_ELAPSED_TIME) - timeStamp;	//Are we ready for another frame or perhaps two or three?
	int frames = interval/msPerFrame;			//Calculate number of animation-frames to do
	timeStamp += frames*msPerFrame;				//Adjust our enviroment-timer accordingly.

	for(int i=0; i<frames && i<3; i++)		//Animate according to cpu-time lag. CAP@3
	{
		if(!isPaused && !mainMenu.menuActive)
		{
			Vector2d dir = Vector2d();

			if(isLeft)
				dir = dir.add(Vector2d(-1, 0));
			if(isRight)
				dir = dir.add(Vector2d(1, 0));
			if(isUp)
				dir = dir.add(Vector2d(0, 1));
			if(isDown)
				dir = dir.add(Vector2d(0, -1));

			SpaceShip* ship = (SpaceShip*)entity[SPACESHIP];
			ship->_movementControl->setDirection(dir);
			ship->_movementControl->accelerate();
			ship->_movementControl->move();

			EnemyManager* enemyMngr = (EnemyManager*)entity[ENEMIES];
			enemyMngr->hitTest((WeaponManager*)entity[WEAPONS]);
			enemyMngr->hitTest(ship);
			enemyMngr->hitTest();

			((UpgradeManager*)entity[UPGRADE])->hitTest(ship->shipPos._x, ship->shipPos._y);

			for(int i = 0; i<MAX_DRAWABLES; i++)
			{
				entity[i]->move();
			}
			counter++;
			events->tick();
		}
	}

	// -------------------------------------------------------------------- Animate
}

// ------------------------------------------------------------------- Render Scene 

// Key Mapping --------------------------------------------------------------------
void onSpecialDown(int key, int x, int y)
{
	if(mainMenu.menuActive)
	{
		mainMenu.call(key);
	}
	else
	{
		switch(key) {
			case GLUT_KEY_LEFT:
				isLeft = true;
				break;
			case GLUT_KEY_RIGHT:
				isRight = true;
				break;
			case GLUT_KEY_UP:
				isUp = true;
				break;
			case GLUT_KEY_DOWN:
				isDown = true;
				break;
		}
		cout << "Key " << key << " pressed!" << endl;
	}
}

void onKeyDown(unsigned char key, int x, int y)
{
	cout << (int)key << " ";
	if (key == 27) 
		mainMenu.menuActive = !mainMenu.menuActive;
	if(key == 32) // SPACE BAR
	{
		if( ((WeaponManager*)entity[WEAPONS])->fireStart() )
			sound.play(SoundManager::SHOOT);
	}
	if(key == 118 || key == 86) // V
	{
		isPaused = !isPaused;
	}
	if(key == 9) ((UpgradeManager*)entity[UPGRADE])->switchUpgrade();
	if(key == 49) gamespeed = 1; // 1
	if(key == 50) gamespeed = 2;// 2
	if(key == 51) gamespeed = 3;// 3
	if(key == 52) gamespeed = 4;// 4
	if(key == 53) gamespeed = 5;// 5
	if(key == 54) gamespeed = 6;// 6
}

void onSpecialUp(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT:
			isLeft = false;
			break;
		case GLUT_KEY_RIGHT:
			isRight = false;
			break;
		case GLUT_KEY_UP:
			isUp = false;
			break;
		case GLUT_KEY_DOWN:
			isDown = false;
			break;
		case GLUT_KEY_F2:
			((SpaceShip*)entity[SPACESHIP])->moveEnergyLimit(20);
			break;
	}
	cout << "Key " << key << " released!" << endl;
}

void onKeyUp(unsigned char key, int x, int y)
{
	if(key == 32) // SPACE BAR
	{
		((WeaponManager*)entity[WEAPONS])->fireStop();
	}
}
// ------------------------------------------------------------------- Key Mapping 

// Texture Loading ---------------------------------------------------------------
bool setupTexture()
{
	drw.loadTextures();

	glDepthFunc(GL_LEQUAL);									// Type Of Depth Testing
	glEnable(GL_DEPTH_TEST);								// Enable Depth Testing
	glEnable(GL_BLEND);										// Enable Blending       (disable alpha testing)
	glAlphaFunc(GL_GREATER,0.01f);							// Set Alpha Testing     (disable blending)
	glEnable(GL_ALPHA_TEST);								// Enable Alpha Testing  (disable blending)
	glEnable(GL_TEXTURE_2D);								// Enable Texture Mapping
	glEnable(GL_CULL_FACE);									// Remove Back Face
	glShadeModel(GL_SMOOTH);
	
	return true; 
}
// --------------------------------------------------------------- Texture Loading

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);
}

bool setVSync(bool sync)
{
	typedef bool (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);

	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

	if( wglSwapIntervalEXT )
	{
		wglSwapIntervalEXT(sync);
		return true;
	}
	else return false;
}

void main(int argc, char **argv)
{
	isPaused = false;
	msPerFrame = 1000/FPS;
	counter  = 0;
	gamespeed= 1;
	glutInit(&argc, argv);
	timeStamp	= glutGet(GLUT_ELAPSED_TIME);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,600);

	glutCreateWindow("TroubleShooter");
	
	//Window function registration.
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glEnable(GL_DEPTH_TEST);

	setupTexture();
	cout << endl << "Setting up vertical sync: ";
	if(setVSync(true)) cout << "Enabled" << endl;
	else cout << "Not available" << endl;

	//Starfield -----------------------------------------------------------
	entity[STARFIELD] = new StarField(&drw); //	starField = new StarField(&drw);
	//----------------------------------------------------------- Starfield

	//SpaceShip -----------------------------------------------------------
	entity[SPACESHIP] = new SpaceShip(&drw);
	SpaceShip* ship = (SpaceShip*)entity[SPACESHIP];
	// ---------------------------------------------------------- SpaceShip

	//Weapon Manager ------------------------------------------------------
	entity[WEAPONS] = new WeaponManager(&drw, ship);
	// ----------------------------------------------------- Weapon Manager

	//Upgrade Manager -----------------------------------------------------
	shieldUpgrader	= new UpgradableShield(ship);
	entity[UPGRADE] = new UpgradeManager(&drw, (WeaponManager*)entity[WEAPONS], ship, shieldUpgrader, &sound);
	// ---------------------------------------------------- Upgrade Manager

	//Emeny Init ----------------------------------------------------------
	entity[ENEMIES] = new EnemyManager(ship->shipPos, &drw, &sound, (UpgradeManager*)entity[UPGRADE]);
	//---------------------------------------------------------- Emeny Init

	//Hud Elements --------------------------------------------------------
	entity[HUD] = new HudElements(ship->getEnergyLimitPointer(), ship->getEnergyPointer(), &drw, (UpgradeManager*)entity[UPGRADE]);
	//-------------------------------------------------------- Hud Elements
	
	events = new EnemySequencer((EnemyManager*)entity[ENEMIES]);

	sound.music();

	//Keyboard Listeners and Setup
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialUp);
	glLoadIdentity();

	glutMainLoop();
}