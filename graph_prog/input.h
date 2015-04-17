#ifndef INPUT_H
#define INPUT_H

// Lab2 - 16/09/2014
// Coder: Alberto Taiuti
//
// Purpose: Header for input class

// Includes
#include <windows.h>

// Constants

namespace winapp {

// Number of keys to keep track of
const int KeysNum = 256;

const int kW = 0x57;
const int kA = 0x41;
const int kS = 0x53;
const int kD = 0x44;
const int kZ = 0x5A;
const int kX = 0x58;


class Input
{
public:
  // Default constructor
  Input();

  // Destructor
  ~Input();

  // Update
  void update();

  // Keyboard related
  void setKeyDown(WPARAM k);
  void setKeyUp(WPARAM k);
  bool isKeyDown(int k);
  bool isKeyPressed(int k);

  // Mouse related
  void setMouseX(int X);
  void setMouseY(int Y);
  int getMouseX();
  int getMouseY();
  bool isLDown();
  bool isLPressed();
  void setLMouseBtn(UINT btn);
  void setRMouseBtn(UINT btn);

private:
  // Array of current read
  bool keys_[KeysNum];

  // Array of previous read
  bool prevKeys_[KeysNum];

  // Array of real-time pressed keys
  bool realTimeKeys_[KeysNum];

  // Define a structure to hold mouse data
  struct Mouse 
  {
    int x, y;
    bool left, right, prevLeft, prevRight, realTimeLeft, realTimeRight;

    Mouse(): x(0), y(0), left(false), right(false),
    prevLeft(false), prevRight(false),
    realTimeLeft(false), realTimeRight(false)
    {};
  } _mouse; // Also define an instance of the mouse struct

};

}
// EO Namespace

#endif