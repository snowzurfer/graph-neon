#ifndef _MANIFOLD_H
#define _MANIFOLD_H


// Includes
#include <maths\vector2.h>


namespace lnfw {

class GameObject;

class Manifold 
{
private:
	// Magnitude of minimum penetration vector
	float minMagnitude_;

	// Direction of the minimum penetration vector
	abfw::Vector2 minDirection_;

	// Pointers to the game objects
	GameObject *a_;
	GameObject *b_;

public:
	// Ctor
	Manifold(GameObject *a, GameObject *b) : a_(a), b_(b) {};

	// Getters and setters
	inline const float getMinMagnitude() const { return minMagnitude_; };
	inline void setMinMagnitude(float minMagnitude) { minMagnitude_ = minMagnitude; };
	inline const abfw::Vector2& getMinDirection() const { return minDirection_; };
	inline void setMinDirection(const abfw::Vector2& v) { minDirection_ = v; };
	inline GameObject* getAPtr() { return a_; };
	inline GameObject* getBPtr() { return b_; };

};
// EO Class

}
// EO Namespacezzzz

#endif