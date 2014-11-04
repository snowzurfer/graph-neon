// Lab2 - 16/09/2014
// Coder: Alberto Taiuti
//
// Purpose: Source for input class

// Includes
#include "input.h"


namespace winapp {


Input::Input() {
	// Initialise keys array
	for(int i = 0; i < KeysNum; i ++) {
		keys_[i] = 0;
		justPressedKeys_[i] = 0;
	}
}

Input::~Input() {

}

void Input::setKeyDown(WPARAM k) {
	// If the key has just been pressed
	if(keys_[k] == false) {
		// Set the just pressed key too
		justPressedKeys_[k] = true;
	}
	else {
		// Set the just pressed key too
		justPressedKeys_[k] = false;
	}
	
	keys_[k] = true;
}

void Input::setKeyUp(WPARAM k) {
	keys_[k] = false;

	// Set the just pressed key too
	justPressedKeys_[k] = false;
}

bool Input::isKeyDown(int k) {
	return keys_[k];
}

bool Input::isKeyPressed(int k) {
	return justPressedKeys_[k];
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

void Input::setLMouseBtn(WPARAM btn) {
	// Depending on btn state
	switch(btn) {
		// Down
		case WM_LBUTTONDOWN: {
			_mouse.left = true;

			break;
		}
		// Up
		case WM_LBUTTONUP: {
			_mouse.left = false;

			break;
		}
	}
}

}
// EO Namespace