#ifndef	_ISHAPE_H
#define _ISHAPE_H

// Includes
#include <maths\vector2.h>

namespace lnfw {

class IShape 
{
public:
	// Ctor
	IShape(const abfw::Vector2 &v) :
		centre_(v) {};

	// Dtor
	virtual ~IShape() {};

	// Add a vertex to the shape
	virtual void addVertex(const abfw::Vector2& v) {} ; 

	// Getters and setters
	inline const abfw::Vector2& getCentre() const { return centre_; };
	inline void setCentre(const float x, const float y) { centre_.x = x; centre_.y = y; };
	inline void setCentre(const abfw::Vector2& v) { centre_ = v; };

	// Function to move the entire shape; centre and vertices
	virtual void setPosition(const float x, const float y) = 0;

protected:
	// Position of the centre in world (screen) coordinates
	abfw::Vector2 centre_;
};
// EO Class

}
// EO Namespace


#endif