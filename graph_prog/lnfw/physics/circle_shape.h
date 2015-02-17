#ifndef	_CIRCLE_H
#define _CIRCLE_H

#include <maths\vector2.h>
#include "shape.h"

// Circle class
namespace lnfw {

class CircleShape : public IShape
{
private:
	// Radius
	float radius_;

	void addVertex(const abfw::Vector2& v) {};

public:
	// Ctor
	// NOTE
	//
	// The ctor shouldn't initialise the radius here: if the
	// client passes a negative or 0 radius, the object
	// creation shouldn't be allowed. It would be better
	// to have an init function to be called after the 
	// ctor. For obious reasons, better not to throw an exception
	// in the ctor! 
	//
	// EO NOTE
	CircleShape(float radius, const abfw::Vector2 &centre) : 
		IShape(centre),
		radius_(radius) {};

	virtual ~CircleShape() {};

	// Getters and setters
	inline const float getRadius() const { return radius_; };
	inline void setRadius(float val) { radius_ = val; };
};
// EO Class

}
// EO Namesfface

#endif
