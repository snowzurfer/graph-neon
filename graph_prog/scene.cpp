// Lab3 - 30/09/2014
// Coder: Alberto Taiuti
//
// Purpose: Source for scene class

// Includes
#include "scene.h"


namespace winapp {

Scene::Scene() : 
  rot0_(0.f),
  rot1_(0.f),
  rot2_(0.f),
  speed_(0.f),
  light0(GL_LIGHT0),
  camera_(NULL)
  {
  
};

Scene::~Scene() {
  // Free memory
  delete camera_;
  camera_ = NULL;
}

void Scene::initialise(HWND *lwnd, Input* in) {
  hwnd_ = lwnd;  // Access to the window
  input_ = in;  // Access to user input

  // Get window rect into our protected rect
  GetClientRect(*hwnd_, &screenRect_);  

  // Init openGL
  initOpenGL(screenRect_.right, screenRect_.bottom); // initialise openGL

  // OpenGL settings
  glShadeModel(GL_SMOOTH);              // Enable Smooth Shading
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);        // Black Background
  glClearDepth(1.0f);                  // Depth Buffer Setup
  glEnable(GL_DEPTH_TEST);              // Enables Depth Testing
  glDepthFunc(GL_LEQUAL);                // The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations
  glEnable(GL_COLOR_MATERIAL);            // Turn on colour rendering manually
  glEnable(GL_LIGHTING);                // Enable lighting
  glEnable(GL_TEXTURE_2D);              // Enable 2D texturing
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // Specify texturing mode
  
  // Initialise other variables
  rot0_ = 0;
  rot1_ = 0;
  rot2_ = 20;
  speed_ = 0.05* (1000 / 40);

  // Create a default camera
  camera_ = new Camera(hwnd_, &screenRect_);
  camera_->setPos(Vec3(4.f, 0.f, 12.f));
  camera_->setUp(Vec3(0.f, 1.f, 0.f));
  camera_->setForward(Vec3(0.f, 0.f, -1.f));
  camera_->updateVectors();

  // Load crate texture
  texture_ = SOIL_load_OGL_texture  (
                      "media\\crate.png",
                      SOIL_LOAD_AUTO,
                      SOIL_CREATE_NEW_ID,
                      SOIL_FLAG_MIPMAPS | 
                      SOIL_FLAG_NTSC_SAFE_RGB | 
                      SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(texture_ == 0) {
	  printf("ERROR LOADING");
  }

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
  cubeRot += speed_;
  if(cubeRot > 360.f) {
    cubeRot = 0.f;
  };

  // Update camera
  camera_->update();
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

  // Send camera input so that it can update itself
  camera_->handleInput(input_);
}

void Scene::render(float interp) {
  // Clear the screen and depth buffer to render a new frame
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // ... And load the identity to clear the matrix
  glLoadIdentity();

  {
    // Get vectors for gluLookAt from camera
    Vec3 camPos = camera_->getPos();
    Vec3 lookAt = camera_->getLookAt();
    Vec3 up    = camera_->getUp();

    // Where we are, What we look at, and which way is up
    gluLookAt(camPos.getX(), camPos.getY(), camPos.getZ(),
          lookAt.getX(), lookAt.getY(), lookAt.getZ(), 
          up.getX(),   up.getY(),    up.getZ()); 

  }

  // Define a directional light
  /*GLfloat light0Ambient_[] = {0.3f, 0.3f, 0.3f, 1.0f};
  GLfloat light0Diffuse_[] =   {1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat light0Position_[]=   {-1.0f, 0.8f, 0.0f, 0.0f};*/

  // Define a point light
  light0.setPosition(-3.0f, 1.f, 5.0f, 1.0f);

  // Apply light modifications
  light0.apply();

  // Save current matrix
  glPushMatrix();
    // Rotate cube
    glRotatef(cubeRot, 0.f, 1.f, 0.f);

    // Draw cube
    drawUnitCube();

  glPopMatrix();
  // Go back to previous matrix

  // Save current matrix
  glPushMatrix();   

    // Translate cube
    glTranslatef(4.f, 0.f, 0.f);
    
    // Rotate cube
    glRotatef(cubeRot, 0.f, 1.f, 0.f);

    // Draw cube
    drawTexturedUnitCube();

  glPopMatrix();
  // Go back to previous matrix


  // Save current matrix
  glPushMatrix();   

  glBindTexture(GL_TEXTURE_2D, texture_);	//tells opengl which texture to use
	glBegin (GL_QUADS);	//Begin drawing state
		glNormal3f(  0.0f,  0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f,  1.0f, 0.0f);		
		glNormal3f( 0.0f,  0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f( -1.0f, -1.0f, 0.0f);
		glNormal3f( 0.0f,  0.0f, 1.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, 0.0f);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f( 1.0f, 1.0f, 0.0f);
	glEnd();		//end drawing


  glPopMatrix();
  // Go back to previous matrix


  // Save current matrix
  glPushMatrix();
    /*glTranslatef(light0Position_[0], light0Position_[1], light0Position_[2]);

    glColor3f(1, 1, 1);
    gluSphere(gluNewQuadric(), 0.3f, 20.f, 20.f);*/
  glPopMatrix();
  // Go back to previous matrix

  // Save current matrix
  glPushMatrix();
    // Move the plane
    glTranslatef(2.f, 3.f, 1.f);

    // Draw cube
    drawPlane(1, 1, 1);

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
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);  // Set the size of the structure
    pfd.nVersion = 1;              // Always set this to 1
  // Now pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;      // standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;        // RGB and Alpha pixel type
    pfd.cColorBits = COLOUR_DEPTH;        // Here we use our #define for the color bits
    pfd.cDepthBits = COLOUR_DEPTH;        // Ignored for RBA
    pfd.cAccumBits = 0;              // nothing for accumulation
    pfd.cStencilBits = 0;            // nothing for stencil
 
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
    glNormal3f(0.f, 0.f, 1.f);
    glColor3f(1.0f, 0.0f, 0.0f);    // Red
    glVertex3f(-1.0f, 1.0f, 1.0f);     // TLF
    
    glNormal3f(0.f, 0.f, 1.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(0.f, 0.f, 1.f);
    //glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF

    glNormal3f(0.f, 0.f, 1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF
    
    glNormal3f(0.f, 0.f, 1.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF

    glNormal3f(0.f, 0.f, 1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);    // TLF


    // RIGHT SIDE FACE
    glNormal3f(1.f, 0.f, 0.f);
    glColor3f(0.5f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF
    
    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);    // BRB

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF


    // BOTTOM SIDE FACE
    glNormal3f(0.f, -1.f, 0.f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF
    
    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);    // BRB
    

    // TOP SIDE FACE
    glNormal3f(0.f, 1.f, 0.f);
    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF
    
    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF
    
    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);    // TLF


    // LEFT SIDE FACE
    glNormal3f(-1.f, 0.f, 0.f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);    
    glVertex3f(-1.0f, 1.0f, 1.0f);     // TLF


    // BACK SIDE FACE
    glNormal3f(0.f, 0.f, -1.f);
    glColor3f(0.0f, 0.0f, 0.5f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);    // BRB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(0.0f, 0.0f, 0.5f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

  glEnd();
  // End drawing
}

void Scene::drawTexturedUnitCube() {
  // Bind texture to the geometry
  glBindTexture(GL_TEXTURE_2D, texture_);
  // Being the drawing state
  glBegin (GL_TRIANGLES);

    // FRONT FACE
    glNormal3f(0.f, 0.f, 1.f);
    glTexCoord2f(0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);    // Red
    glVertex3f(-1.0f, 1.0f, 1.0f);     // TLF
    
    glNormal3f(0.f, 0.f, 1.f);
    glTexCoord2f(0.f, 1.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(0.f, 0.f, 1.f);
    glTexCoord2f(1.f, 0.f);
    //glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF

    glNormal3f(0.f, 0.f, 1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF
    
    glNormal3f(0.f, 0.f, 1.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF

    glNormal3f(0.f, 0.f, 1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);    // TLF


    // RIGHT SIDE FACE
    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(0.5f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF
    
    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);    // BRB

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF


    // BOTTOM SIDE FACE
    glNormal3f(0.f, -1.f, 0.f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF
    
    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);    // BRB
    

    // TOP SIDE FACE
    glNormal3f(0.f, 1.f, 0.f);
    glColor3f(0.0f, 0.5f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF
    
    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF
    
    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);    // TLF


    // LEFT SIDE FACE
    glNormal3f(-1.f, 0.f, 0.f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);    
    glVertex3f(-1.0f, 1.0f, 1.0f);     // TLF


    // BACK SIDE FACE
    glNormal3f(0.f, 0.f, -1.f);
    glColor3f(0.0f, 0.0f, 0.5f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);    // BRB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(0.0f, 0.0f, 0.5f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

  glEnd();
  // End drawing
}

void Scene::drawPlane(const float r, const float g, const float b) {
  // Begin drawing
  glBegin(GL_TRIANGLES);
    // Setup colour
    glColor3f(r, g, b);


    glNormal3f(0.f, 1.f, 0.f);
    glVertex3f(-0.5f, 0.f, -0.5f);    // TL
    
    glNormal3f(0.f, 1.f, 0.f);
    glVertex3f(-0.5f, 0.f, 0.5f);    // BL

    glNormal3f(0.f, 1.f, 0.f);
    glVertex3f(0.5f, 0.0f, 0.5f);    // BR

    glNormal3f(0.f, 1.f, 0.f);
    glVertex3f(-0.5f, 0.f, -0.5f);    // TL
    
    glNormal3f(0.f, 1.f, 0.f);
    glVertex3f(0.5f, 0.0f, 0.5f);    // BR

    glNormal3f(0.f, 1.f, 0.f);
    glVertex3f(0.5f, 0.0f, -0.5f);    // TR

  glEnd();
  // End drawing
}

}
// EO Namespace