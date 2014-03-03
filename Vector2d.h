#ifndef VECTOR2D_H
#define VECTOR2D_H
#include <cmath>

using namespace std;

// By Henning
// Standard 2D vector operations

class Vector2d
{

public:
	float _x, _y;

	Vector2d();
	Vector2d(float x, float y);
	Vector2d add(const Vector2d &b) const;
	Vector2d subtract(const Vector2d &b) const;
	float dot(const Vector2d &b) const;
	float magnitude();
	Vector2d normalize();
	Vector2d projectOnTo(const Vector2d &b) const;
	Vector2d normal();
	Vector2d multiply(const Vector2d &b) const;
};

Vector2d::Vector2d()
{
	_x = 0;
	_y = 0;
}

Vector2d::Vector2d(float x, float y)
{
	_x = x;
	_y = y;
}

Vector2d Vector2d::add(const Vector2d &b) const
{
	return Vector2d(_x + b._x, _y + b._y);
}

Vector2d Vector2d::subtract(const Vector2d &b) const
{
	return Vector2d(_x - b._x, _y - b._y);
}

float Vector2d::dot(const Vector2d &b) const
{
	return _x * b._x + _y * b._y;
}

float Vector2d::magnitude()
{
	return sqrt(_x*_x + _y*_y);
}

Vector2d Vector2d::normalize()
{
	float l = magnitude();
	if(l > 0)
	{
		return Vector2d(_x/l, _y/l);
	} 
	else
	{
		return Vector2d();
	}
}

Vector2d Vector2d::projectOnTo(const Vector2d &b) const
{
	float dp = dot(b);
	float px = ( dp / (b._x * b._x + b._y * b._y)) * b._x;
	float py = ( dp / (b._x * b._x + b._y * b._y)) * b._y;
	return Vector2d(px, py);
}

Vector2d Vector2d::normal()
{
	return Vector2d(_y, -_x);
}

Vector2d Vector2d::multiply(const Vector2d &b) const
{
	return Vector2d(_x * b._x, _y * b._y);
}

#endif //VECTOR2D_H