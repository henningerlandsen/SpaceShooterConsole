#pragma once
#pragma intrinsic(tween)
#pragma intrinsic(pow)

#include <cmath>

using namespace std;

class TweenControl
{
public:

	typedef struct
	{
		float a;
		float b;
		float exp;
	}Tween;

	TweenControl();
	inline float tween(Tween t, float fraction);
	inline float tween(float a, float b, float exp, float fraction);
};

TweenControl::TweenControl()
{

}

inline float TweenControl::tween(TweenControl::Tween t, float fraction)
{
	if(t.exp == 0) return t.a;
	else return t.a + (1-pow( 1-fraction, t.exp )) * (t.b - t.a);
}

inline float TweenControl::tween(float a, float b, float exp, float fraction)
{
	if(exp == 0) return a;
	else return a + (1-pow( 1-fraction, exp )) * (b - a);
}