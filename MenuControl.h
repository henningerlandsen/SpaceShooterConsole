#pragma once
#include "IDrawable.h"
#include "SoundManager.h"
#include "MenuItem.h"
#include "MenuActions.h"
#include <stdio.h>

using namespace std;

class MenuControl : public IDrawable
{
	//================= Member attributes =================
public:
	enum MenuID {
		MENU_START,
		MENU_INGAME,
		MENU_END,
		MENU_LENGTH
	};
	bool menuActive;
	MenuID _activeMenu;

private:
	SoundManager* _snd;

	struct MenuGroup
	{
		vector<MenuItem> items; //Collection of MenuItems.
		int selected;
		bool active;
	};

	MenuGroup menus[MENU_LENGTH]; //List of menus.

	//================= Member functions =================
public:
	MenuControl(DrawingAssistant* drw, SoundManager* snd);
	~MenuControl();
	void call(int keynum);
	void showMenu(MenuID menuID);

	//IDrawable implementation
	void draw();
	void move();
};

MenuControl::MenuControl(DrawingAssistant *drw, SoundManager *snd)
: IDrawable(drw)
{
	_snd = snd;
	MenuActions::registerSoundManager(_snd);
	menuActive = false;
	
	MenuItem item("RESUME GAME", drw);
	item.registerKeyAction(13, MenuActions::mitStartGame_onSelect);
	menus[MENU_START].items.push_back( item );
	
	char* label = _snd->getSoundLevel();
	item = MenuItem( label, drw );
	item.registerKeyAction((int)GLUT_KEY_LEFT, MenuActions::mitSoundVolume_onDown);
	item.registerKeyAction((int)GLUT_KEY_RIGHT, MenuActions::mitSoundVolume_onUp);	
	menus[MENU_START].items.push_back( item );
	
	label = _snd->getMusicLevel();
	item = MenuItem( label, drw );
	item.registerKeyAction((int)GLUT_KEY_LEFT, MenuActions::mitMusicVolume_onDown);
	item.registerKeyAction((int)GLUT_KEY_RIGHT, MenuActions::mitMusicVolume_onUp);
	menus[MENU_START].items.push_back( item );

	showMenu(MENU_START);
	//menuActive = true;
}

void MenuControl::draw()
{
	if(menuActive)
	{
		vector<MenuItem>::iterator j;
		int k = 0;
		for(j=menus[_activeMenu].items.begin(); j!=menus[_activeMenu].items.end(); j++)
		{
			if(menus[_activeMenu].selected != k)
				glColor4f(0.5, 0.5, 0.5, 1.0);
			else
				glColor4f(1.0, 1.0, 1.0, 1.0);
			(*j).draw(-0.3*k);
			k++;
		}
	}
}

void MenuControl::move()
{

}

void MenuControl::call(int keynum)
{
	switch(keynum)
	{
	case GLUT_KEY_DOWN:
		if(++menus[_activeMenu].selected >= menus[_activeMenu].items.size())
			menus[_activeMenu].selected = 0;
		break;
	case GLUT_KEY_UP:
		if(--menus[_activeMenu].selected < 0)
			menus[_activeMenu].selected = menus[_activeMenu].items.size()-1;
		break;
	default:
		menus[_activeMenu].items[menus[_activeMenu].selected].callKeyAction(keynum);
		break;
	}

	char* label = _snd->getSoundLevel();
	menus[MENU_START].items[1].setText(label);
	label = _snd->getMusicLevel();
	menus[MENU_START].items[2].setText(label);

}

void MenuControl::showMenu(MenuControl::MenuID menuID)
{
	menus[menuID].selected = 0;
	menus[menuID].active = true;
	_activeMenu = menuID;
}

MenuControl::~MenuControl()
{
}