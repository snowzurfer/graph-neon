// Lab3 - 30/09/2014
// Coder: Alberto Taiuti
//
// Purpose: Source for scene class

// Includes
#include "scene.h"


namespace winapp {

void Scene::initialise(HWND *lwnd, Input* in) {
	hwnd_ = lwnd;	// Access to the window
	input_ = in;	// Access to user input

	// Get window rect into our protected rect
	GetClientRect(*hwnd_, &screenRect_);	

	// Init openGL
	initOpenGL(screenRect_.right, screenRect_.bottom); // initialise openGL

	// OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_COLOR_MATERIAL);						// Turn on colour rendering manually
	
	// Initialise other variables
	rot0_ = 0;
	rot1_ = 0;
	rot2_ = 20;
	speed_ = 0.05* (1000 / 40);
}

void Scene::resize() {
	// If there is no handle to the window
	if(hwnd_ == NULL) {
		// Exit
		return;
	}

	// Get the new window size
	GetClientRect(*hwnd_, &screenRect_);

	// Resize the rendering context
	resizeGLWindow(screenRect_.right, screenRect_.bottom);
}

void Scene::update() {
	rot0_ += speed_;
	rot1_ += (speed_ * 2);
	rot2_ += (speed_ * 0.8);
}

void Scene::procInput() {
	// If M is pressed
	if(input_->isKeyDown(0x4D)) {
		// Create a variable to hold the status returned by the upcoming function
		GLint polygonMode[2];
		// Get the polygon mode being used
		glGetIntegerv(GL_POLYGON_MODE, polygonMode);

		// Depending on the previous mode, change the front face mode
		if(polygonMode[0] == GL_LINE) {
			glPolygonMode(GL_FRONT, GL_FILL);
		}
		else if(polygonMode[0] == GL_FILL) {
			glPolygonMode(GL_FRONT, GL_LINE);
		}

		// Set the key up so that it doesn't get processed again
		input_->setKeyUp(0x4D);
	}
}

void Scene::render(float interp) {
	// Clear the screen and depth buffer to render a new frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// ... And load the identity to clear the matrix
	glLoadIdentity();

	// Set camera looking down the -z axis, 3 units away from the center
	gluLookAt(0, 3, 15,     0, 0, 0,     0, 1, 0); // Where we are, What we look at, and which way is up

	

	// Save current matrix
	glPushMatrix();
		// Stretch the world to draw the arm of the robot
		glScalef(3, 1, 1);

		// Render the arm
		drawUnitCube();
	glPopMatrix();
	// Go back to previous matrix

	// Save current matrix
	glPushMatrix();
		// Move to the right top
		glTranslatef(3, 1, 0);

		// Rotate
		glRotatef(30, 0, 0, 1);

		// Move down-right
		glTranslatef(1, -1, 0);

		// Render hand
		drawUnitCube();
	glPopMatrix();
	// Go back to previous matrix

	// Swap the frame buffers (back with front)
	SwapBuffers(hdc_);
}

bool Scene::createPixelFormat(HDC hdc) { 
	// Create and empty px descriptor struct
    PIXELFORMATDESCRIPTOR pfd = {0}; 

	// Variable to hold the pixel format returned from win
	// (the closest, because windows)
    int pixelformat; 
	
	// Start defining the way we want our pixels
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Set the size of the structure
    pfd.nVersion = 1;							// Always set this to 1
	// Now pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;			// standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;				// RGB and Alpha pixel type
    pfd.cColorBits = COLOUR_DEPTH;				// Here we use our #define for the color bits
    pfd.cDepthBits = COLOUR_DEPTH;				// Ignored for RBA
    pfd.cAccumBits = 0;							// nothing for accumulation
    pfd.cStencilBits = 0;						// nothing for stencil
 
	// Gets a best match on the pixel format as passed in from device
	// and store it into a variable
    if ((pixelformat = ChoosePixelFormat(hdc, &pfd)) == false) { 
        MessageBox(NULL, reinterpret_cast<LPCSTR>("ChoosePixelFormat failed"), 
						reinterpret_cast<LPCSTR>("Error"), MB_OK |
												MB_ICONWARNING); 
		
		// Exit and report it
        return false; 
    } 
 
	// Attempt to set the pixel format if it is ok
    if (SetPixelFormat(hdc, pixelformat, &pfd) == false) { 
        MessageBox(NULL, reinterpret_cast<LPCSTR>("SetPixelFormat failed"), 
						reinterpret_cast<LPCSTR>("Error"), MB_OK |
												MB_ICONWARNING);
		
		// Exit and report it
        return false; 
    } 
 
	// We got here! Everything went well, communcate it
    return true;
}

void Scene::resizeGLWindow(int w, int h) {
	// Prevent a divide by zero error
	if (h == 0) {
		h = 1; // Make the height == 1
	}

	// Setup the viewport
	glViewport(0, 0, w, h);

	// Setup the matrix mode to projection
	glMatrixMode(GL_PROJECTION);
	// ...And load the identity to reset
	glLoadIdentity();

	// Calculate aspect ratio and set the frustum for clipping
	gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 1, 150.0);
	//glFrustum(screenRect_.left, screenRect_.right, screenRect_.bottom, screenRect_.top, 1.0f, 150.0f);
	//glFrustum(-2, 2, -2, 2, 1, 100);

	// Select the modelview matrix now
	glMatrixMode(GL_MODELVIEW);
	// Reset it
	glLoadIdentity();
}

void Scene::initOpenGL(int w, int h) {  
	// Get an HDC for the window
    hdc_ = GetDC(*hwnd_);

	// Attempt to create/set the pixel format
    if (!createPixelFormat(hdc_)) {
		// If failed, close the program
        PostQuitMessage (0);
	}

	// Creates hw rendering context from  hdc
    hrc_ = wglCreateContext(hdc_);
	// Use this HRC for wiggle
	wglMakeCurrent(hdc_, hrc_);		

	// Setup the window for the 1st time
	resizeGLWindow(w, h);	
}

void Scene::drawUnitCube() {
	// Being the drawing state
	glBegin (GL_TRIANGLES);

		// FRONT FACE
		glColor3f(1.0f, 0.0f, 0.0f);		// Red
		glVertex3f(-1.0f, 1.0f, 1.0f); 		// TLF
		
		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);		// BLF

		//glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);		// BRF

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);		// BRF
		
		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);		// TRF

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);		// TLF


		// RIGHT SIDE FACE
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);		// BRF
		
		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);		// BRB

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);		// TRB

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);		// BRF
		
		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);		// TRB

		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);		// TRF


		// BOTTOM SIDE FACE
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);		// BRF
		
		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);		// BLF

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);	// BLB

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);		// BRF

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);	// BLB

		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);		// BRB
		

		// TOP SIDE FACE
		glColor3f(0.0f, 0.5f, 0.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);		// TRF
		
		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);		// TRB

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);		// TLB

		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);		// TRF
		
		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);		// TLB

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);		// TLF


		// LEFT SIDE FACE
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);		// TLB

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);	// BLB

		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);		// BLF

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);		// TLB

		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);		// BLF

		//glColor3f(1.0f, 0.0f, 0.0f);		
		glVertex3f(-1.0f, 1.0f, 1.0f); 		// TLF


		// BACK SIDE FACE
		glColor3f(0.0f, 0.0f, 0.5f);
		glVertex3f(1.0f, 1.0f, -1.0f);		// TRB

		//glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);		// BRB

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);	// BLB

		//glColor3f(0.0f, 0.0f, 0.5f);
		glVertex3f(1.0f, 1.0f, -1.0f);		// TRB

		//glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);	// BLB

		//glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);		// TLB

	glEnd();
	// End drawing
}

}
// EO Namespace