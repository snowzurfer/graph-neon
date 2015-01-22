
// Includes
#include "camera.h"

namespace winapp {

Camera::Camera(HWND *hwnd, RECT *screenRect) : 
  position_(),
  rotation_(),
  forward_(),
  up_(),
  right_(),
  forwardSpeed_(0.f),
  lateralSpeed_(0.f),
  screenRect_(screenRect),
  hwnd_(hwnd)
  {
}

Camera::~Camera() {

}

void Camera::handleInput(Input *input) {
  // Depending on key pressed
  if(input->isKeyDown(kW)) {
    // Move forward
    forwardSpeed_ = kCameraMovementSpeed;
  }
  else if(input->isKeyDown(kS)) {
    // Move backwards
    forwardSpeed_ = -kCameraMovementSpeed;
  }
  else {
    // Stop frontal movement
    forwardSpeed_ = 0.f;
  }

  // Depending on key pressed
  if(input->isKeyDown(kA)) {
    // Move left laterally
    lateralSpeed_ = -kCameraMovementSpeed;
  }
  else if(input->isKeyDown(kD)) {
    // Move right laterally
    lateralSpeed_ = kCameraMovementSpeed;
  }
  else {
    // Stop lateral movement
    lateralSpeed_ = 0.f;
  }

  // Calculate centre of window
  POINT winCentre;
  winCentre.x = (screenRect_->right - screenRect_->left) / 2;
  winCentre.y = (screenRect_->bottom - screenRect_->top) / 2;

  // Read mouse displacement with respect to the window client area
  int mouseXDisp = input->getMouseX() - winCentre.x;
  int mouseYDisp = - (input->getMouseY() - winCentre.y);

  // If the mouse has moved
  if(mouseXDisp != 0 || mouseYDisp != 0) {
    // Yaw
    rotation_.setY(rotation_.getY() + (mouseXDisp * kSecPerUpdate) * 0.001f);
    // Pitch
    rotation_.setX(rotation_.getX() + (mouseYDisp * kSecPerUpdate) * 0.001f);

    // Set mouse position back
    ClientToScreen(*hwnd_, &winCentre);
    SetCursorPos(winCentre.x, winCentre.y);

    // Update vectors
    updateVectors();
  }
  
}

void Camera::update() {
  // Calculate the velovity for frontal movement
  Vec3 frontalVelocity(forward_.scale(forwardSpeed_));
  // Calculate the velocity for lateral movement
  Vec3 lateralVelocity(right_.scale(lateralSpeed_));

  // Integrate velocity into position
  position_ += ((frontalVelocity + lateralVelocity).scale(kSecPerUpdate));
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
  right_ = forward_.cross(up_);
}

const Vec3 Camera::getLookAt() const {
  return Vec3(position_ + forward_);
}

}
// EO Namespace