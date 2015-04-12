// Lab3 - 30/09/2014
// Coder: Alberto Taiuti
//
// Purpose: Source for scene class

// Includes
#include "scene.h"
#include <shapes_factory.h>
#include <shape_comp.h>
#include <lnfw/physics/transform.h>
#include <texture_comp.h>
#include <material_comp.h>
#include <vertex_renderer_comp.h>
#include <classic_renderer_comp.h>
#include <base_renderer_comp.h>
#include <animated_texture_comp.h>
#include <animated_textures_sys.h>
#include <models_loader.h>
#include <velocity_comp.h>
#include <shadow_comp.h>
#include <entities_factory.h>

namespace winapp {

Scene::Scene() : 
  rot0_(0.f),
  rot1_(0.f),
  rot2_(0.f),
  speed_(0.f),
  light0(GL_LIGHT0),
  //skyBox_(NULL),
  camera_(NULL),
  lights_()
  {
  
};

Scene::~Scene() {
  // Free memory
  delete camera_;
  camera_ = NULL;

  // Free up the entities list
  for(entityItor_ itor = entities_.begin(); itor != entities_.end(); ++itor) {
    lnfw::Entity *deleteThis = *itor;

    delete deleteThis;
  }
  entities_.clear();

  // Delete lights
  for(int i = 0; i < lights_.size(); ++i) {
    delete lights_[i];
  }
  lights_.clear();

}

void Scene::initialise(HWND *lwnd, Input* in) {
  hwnd_ = lwnd;  // Access to the window
  input_ = in;  // Access to user input

  // Get window rect into our protected rect
  GetClientRect(*hwnd_, &screenRect_);  

  // Init openGL
  initOpenGL(screenRect_.right, screenRect_.bottom); // initialise openGL

  // OpenGL settings
  glShadeModel(GL_SMOOTH);										                  // Enable Smooth Shading
  glClearColor(0.3809f, 0.501f, 0.58431f, 0.f);				
  glClearDepth(1.0f);											                      // Depth Buffer Setup
  glClearStencil(0x0);                                          // Stencil buffer setup
  glEnable(GL_DEPTH_TEST);										                  // Enables Depth Testing
  glDepthFunc(GL_LEQUAL);										                    // The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		        // Really Nice Perspective Calculations
  //glEnable(GL_COLOR_MATERIAL);								                // Turn on colour rendering manually
  glEnable(GL_LIGHTING);										                    // Enable lighting
  glEnable(GL_TEXTURE_2D);										                  // Enable 2D texturing
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	// Specify texturing mode
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Default texture behaviour
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			      // Set blending function
  glEnable(GL_CULL_FACE);										                    // Enable culling
  glCullFace(GL_BACK);											                    // Set it for the back faces
  glFrontFace(GL_CCW);                                          // Define default rendering direction


  // Initialise other variables
  rot0_ = 0;
  rot1_ = 0;
  rot2_ = 20;
  speed_ = 0.05* (1000 / 40);
  txPos_.set(0.f, 0.f, 0.f);
  txRot_.set(0.f, 0.f, 0.f);
  txScl_.set(1.f, 1.f, 1.f);

  // Create a default camera
  camera_ = new Camera(hwnd_, &screenRect_);
  camera_->setPos(Vec3(0.f, 0.f, 12.f));
  camera_->setUp(Vec3(0.f, 1.f, 0.f));
  camera_->setForward(Vec3(0.f, 0.f, -1.f));
  camera_->updateVectors();

  // Load crate texture
  crateSolidTex_ = SOIL_load_OGL_texture  (
                      "media/crate.png",
                      SOIL_LOAD_AUTO,
                      SOIL_CREATE_NEW_ID,
                      SOIL_FLAG_MIPMAPS | 
                      SOIL_FLAG_NTSC_SAFE_RGB | 
                      SOIL_FLAG_COMPRESS_TO_DXT
  );

  crateTransparentTex_ = SOIL_load_OGL_texture  (
                      "media/crateTrans.png",
                      SOIL_LOAD_AUTO,
                      SOIL_CREATE_NEW_ID,
                      SOIL_FLAG_MIPMAPS | 
                      SOIL_FLAG_NTSC_SAFE_RGB | 
                      SOIL_FLAG_COMPRESS_TO_DXT
  );

  if(crateSolidTex_ == 0) {
	  printf("ERROR LOADING");
  }

  if(crateTransparentTex_ == 0) {
	  printf("ERROR LOADING");
  }


  // Load skybox texture
  GLuint roomTexture = 0;
  roomTexture = SOIL_load_OGL_texture  (
                      "media/Models/wizard_house/wizardohouseTempTex2048.png",
                      SOIL_LOAD_AUTO,
                      SOIL_CREATE_NEW_ID,
                      SOIL_FLAG_MIPMAPS | 
                      SOIL_FLAG_NTSC_SAFE_RGB | 
                      SOIL_FLAG_COMPRESS_TO_DXT
  );

  // If the texture has been loaded
  if(roomTexture != 0) {
	  printf("SKYBOX TEXTURE LOADED");
    // Create the skybox
    //skyBox_ = new Skybox(skyboxTexture);
  }

  // Setup lights
  Light *light = new Light(GL_LIGHT0);
  light->setPosition(150.f, 150.f, 150.f, 1.0f); // Point light
  lights_.push_back(light);
  // Apply light modifications
  for(int i = 0; i < lights_.size(); ++i) {
    lights_[i]->apply();
  }

  // Create a entities factory
  EntitiesFactory entitiesFactory;

  // Create a shape builder
  ShapesFactory shapeBuilder;
  // Create a models loader
  ModelsLoader modelsLoader;

  //entities_.push_back(entitiesFactory.createMainRoom(lights_));

  lnfw::Entity *cone = entitiesFactory.createCone(lights_);
  cone->transform.position.set(10.f, 10.f, 10.f);
  //cone->transform.rotation.set(5.f, 5.f, 5.f);
  entities_.push_back(cone);

  entities_.push_back(entitiesFactory.createBoxRoom());
  entities_.push_back(entitiesFactory.createMetallicDisk());
  entities_.push_back(entitiesFactory.createSandTimer());
  

  

  


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
  rot1_ += (speed_ * 2.f);
  rot2_ += (speed_ * 0.8f);
  cubeRot += speed_;
  if(cubeRot > 360.f) {
    cubeRot = 0.f;
  };

  // Update camera
  camera_->update();

  // Update systems
  animatedTextureSys_.update(entities_);
  movementSys_.update(entities_);
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

  // Render geometry
  renderingSystem_.update(entities_);

  // Render shadows
  shadowingSys_.update(entities_);

  // Save current matrix
  /*glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

    // Translate to cam position
    Vec3 camPos = camera_->getPos();
    glTranslatef(camPos.getX(), camPos.getY(), camPos.getZ());

    // Render the skybox
    //skyBox_->draw();

  glPopMatrix();


  // Save current matrix
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
	// Translate cube
	//glTranslatef(0.f, 0.f, 0.f);

    // Rotate cube
    //glRotatef(cubeRot, 0.f, 1.f, 0.f);

	  // Set colour to transparent
	  //glColor4f(0.f, 0.f, 1.f, 0.25f);
	  // Activate blending
	  //glEnable(GL_BLEND);

    // Draw cube using display list
    //glCallList(unitCubeDList_);
    gluSphere(gluNewQuadric(), 0.20, 20,20);

	  // Deactivate blending
	  //glDisable(GL_BLEND);

  glPopMatrix();
  // Go back to previous matrix

  // Save current matrix
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();   

    // Translate cube
    glTranslatef(2.f, 2.f, 2.f);
    
    // Rotate cube
    glRotatef(cubeRot, 0.f, 1.f, 0.f);

    //glScalef(5.f, 5.f, 5.f);

	  // Activate blending
	  glEnable(GL_BLEND);

      // Draw cube
	    drawTexturedUnitCube(crateTransparentTex_);

	  // Deactivate blending
	  glDisable(GL_BLEND);

  glPopMatrix();*/
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
  pfd.cStencilBits = COLOUR_DEPTH;            // 8 Bits for stencil
 
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
 
  // We got here! Everything went well, communicate it
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
  gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.001, 150.0);
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

void Scene::drawTexturedUnitCube(GLuint texture) {
  // Bind texture to the geometry
  glBindTexture(GL_TEXTURE_2D, texture);

  // Set ST parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


  // Modify the texture
  glMatrixMode(GL_TEXTURE);

    glPushMatrix();

    glTranslatef(txPos_.getX(), txPos_.getY(), txPos_.getZ()); 
    glRotatef(txRot_.getX(), 1.f, 0.f, 0.f);
    glRotatef(txRot_.getY(), 0.f, 1.f, 0.f);
    glRotatef(txRot_.getZ(), 0.f, 0.f, 1.f);
    glScalef(txScl_.getX(), txScl_.getY(), txScl_.getZ()); 

  // Draw the shape and map texture
  glMatrixMode(GL_MODELVIEW);

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
    glTexCoord2f(1.f, 1.f);
    //glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF

    glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.f, 1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF
    
    glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF

    glNormal3f(0.f, 0.f, 1.f);
	glTexCoord2f(0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);    // TLF


    // RIGHT SIDE FACE
    glNormal3f(1.f, 0.f, 0.f);
	glTexCoord2f(0.f, 1.f);
    //glColor3f(0.5f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF
    
    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.f, 1.f);
    glVertex3f(1.0f, -1.0f, -1.0f);    // BRB

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 0.f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.f, 1.f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 0.f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.f, 0.f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF


    // BOTTOM SIDE FACE
    glNormal3f(0.f, -1.f, 0.f);
	glTexCoord2f(0.f, 0.f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF
    
    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.f, 0.f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 1.f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.f, 0.f);
    glVertex3f(1.0f, -1.0f, 1.0f);    // BRF

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 1.f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, -1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.f, 1.f);
    glVertex3f(1.0f, -1.0f, -1.0f);    // BRB
    

    // TOP SIDE FACE
    glNormal3f(0.f, 1.f, 0.f);
    glTexCoord2f(0.f, 0.f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF
    
    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.f, 1.f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 1.f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.f, 0.f);
    glVertex3f(1.0f, 1.0f, 1.0f);    // TRF
    
    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 1.f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(0.f, 1.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 0.f);
    glVertex3f(-1.0f, 1.0f, 1.0f);    // TLF


    // LEFT SIDE FACE
    glNormal3f(-1.f, 0.f, 0.f);
    glTexCoord2f(0.f, 0.f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.f, 1.f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.f, 1.f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.f, 0.f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.f, 1.f);
    glVertex3f(-1.0f, -1.0f, 1.0f);    // BLF

    glNormal3f(-1.f, 0.f, 0.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 0.f);
    glVertex3f(-1.0f, 1.0f, 1.0f);     // TLF


    // BACK SIDE FACE
    glNormal3f(0.f, 0.f, -1.f);
    glTexCoord2f(0.f, 0.f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.f, 1.f);
    glVertex3f(1.0f, -1.0f, -1.0f);    // BRB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 1.f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(0.0f, 0.0f, 0.5f);
	glTexCoord2f(0.f, 0.f);
    glVertex3f(1.0f, 1.0f, -1.0f);    // TRB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.f, 1.f);
    glVertex3f(-1.0f, -1.0f, -1.0f);  // BLB

    glNormal3f(0.f, 0.f, -1.f);
    //glColor3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.f, 0.f);
    glVertex3f(-1.0f, 1.0f, -1.0f);    // TLB

  glEnd();
  // End drawing

  // Modify the texture
  glMatrixMode(GL_TEXTURE);
    glPopMatrix();

  glMatrixMode(GL_MODELVIEW);

}

}
// EO Namespace