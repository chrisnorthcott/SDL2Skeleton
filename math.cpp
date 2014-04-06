#include "math.h"
/*
	Returns the smallest value in an array of
	floats.
*/
inline float array_min(float *a)
{
	float t;
	int i, len = sizeof(a);
	t = a[0];
	for(i = 0; i < len; i++)
	{
		if(a[i] < t) t = a[i]; 	
	}
	return t;
}
/*
	Returns the largest value in an array of
	floats.
*/
inline float array_max(float *a)
{
	float t;
	int i, len = sizeof(a);
	t = a[0];
	for(i = 0; i < len; i++)
	{
		if(a[i] > t) t = a[i]; 	
	}
	return t;
}

/*
	Silicon Graphics' Fast Square Root implementation,
	supplanted with a hardware equivalent where available.

	See http://en.wikipedia.org/wiki/Fast_inverse_square_root
*/
inline double fsqrt(const double d)
{
#ifdef SSESQRT
	/*FIXME: division by 0 on AMD boxen*/
	float rec;
	__asm__ __volatile__ (
		"movss %1, %%xmm0\n"
		"rsqrtss %%xmm0, %%xmm1\n"
		"movss %%xmm1, %0"
		:"=m"(rec)
		:"m"(d)
		:"xmm0", "xmm1"
	);
	return 0.5f * (rec + 1.0f / (d * rec));
#else
	const float dh = d * 0.5f;
	union { float x; int i; } U;
	U.x = d;
	U.i = 1597463007 - (U.i >> 1);					//approximation step
	return d * U.x * (1.5f - dh * U.x * U.x);	//narrow it down (Newton's)
#endif
}

/*
	Fast Arcosine, in a similar fashion to the implementation
	above. Used in Vector2D::Angle(const Vector2D&) among others.
*/
inline double facos(const float x)
{
	union { float f; unsigned int b; } Z;
	Z.f = x;
	bool xN = (Z.b & 0x80000000);
	Z.b &= 0x7fffffff;
	float s = fsqrt(Z.f);
	float u = 1.57079632679489661923f -
		1.05199081698724154807f * Z.f;
	u = (Z.b < 0x3f400000) ? u : 2.f * fsqrt(1.f-s);
	return xN? 3.14159265358979323846f - u : u;	
}

/*
	Default Point constructor, to eliminate weird GCC errors
	when we let it use its own default.
*/
Point::Point()
{
	Point::x = 0;
	Point::y = 0;	
}

/*
	Same as above, specifying your own.
*/
Point::Point(float px, float py)
{
	Point::x = px;
	Point::y = py;	
}

/*
	Constructor for Line.
*/
Line::Line(Point& pStart, Point& pEnd)
{
	Line::start = Point(pStart.x, pStart.y);
	Line::end = Point(pEnd.x, pEnd.y);
}

/*
	Overloaded Line constructor, specifying start and end
	coordinates directly instead of via Point.
*/
Line::Line(float x0, float y0, float x1, float y1)
{
	Line::start = Point(x0, y0);
	Line::end = Point(x1, y1);	
}

/*
	I officially hate this function.
	Checks for intersection (crossing) with another Line
	using the algorithm A. LeMothe describes in "Tricks of 
	the	Windows Game Programming Gurus" pp 837-842
	I understand the math but to get the function working I
	have copied the algorithm verbatim for now with a view
	to using a more elegant solution with Vector2D/Matrix.
	Basically we compute 2 segment vectors S1 and S2.
	Then we stuff everything into "matrices" (use your
	imagination!) & use Cramer's rule to solve the 
	parametric equations

		U = p0 + tS1
		V = p2 + sS2

	for s and t to find the intersection point U=V between the two
	line segments. If these are between 0 and 1 there is an
	intersection & in this case the intersection point is put 
	into Line::intersection and this function returns true.

	<phew.>
*/
bool Line::Intersects(const Line& l)
{
	//LeMothe's algorithm
	//TODO: convert to use Vector class
	float p0_x = this->start.x,
	 	p0_y = this->start.y,
		p1_x = this->end.x,
		p1_y = this->end.y,
		p2_x = l.start.x,
		p2_y = l.start.y,
		p3_x = l.end.x,
		p3_y = l.end.y;
		
	//get segment vectors
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;
	s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;
	s2_y = p3_y - p2_x;
	
	//solve using Cramer's rule (Xi = Det(Ai) / Det(A))
	float s,t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) /
		(-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) /
		(-s2_x * s1_y + s1_x * s2_y);
		
	//if s/t are between 0 and 1 we have an intersection
	if(s>=0 && s<=1 && t >= 0 && t <= 1)
	{
		this->intersection = Point(
			p0_x + (s * s1_x),
			p0_y + (s * s1_y));
		return true;
	}
	
	return false;
}

/*
	Vector2 constructor.
*/
Vector2::Vector2(float vx, float vy)
{
	if(!vx) vx = 0;
	if(!vy) vy = 0;
	Vector2::x = vx;
	Vector2::y = vy;	
}

/*
	Addition
*/
Vector2 Vector2::operator+(const Vector2& v) const
{
	return Vector2(this->x + v.x, this->y + v.y);
}

/*
	Subtraction
*/
Vector2 Vector2::operator-(const Vector2& v) const
{
	return Vector2(this->x - v.x, this->y - v.y);	
}

/*
	Vector dot product
*/
float Vector2::operator*(const Vector2& v) const
{
	return (this->x * v.x) + (this->y * v.y);
}

/*
	Scalar multiplication
*/
Vector2 Vector2::operator*(const float f) const
{
	return Vector2(this->x * f, this->y * f);	
}

/*
	Length of vector
*/
float Vector2::Length()
{
	return fsqrt((this->x * this->x) + (this->y * this->y));	
}

/*
	Vector normal
*/
Vector2 Vector2::Normal()
{
	return Vector2(this->x / this->Length(),
					this->y / this->Length());
}
/*
	Perpendicular (this way only works in 2D, 3D needs
		crossproduct.)
*/
Vector2 Vector2::Perpendicular()
{
	return Vector2(-this->y, this->x);
}
/*
	Angle between two vectors.
*/
float Vector2::Angle(const Vector2& v)
{
	Vector2 u = *this;
	float dotProduct = u * v; 
	return facos(dotProduct);
}