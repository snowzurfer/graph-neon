
// Includes
#include "camera.h"

namespace winapp {

Camera::Camera() : 
	position_(),
	rotation_(),
	forward_(),
	up_(),
	right_(),
	speed_(20.f)
	{
}

void Camera::update_() {
	// Calculate this values once for performance's sake
	float cosY = std::cosf(rotation_.getY() * k180OverPi);
	float cosP = std::cosf(rotation_.getX() * k180OverPi);
	float cosR = std::cosf(rotation_.getZ() * k180OverPi);
	float sinY = std::sinf(rotation_.getY() * k180OverPi);
	float sinP = std::sinf(rotation_.getX() * k180OverPi);
	float sinR = std::sinf(rotation_.getZ() * k180OverPi);

	// Calculate the forward vector
	forward_.setX(sinY * cosP);
	forward_.setY(sinP);
	forward_.setZ(cosP * -cosY);

	// Calculate the up vector
	up_.setX(-cosY * sinR - sinY * sinP * cosR);
	up_.setY(cosP * cosR);
	up_.setZ(-sinY * sinR - sinP * cosR * -cosY);

	// Calculate the right vector
	right_ = up_.cross(forward_);
}

const Vec3 Camera::getLookAt() const {
	return Vec3(position_ + forward_);
}

}
// EO Namespace