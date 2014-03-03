#pragma once
#include "IDrawable.h"
#include "MenuActions.h"
#include <vector>

using namespace std;

class MenuItem : public IDrawable
{
	//================= Member attributes =================
private:
	struct KeyAction
	{
		int key;
		void (*ptrAction)(void);
	};

	vector<KeyAction> _actionList;
	char* _text;

	//================= Member functions =================
public:
	MenuItem(char* c, DrawingAssistant* drw);								//MenuItem Constructor. Arguments: char* c - Menu text
	void setText(char* c);													//Sets menu text. Argumante: char* c - Menu text
	void registerKeyAction(int key, void(*ptrMethod)(void));	//Registers an action to a key.
	void callKeyAction(int key);											//Finds and executes the action attached to a key.

	//IDrawable implementation
	void draw();
	void draw(float ypos);
	void move();
};

MenuItem::MenuItem(char *c, DrawingAssistant* drw)
:IDrawable(drw)
{

	setText(c);
}

void MenuItem::setText(char *c)
{
	_text = c;
}

void MenuItem::registerKeyAction(int key, void (*ptrMethod)(void))
{
	KeyAction temp;

	temp.key = key;
	temp.ptrAction = ptrMethod;

	_actionList.push_back(temp);
}

void MenuItem::callKeyAction(int key)
{
	vector<KeyAction>::iterator i;
	for(i=_actionList.begin(); i != _actionList.end(); i++) //Search for key id.
	{
		if((*i).key == key)
		{
			void(*ptrTemp)() = (*i).ptrAction;
			(*ptrTemp)();
			//MenuActions::mitSoundVolume_onDown();
		}
	}
}

void MenuItem::draw()
{
	draw(0.0);
}

void MenuItem::draw(float ypos)
{
	_drw->setTexture(DrawingAssistant::T_TEXT);
	_drw->write(_text, 0.2, 0.3, -1, ypos);
}

void MenuItem::move()
{

}

