#ifndef _CAMERA_H
#define _CAMERA_H

// Includes
#include "Vector3\Vec3.h"
#include <cmath>

namespace winapp {

const float kPi = 3.1415;
const float kPiOver180 = kPi / 180.f;
const float k180OverPi = 180.f / kPi;

class Camera
{
public:
	// Ctor
	Camera();

	// Dtor
	~Camera();

	// Handle input
	void handleInput();

	// Update camera based on input
	void update();


	// Getters and setters
	const Vec3 &getPos() const { return position_; }
	void setPos(const Vec3 &val) { position_ = val; }
	const Vec3 &getRotation() const { return rotation_; }
	void setRotation(const Vec3 &val) { rotation_ = val; }
	const Vec3 &getForward() const { return forward_; }
	void setForward(const Vec3 &val) { forward_ = val; }
	const Vec3 getLookAt() const;
	const Vec3 &getUp() const { return up_; }
	void setUp(const Vec3 &val) { up_ = val; }

private:
	// Position in world coordinates
	Vec3 position_;

	// Rotation.
	// x = pitch
	// y = yaw
	// z = roll
	Vec3 rotation_;

	// The direction the camera is pointed to
	Vec3 forward_;
	
	// Define where up is with respect to the camera
	Vec3 up_;
	
	// Define where right is with respect to the camera
	Vec3 right_;


	// Define speed of the camera
	float speed_;


	// Update values of camera after rotation has been modified
	void update_();
};
// EO Class

}
// EO Namespace

#endif