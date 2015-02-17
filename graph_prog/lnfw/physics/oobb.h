#ifndef _OOBB_H
#define _OOBB_H


// Includes
#include <maths\vector2.h>
#include <vector>
#include <cassert>
#include "projection.h"
#include <cmath>
#include "manifold.h"
#include "shape.h"
#include "circle_shape.h"
#include <iostream>


namespace lnfw {


class OOBB : public IShape
{
public:
	// Ctor
	OOBB(float centreX, float centreY) :
		IShape(abfw::Vector2(centreX, centreY)),
		vertices_(),
		angle_(0.f) {};

	// Default ctor
	OOBB() :
		IShape(abfw::Vector2(0, 0)),
		vertices_(),
		angle_(0.f) {};

	// Copy ctor
	OOBB(const OOBB& rho) :
		IShape(rho.centre_),
		vertices_(rho.vertices_),
		angle_(rho.angle_) {};

	// Assignment op. overload
	inline OOBB& operator = (const OOBB& rho) {
		vertices_ = rho.vertices_;
		centre_ = rho.centre_;
		angle_ = rho.angle_;

		return *this;	
	};

	// Add a vertex to the shape
	void addVertex(const abfw::Vector2& v);

	// Rotate anticlockwise about the centre
	void setAngleAC(const float angle);

	// Getters and setters 
	inline const std::vector<abfw::Vector2> &getVertices() const { return vertices_; };
	inline const float getAngle() const { return angle_; };

	// Move the whole OOBB, vertices comprised
	void setPosition(const float x, const float y);

private:
	// Vertices of the geometry
	std::vector<abfw::Vector2> vertices_;
	typedef std::vector<abfw::Vector2>::iterator verticesItor_;

	// Angle, anticlockwise
	float angle_;	
};
// EO Class


/////////////////////////////////////////////
/// Functions related to the class but not
/// needed as class memebers
/////////////////////////////////////////////

// Create and return the right normal of (v)
inline abfw::Vector2 createRNormal(const abfw::Vector2& v) {
	return abfw::Vector2(v.y, -(v.x));
}

// Return the dot product of two vectors
inline const float dotProd(const abfw::Vector2& lho, const abfw::Vector2& rho) {
	return ((lho.x * rho.x) + (lho.y * rho.y));
}

// Normalise a vector
inline const abfw::Vector2 normalise(const abfw::Vector2& v) {
	float magnitude = std::sqrtf(v.x * v.x + v.y * v.y);

	return abfw::Vector2((v.x / magnitude), (v.y / magnitude));
}

inline const float magnitudeSqrd(const abfw::Vector2& v) {
	return ((v.x * v.x) + (v.y + v.y));
}

inline const float magnitude(const abfw::Vector2& v) {
	return std::sqrtf(magnitudeSqrd(v));
}

// Following Item 23 of Effective C++ (Meyers, 2005)
std::vector<abfw::Vector2> createNorms(const OOBB& oobb);
const Projection computeMinMax(const OOBB& lho, const OOBB& rho);
const bool SAT(const OOBB& a, const OOBB& b, Manifold& mfold);					// SAT for OOBB vs OOBB
const bool SAT(const OOBB& a, const CircleShape& b, Manifold& mfold);			// SAT for OOBB vs Circle
const bool SAT(const CircleShape& a, const OOBB& b, Manifold& mfold);			// Same as above, but allows to call with arguments swapped
const bool SAT(const CircleShape& a, const CircleShape& b, Manifold& mfold);	// SAT for Circle vs Circle
const bool SAT(const IShape &a, const IShape &b, Manifold& mfold);

//////////////////////////////////////////////

}
// EO Namespace

#endif