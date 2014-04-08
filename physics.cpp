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
	BoundingPolygon::vertices = vertices;
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
	//Implementation of SAT algorithm
	// See http://en.wikipedia.org/wiki/Hyperplane_separation_theorem
}

std::vector<ICollidable> ICollidable::Collides()
{
	throw std::runtime_error("Not implemented");
}
bool ICollidable::CollidesWith(ICollidable ic)
{
	Vector2 axis;
	double aMin, aMax, bMin, bMax;
	int edge, i;
	int nVertCollider = ic.bp.vertices.size();
	int nVertCollidee = this->bp.vertices.size();
	//loop over Collidee's edges
	for(edge = 0; edge < nVertCollidee; edge++ )
	{
		//get projection axis
		if(edge == 0)
		{
			axis = Vector2(
				this->bp.vertices[nVertCollidee - 1].y - this->bp.vertices[0].y,
				this->bp.vertices[0].x - this->bp.vertices[nVertCollidee - 1].x);
		}else{
			axis = Vector2(
				this->bp.vertices[edge - 1].y - this->bp.vertices[edge].y,
				this->bp.vertices[edge].x - this->bp.vertices[edge - 1].x);
		}

		/* Normalise the axis */
		axis.Normalise();

		/* Do the projection for collidee and get its bounds:*/
		Vector2 projectionPoint = Vector2(this->bp.vertices[0].x, this->bp.vertices[0].y);
		aMin = aMax = axis * projectionPoint;
		for(i = 1; i < nVertCollidee; i++)
		{
			double dotProd = axis * Vector2( this->bp.vertices[i].x, this->bp.vertices[i].y );
			if(dotProd > aMax)
				aMax = dotProd;
			else if(dotProd < aMin)
				aMin = dotProd;
		}

		/* Project collider onto the same axis... */
		projectionPoint = Vector2(ic.bp.vertices[0].x, ic.bp.vertices[0].y);
		bMin = bMax = axis * projectionPoint;
		for(i = 1; i < nVertCollider; i++)
		{
			double dotProd = axis * Vector2( ic.bp.vertices[i].x, ic.bp.vertices[i].y );
			if(dotProd > aMax)
				aMax = dotProd;
			else if(dotProd < aMin)
				aMin = dotProd;
		}

		/* and look to see if the mins/maxes overlap. If they don't
			there is guaranteed to be no collision */
		if(aMax < bMax || aMin > bMin)
		{
			return false;
		}
	}
	//Lather rinse repeat!
	//This time, the other way round: project Collider through Collidee.
	for(edge = 0; edge < nVertCollider; edge++ )
	{
		//get projection axis
		if(edge == 0)
		{
			axis = Vector2(
				ic.bp.vertices[nVertCollider - 1].y - ic.bp.vertices[0].y,
				ic.bp.vertices[0].x - ic.bp.vertices[nVertCollider - 1].x);
		}else{
			axis = Vector2(
				ic.bp.vertices[edge - 1].y - ic.bp.vertices[edge].y,
				ic.bp.vertices[edge].x - ic.bp.vertices[edge - 1].x);
		}

		/* Normalise the axis */
		axis.Normalise();

		/* Do the projection for collider and get its bounds:*/
		Vector2 projectionPoint = Vector2(ic.bp.vertices[0].x, ic.bp.vertices[0].y);
		aMin = aMax = axis * projectionPoint;
		for(i = 1; i < nVertCollider; i++)
		{
			double dotProd = axis * Vector2( ic.bp.vertices[i].x, ic.bp.vertices[i].y );
			if(dotProd > aMax)
				aMax = dotProd;
			else if(dotProd < aMin)
				aMin = dotProd;
		}

		/* Project collidee onto the same axis... */
		projectionPoint = Vector2(this->bp.vertices[0].x, this->bp.vertices[0].y);
		bMin = bMax = axis * projectionPoint;
		for(i = 1; i < nVertCollidee; i++)
		{
			double dotProd = axis * Vector2( this->bp.vertices[i].x, this->bp.vertices[i].y );
			if(dotProd > aMax)
				aMax = dotProd;
			else if(dotProd < aMin)
				aMin = dotProd;
		}

		/* and look to see if the mins/maxes overlap. If they don't
			there is guaranteed to be no collision */
		if(aMax < bMax || aMin > bMin)
		{
			return false;
		}
	}
	// if we reach here: collision!
	return true;
}
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