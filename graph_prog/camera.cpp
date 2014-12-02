
// Includes
#include "camera.h"

namespace winapp {

Camera::Camera() : 
	position_(),
	rotation_(),
	forward_(),
	up_(),
	right_(),
	forwardSpeed_(0.f),
	lateralSpeed_(0.f)
	{
}

void Camera::handleInput(Input *input) {
	// Depending on key pressed
	if(input->isKeyPressed(kW)) {
		// Move forward
		forwardSpeed_ = kCameraMovementSpeed;
	}
	else if(input->isKeyPressed(kS)) {
		// Move backwards
		forwardSpeed_ = -kCameraMovementSpeed;
	}
	else {
		// Stop frontal movement
		forwardSpeed_ = 0.f;
	}

	// Depending on key pressed
	if(input->isKeyPressed(kA)) {
		// Move left laterally
		lateralSpeed_ = -kCameraMovementSpeed;
	}
	else if(input->isKeyPressed(kS)) {
		// Move right laterally
		lateralSpeed_ = kCameraMovementSpeed;
	}
	else {
		// Stop lateral movement
		lateralSpeed_ = 0.f;
	}
}

void Camera::update() {
	// Calculate the velovity for frontal movement
	Vec3 frontalVelocity(forward_.scale(forwardSpeed_));
	// Calculate the velocity for lateral movement
	Vec3 lateralVelocity(right_.scale(lateralSpeed_));

	// Integrate velocity into position
	position_ += (frontalVelocity + lateralVelocity);
}

void Camera::updateVectors() {
	update_();
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