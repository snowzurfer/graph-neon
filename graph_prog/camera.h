#ifndef _CAMERA_H
#define _CAMERA_H

// Includes
#include "Vector3\Vec3.h"
#include <cmath>
#include "input.h"
#include "app_globals.h"
#include <Windows.h>
#include <iostream>
#include <cstdio>

namespace winapp {

const float kPi = 3.1415f;
const float kPiOver180 = kPi / 180.f;
const float k180OverPi = 180.f / kPi;
const float kCameraMovementSpeed = 5.f;


class Camera
{
public:
  // Ctor
  // screenRect is a reference to size of window from scene
  Camera(HWND *hwnd, RECT *screenRect);

  // Dtor
  ~Camera();

  // Handle input
  void handleInput(Input *input);

  // Update camera based on input
  void update();

  // Re-calculate the vectors after a rotation has
  // happened.
  void updateVectors();

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
  float forwardSpeed_;
  float lateralSpeed_;

  // Update values of camera after rotation has been modified
  void update_();

  // Size of the window; used to correctly set mouse position
  RECT *screenRect_;
  // Handle to the window
  HWND *hwnd_;
};
// EO Class

}
// EO Namespace

#endif