#pragma once
#include "IDrawable.h"
#include "TweenControl.h"

#define MAX_FXDRAWINGS 100

static float colors[12][3] =
{
	{1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
	{0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
	{0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};

////////////////////////////////////////////////////////////////////////////////////////////

class SpecialFXManager :
	public IDrawable
{
	int sprites;
	int particles;
	int trails;
public:
	float tx;
	float ty;
	float tz;
	float ts;

	TweenControl tc;

	typedef struct{
		float life;
		float fade;
		float color_r;
		float color_g;
		float color_b;
		float alpha;
		float rotate;
		TweenControl::Tween x;
		TweenControl::Tween y;
		TweenControl::Tween z;
		TweenControl::Tween size;
		DrawingAssistant::TextureNum tex;
	} Drawing;

	Drawing drawings[MAX_FXDRAWINGS];

	SpecialFXManager(DrawingAssistant* drw);
	~SpecialFXManager(void);
	void explosion(float x, float y);
	void ringOfFire(float x, float y);
	void ringOfGreen(float x, float Y);
	void addToBuffer(Drawing temp[], int length);
	void draw();
	void move();
};

////////////////////////////////////////////////////////////////////////////////////////////

#include "TweenControl.h"
#include <cmath>
#include <iostream>
#include <ctime>

using namespace std;

SpecialFXManager::SpecialFXManager(DrawingAssistant* drw)
: IDrawable(drw)
{
	particles = 6;
	trails = 3;
	sprites = particles * trails + 3;
	srand(time(0));

	for(int i=0; i<MAX_FXDRAWINGS; i++)
	{
		drawings[i].alpha = 0;
		drawings[i].color_b = colors[rand()%12][2];
		drawings[i].color_g = colors[rand()%12][1];
		drawings[i].color_r = colors[rand()%12][0];
		drawings[i].fade = 0;
		drawings[i].life = 1.0;
		drawings[i].rotate = 0;

		drawings[i].size.a = 0;
		drawings[i].size.b = 0;
		drawings[i].size.exp = 0;

		drawings[i].x.a = 0;
		drawings[i].x.b = 0;
		drawings[i].x.exp = 0;

		drawings[i].y.a = 0;
		drawings[i].y.b = 0;
		drawings[i].y.exp = 0;
		
		drawings[i].z.a = 0;
		drawings[i].z.b = 0;
		drawings[i].z.exp = 0;
	}
}

SpecialFXManager::~SpecialFXManager(void)
{
}

void SpecialFXManager::draw()
{
	glDepthFunc(GL_ALWAYS);
	for(int i=0; i<MAX_FXDRAWINGS; i++)
	{
		if(drawings[i].life < 1.0)
		{
 			tx = tc.tween(drawings[i].x, drawings[i].life);
			ty = tc.tween(drawings[i].y, drawings[i].life);
			tz = tc.tween(drawings[i].z, drawings[i].life);
			ts = tc.tween(drawings[i].size, drawings[i].life);

 			glColor4f(drawings[i].color_r, drawings[i].color_g, drawings[i].color_b, drawings[i].alpha - drawings[i].life);
			_drw->setTexture(drawings[i].tex);

			//if(drawings[i].tex == DrawingAssistant::T_PARTICLE_RGB) cout << "PID:  " << i << " \tR: " << drawings[i].color_r << "\tG: " << drawings[i].color_g << "\tB: " << drawings[i].color_b << "\tZ: " << drawings[i].z.b << endl;
			
			glTranslatef(tx, ty, tz);
			glRotatef(drawings[i].rotate * drawings[i].life, 0.0, 0.0, 1.0);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			_drw->drawTextured2dQuadCenter( 0, 0, 0, ts, ts);
			
			glRotatef(-drawings[i].rotate * drawings[i].life, 0.0, 0.0, 1.0);
			glTranslatef(-tx, -ty, -tz);
		}
	}
	glDepthFunc(GL_LEQUAL);	
}

void SpecialFXManager::move()
{
	for(int i=0; i<MAX_FXDRAWINGS; i++)
	{
		if(drawings[i].life < 1.0)
		{
			drawings[i].life += drawings[i].fade;
		}
	}
}

void SpecialFXManager::ringOfFire(float x, float y)
{
	int const temp_length = 10;
	Drawing temp[temp_length];

	temp[0].alpha	= 1.0;
	temp[0].color_b = 0.0;
	temp[0].color_g = 0.5;
	temp[0].color_r = 1.0;
	temp[0].fade	= 0.01;
	temp[0].life	= 0.0;
	temp[0].rotate	= 0.0;
	temp[0].size.a	= 0.2;
	temp[0].size.b	= 0.6;
	temp[0].size.exp= 4;
	temp[0].tex		= DrawingAssistant::T_XPL_RING;
	temp[0].x.a		= x;
	temp[0].x.exp	= 0.0;
	temp[0].y.a		= y;
	temp[0].y.exp	= 0.0;
	temp[0].z.a		= 0.0;
	temp[0].z.exp	= 0.0;

	for(int i=1; i<temp_length; i++)
	{
		temp[i].alpha	= 1.0;
		temp[i].color_b = 0.0;
		temp[i].color_g = 0.2;
		temp[i].color_r = 0.6;
		temp[i].fade	= 0.04;
		temp[i].life	= 0.0;
		temp[i].rotate	= (float)rand()/RAND_MAX * 0.02;
		temp[i].size.a	= 0.1;
		temp[i].size.b	= 0.3;
		temp[i].size.exp= 1;
		temp[i].tex		= DrawingAssistant::T_PARTICLE_CLOUD;
		temp[i].x.a		= x;
		temp[i].x.b		= x + (-0.5 + (float)rand()/RAND_MAX) * 0.6;
		temp[i].x.exp	= 1;
		temp[i].y.a		= y;
		temp[i].y.b		= y + (-0.5 + (float)rand()/RAND_MAX) * 0.6;
		temp[i].y.exp	= 0.0;
		temp[i].z.a		= 0.0;
		temp[i].z.exp	= 0.0;
	}


	addToBuffer(temp, temp_length);
}

void SpecialFXManager::ringOfGreen(float x, float y)
{
	int const temp_length = 10;
	Drawing temp[temp_length];

	temp[0].alpha	= 1.0;
	temp[0].color_b = 0.5;
	temp[0].color_g = 1.0;
	temp[0].color_r = 0.0;
	temp[0].fade	= 0.01;
	temp[0].life	= 0.0;
	temp[0].rotate	= 0.0;
	temp[0].size.a	= 0.2;
	temp[0].size.b	= 1.0;
	temp[0].size.exp= 4;
	temp[0].tex		= DrawingAssistant::T_XPL_RING;
	temp[0].x.a		= x;
	temp[0].x.exp	= 0.0;
	temp[0].y.a		= y;
	temp[0].y.exp	= 0.0;
	temp[0].z.a		= 0.0;
	temp[0].z.exp	= 0.0;

	for(int i=1; i<temp_length; i++)
	{
		temp[i].alpha	= 1.0;
		temp[i].color_b = 0.0;
		temp[i].color_g = 1.0;
		temp[i].color_r = 0.5;
		temp[i].fade	= 0.04;
		temp[i].life	= 0.0;
		temp[i].rotate	= (float)rand()/RAND_MAX * 0.02;
		temp[i].size.a	= 0.1;
		temp[i].size.b	= 0.3;
		temp[i].size.exp= 1;
		temp[i].tex		= DrawingAssistant::T_PARTICLE_CLOUD;
		temp[i].x.a		= x;
		temp[i].x.b		= x + (-0.5 + (float)rand()/RAND_MAX) * 0.6;
		temp[i].x.exp	= 1;
		temp[i].y.a		= y;
		temp[i].y.b		= y + (-0.5 + (float)rand()/RAND_MAX) * 0.6;
		temp[i].y.exp	= 0.0;
		temp[i].z.a		= 0.0;
		temp[i].z.exp	= 0.0;
	}


	addToBuffer(temp, temp_length);
}

void SpecialFXManager::explosion(float x, float y)
{
	Drawing temp[21];
	for(int i=0; i<sprites; i++)		// Common values
	{
		temp[i].alpha	= 1.0;
		temp[i].color_b = colors[rand()%12][2];
		temp[i].color_g = colors[rand()%12][1];
		temp[i].color_r = colors[rand()%12][0];
		temp[i].life	= 0.0;
		temp[i].fade	= 0.02;
		temp[i].rotate	= 0.0;
		temp[i].x.a		= x;
		temp[i].y.a		= y;
		temp[i].z.a		= 0;
	}
	for(int i=0; i<=particles;i++)		//Particles
	{
		temp[i].size.a	= 0.01 + 0.01 * (double)rand()/RAND_MAX ;
		temp[i].size.b	= 0.03 + 0.03 * (double)rand()/RAND_MAX;
		temp[i].alpha	= 0.8;
		temp[i].size.exp= 2;
		temp[i].x.b		= temp[i].x.a + 1 - (double)rand()/RAND_MAX * 2;
		temp[i].x.exp	= 3;
		temp[i].y.b		= temp[i].y.a + 1 - (double)rand()/RAND_MAX * 2;
		temp[i].y.exp	= 3;
		temp[i].z.b		= temp[i].z.a + 0.5 - (double)rand()/RAND_MAX;
		temp[i].z.exp	= 3;
		temp[i].tex		= DrawingAssistant::T_PARTICLE;
		temp[i].fade	= 0.016;

		temp[i+particles]			= temp[i];
		temp[i+particles].alpha		= 0.8;
		temp[i+particles].size.a	= 0.015;
		temp[i+particles].size.b	= 0.04;
		temp[i+particles].x.exp		= 2.75;
		temp[i+particles].y.exp		= 2.75;
		temp[i+particles].z.exp		= 2.75;
		temp[i+particles].tex		= DrawingAssistant::T_PARTICLE;

		temp[i+particles*2]			= temp[i];
		temp[i+particles*2].alpha	= 0.8;
		temp[i+particles*2].size.a	= 0.01;
		temp[i+particles*2].size.b	= 0.03;
		temp[i+particles*2].x.exp	= 2.5;
		temp[i+particles*2].y.exp	= 2.5;
		temp[i+particles*2].z.exp	= 2.5;
		temp[i+particles*2].tex		= DrawingAssistant::T_PARTICLE;

		//i+=trails;
	}

	temp[sprites-3].tex			= DrawingAssistant::T_XPL_RING;
	temp[sprites-3].alpha		= 1.0;
	temp[sprites-3].size.a		= 0.1;
	temp[sprites-3].size.b		= 0.3;
	temp[sprites-3].size.exp	= 3.0;
	temp[sprites-3].x.b			= temp[7].x.a;
	temp[sprites-3].x.exp		= 0.0;
	temp[sprites-3].y.b			= temp[7].y.a - 0.5;
	temp[sprites-3].y.exp		= 0.0;
	temp[sprites-3].z.b			= 0.0;
	temp[sprites-3].z.exp		= 0.0;
	temp[sprites-3].fade		= 0.03;

	temp[sprites-2].tex			= DrawingAssistant::T_XPL_BALL;
	temp[sprites-2].alpha		= 1.0;
	temp[sprites-2].size.a		= 0.2;
	temp[sprites-2].size.b		= 0.1;
	temp[sprites-2].size.exp	= 2.0;
	temp[sprites-2].x.b			= temp[8].x.a;
	temp[sprites-2].x.exp		= 0.0;
	temp[sprites-2].y.b			= temp[8].y.a - 0.5;
	temp[sprites-2].y.exp		= 0.0;
	temp[sprites-2].z.b			= 0.0;
	temp[sprites-2].z.exp		= 0.0;
	temp[sprites-2].fade		= 0.03;

	temp[sprites-1].tex			= DrawingAssistant::T_XPL_STAR;
	temp[sprites-1].alpha		= 1.0;
	temp[sprites-1].size.a		= 0.01;
	temp[sprites-1].size.b		= 0.5;
	temp[sprites-1].size.exp	= 4.0;
	temp[sprites-1].x.b			= temp[9].x.a;
	temp[sprites-1].x.exp		= 0.0;
	temp[sprites-1].y.b			= temp[9].y.a - 0.5;
	temp[sprites-1].y.exp		= 0.0;
	temp[sprites-1].z.b			= 0.0;
	temp[sprites-1].z.exp		= 0.0;
	temp[sprites-1].fade		= 0.03;
	temp[sprites-1].rotate		= 50.0;

	int j = 0;
	for(int i=0; i<MAX_FXDRAWINGS && j<sprites; i++)
	{
		if(drawings[i].life >= 1.0)
		{
			drawings[i] = temp[j++];
		}
	}

}

void SpecialFXManager::addToBuffer(Drawing temp[], int length)
{
	int j = 0;
	for(int i=0; i<MAX_FXDRAWINGS && j<length; i++)
	{
		if(drawings[i].life >= 1.0)
		{
			drawings[i] = temp[j++];
		}
	}
}
