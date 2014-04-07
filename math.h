#ifndef _MATH2D_H
#define _MATH2D_H

#include <vector>

inline float array_min(float *a);
inline float array_max(float *a);
inline double fsqrt(const double d);
inline double facos(const float x);

class Point
{
	public:
		Point();
		Point(float, float);
		float x;
		float y;	
};

class QuadraticCurve
{
	public:
		QuadraticCurve(Point initial, Point middle, Point end);
		Point GetPointAt(float percentage);
		float Interpolate(float x, float y, float percentage);
		Point initial, middle, end;
};

class Line
{
	public:
		Line(Point&, Point&);
		Line(float, float, float, float);
		Point start;
		Point end;
		Point intersection;
		bool Intersects(const Line& l);
};

class Vector2
{
	public:
		Vector2();
		Vector2(float, float);
		Vector2 operator+(const Vector2& v) const;
		Vector2 operator-(const Vector2& v) const;
		float operator*(const Vector2& v) const;
		Vector2 operator*(const float f) const;
		float Length();
		Vector2 Normal();
		Vector2 Perpendicular();
		float Angle(const Vector2& v);
		float x;
		float y;
};


#endif