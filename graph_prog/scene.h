#ifndef SCENE_H
#define SCENE_H

// Lab3 - 30/09/2014
// Coder: Alberto Taiuti
//
// Purpose: Header for scene class


// Includes
#include <Windows.h>
#include <MMSystem.h>
#include "input.h"
#include <math.h>
#include <gl/GL.h>	
#include <gl/GLU.h>
#include "light.h"
#include "camera.h"


namespace winapp {

// Colour depth
const int COLOUR_DEPTH = 16;	

class Scene 
{
public:
	// Ctor
	Scene();

	// Initialise the class
	void initialise(HWND*, Input*);
	
	// Process input
	void procInput();

	// Update the scene
	void update();

	// Render the scene on the back buffer
	void render(float interp);

	// Resize window
	void resize();

protected:
	// Create/setup the pixel format for the rendering context
	bool createPixelFormat(HDC);

	// Resize the window where oGL is being drawn
	void resizeGLWindow(int w, int h);

	// Init openGL
	void initOpenGL(int w, int h);

	// Set the buffers depending on device context
	void setBuffers();

	// Draw an unit cube at the centre of the screen
	void drawUnitCube();

	// Draw a plane composed of 2 triangles on the x-z plane
	void drawPlane(const float r, const float g, const float b);

	// HDC Used with the drawsprite function to manipulate/use the sprite
	HDC bitmapHDC_;

	// Win32 related variables
	HWND *hwnd_;					// Pointer to the window handle
	Input *input_;					// Pointer to the input object
	RECT screenRect_;				// Section of the screen to be drawn on
	HGLRC hrc_;						// Hardware rendering context
	HDC	hdc_;						// Device context for window being used

	// Tutorial variables
	int s_wdith, s_height;
	float rot0_, rot1_, rot2_, speed_, cubeRot;
	Light light0;

	// Camera
	Camera *camera_;
	


	// For moving object
	unsigned int xpos, ypos;

};
// EO Class

}
// EO Namespace

#endif