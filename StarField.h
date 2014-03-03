#pragma once

#include "IDrawable.h"

#define MAX_STARS 300

//////////////////////////////////////////////////////////////////////////////////////////

class StarField : public IDrawable
{
public:
	struct Star
	{
		float x;
		float y;
		float z;
	};
	
	Star stars[MAX_STARS];
	float speed;
	float starSize;

	
	StarField(DrawingAssistant* drw);
	~StarField();
	void draw();
	void move();
};

/////////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

StarField::StarField(DrawingAssistant* drw)
	: IDrawable(drw)
{
	srand( time(0) );
	for(int i=0; i<MAX_STARS; i++)
	{
		stars[i].z = ((float)rand() / RAND_MAX ) * 10.0;
		stars[i].x = ( -2.5 + (float)rand() / RAND_MAX *  5.0) * ( 1 + stars[i].z/5.0 );
		stars[i].y = ( -2.5 + (float)rand() / RAND_MAX *  5.0) * ( 1 + stars[i].z/5.0 );
	}
	speed = 0.03;
	starSize = 0.06;
}

StarField::~StarField()
{
}

void StarField::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_TEXTURE_2D);

	for(int i=0; i<MAX_STARS; i++)
	{
		_drw->setTexture(DrawingAssistant::T_STAR);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0-stars[i].z/10.0);
		_drw->drawTextured2dQuadCenter(stars[i].x, stars[i].y, -stars[i].z, starSize, starSize);
	}
}

void StarField::move()
{
	for(int i=0; i<MAX_STARS; i++)
	{
		if(stars[i].y < -2.5 * ( 1 + stars[i].z/5.0 ) )
		{
			stars[i].z = ((float)rand() / RAND_MAX ) * 10.0;
			stars[i].x = ( -2.5 + (float)rand() / RAND_MAX *  5.0) * ( 1 + stars[i].z/5.0 );
			stars[i].y = 2.5 * ( 1 + stars[i].z/5.0 );
		}
		stars[i].y -= speed;
	}
}