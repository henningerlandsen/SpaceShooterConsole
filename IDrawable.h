#ifndef IDRAWABLE_H
#define IDRAWABLE_H
#include "DrawingAssistant.h"
#include "GL\glaux.h"
#include "GL\glut.h"

class IDrawable
{
protected:
	
public:
	DrawingAssistant* _drw;
	IDrawable(DrawingAssistant* drw);
	virtual void draw();
	virtual void move();
};

IDrawable::IDrawable(DrawingAssistant* drw)
{
	_drw = drw;
}

void IDrawable::draw(){}
void IDrawable::move(){}

#endif //IDRAWABLE_H