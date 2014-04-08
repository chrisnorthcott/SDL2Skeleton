#include "physics.h"
#include <stdexcept>

Physics::Physics(){}

void Physics::ApplyPhysics()
{
	// float mass;
	// float coefFriction;
	// Vector2 acceleration;
	// Vector2 appliedForce;
	// Vector2 velocity;
	// int wx, wy;		//world coordinates of object
}

BoundingPolygon::BoundingPolygon()
{
	throw std::runtime_error("Not implemented");
}

BoundingPolygon::BoundingPolygon(Physics p, std::vector<Point> vertices)
{
	throw std::runtime_error("Not implemented");
}
void DrawBoundingPolygon(SDL_Renderer *r)
{
	throw std::runtime_error("Not implemented");
}
//Physics p;
//std::vector<Point> vertices;

ICollidable::ICollidable(){}

ICollidable::ICollidable(Physics p, std::vector<ICollidable> collidableObjects)
{
	throw std::runtime_error("Not implemented");
}

std::vector<ICollidable> ICollidable::Collides()
{
	throw std::runtime_error("Not implemented");
}
ICollidable ICollidable::CollidesWith(ICollidable ic)
{
	throw std::runtime_error("Not implemented");
}
bool ICollidable::CollidesWith(Line l)
{
	throw std::runtime_error("Not implemented");
}
bool ICollidable::CollidesWith(QuadraticCurve qc)
{
	throw std::runtime_error("Not implemented");}
// Physics p;
// BoundingPolygon bp;

IGravity::IGravity(){}

IGravity::IGravity(Physics p)
{
	throw std::runtime_error("Not implemented");
}
void ApplyGravity()
{
	throw std::runtime_error("Not implemented");
}