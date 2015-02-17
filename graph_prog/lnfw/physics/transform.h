#ifndef _TRANSFORM_H
#define _TRANSFORM_H


// Includes
#include "Vector3\Vec3.h"

namespace lnfw {

class Transform
{
public:
	// Ctor
	Transform() :
		position(0.f, 0.f, 0.f),
		scale(1.f, 1.f, 1.f),
		rotation(0.f, 0.f, 0.f) {};

	// Components of the transform
	Vec3 position;
	Vec3 scale;
	Vec3 rotation;
};
// EO Class

}
// EO Namespace

#endif