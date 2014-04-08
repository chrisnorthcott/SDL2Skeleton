#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <SDL2/SDL.h>
#include <vector>
#include "math.h"

#define GRAVITY		Vector2(0, -1);

class Physics 		//for tracking certain physics variables
{
public:
	Physics();
	void ApplyPhysics();
	float mass;
	float coefFriction;
	Vector2 acceleration;
	Vector2 appliedForce;
	Vector2 velocity;
	int wx, wy;		//world coordinates of object
};

class BoundingPolygon
{
public:
	BoundingPolygon();
	BoundingPolygon(Physics p, std::vector<Point> vertices);
	void DrawBoundingPolygon(SDL_Renderer *r);
	Physics p;
	std::vector<Point> vertices;
};

class ICollidable
{
public:
	ICollidable();
	ICollidable(Physics p, std::vector<ICollidable> collidableObjects);
	std::vector<ICollidable> Collides();
	bool CollidesWith(ICollidable ic);
	Physics p;
	BoundingPolygon bp;
};

class IGravity
{
public:
	IGravity();
	IGravity(Physics p);
	Physics p;
	void ApplyGravity();
};

#endif