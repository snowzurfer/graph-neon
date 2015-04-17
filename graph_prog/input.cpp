// Lab2 - 16/09/2014
// Coder: Alberto Taiuti
//
// Purpose: Source for input class

// Includes
#include "input.h"
#include <iostream>
#include <algorithm>

namespace winapp {


Input::Input() {
  // Initialise keys array
  for(int i = 0; i < KeysNum; i ++) {
    keys_[i] = prevKeys_[i] = realTimeKeys_[i] = 0;
  }
}

Input::~Input() {

}

void Input::update() {
  // Read from the real time keyboard
  std::copy(keys_, keys_ + KeysNum, prevKeys_);
  std::copy(realTimeKeys_, realTimeKeys_ + KeysNum, keys_);

  // Read mouse from the real time buttons feed
  _mouse.prevLeft = _mouse.left;
  _mouse.left = _mouse.realTimeLeft;
  _mouse.prevRight = _mouse.right;
  _mouse.right = _mouse.realTimeRight;

}

void Input::setKeyDown(WPARAM k) { 
  realTimeKeys_[k] = true;
}

void Input::setKeyUp(WPARAM k) {
  realTimeKeys_[k] = false;
}

bool Input::isKeyDown(int k) {
  return keys_[k];
}

bool Input::isKeyPressed(int k) {
  return ( (!prevKeys_[k]) && (keys_[k]) );
}

void Input::setMouseX(int X) {
  _mouse.x = X;
}

void Input::setMouseY(int Y) {
  _mouse.y = Y;
}

int Input::getMouseX() {
  return _mouse.x;
}

int Input::getMouseY() {
  return _mouse.y;
}

bool Input::isLDown() {
  return _mouse.left;
}

bool Input::isLPressed() {
  return ((_mouse.prevLeft ^ _mouse.left) & _mouse.left);
}


void Input::setLMouseBtn(UINT btn) {
  // Depending on btn state
  switch(btn) {
    // Down
    case WM_LBUTTONDOWN: {
      _mouse.realTimeLeft = true;
      break;
    }
    // Up
    case WM_LBUTTONUP: {
      _mouse.realTimeLeft = false;
      break;
    }
  }
}

}
// EO Namespace