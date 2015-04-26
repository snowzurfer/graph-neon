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
  lights_(),
  shadowingSys_(lights_)
  {
  
};

Scene::~Scene() {
  // Free memory
  delete camera_;
  camera_ = NULL;

  delete raypickingSys_;
  raypickingSys_ = NULL;

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

  // Allocate a console to the application
 // AllocConsole();
  //freopen("CONOUT$", "w", stdout);

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


  // Create the raypicking system
  raypickingSys_ = new RaypickingSys(camera_, input_);

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
  light->setPosition(30.f, 30.f, 30.f, 1.0f); // Point light
  lights_.push_back(light);
  // Apply light modifications
  for(int i = 0; i < lights_.size(); ++i) {
    lights_[i]->apply();
  }

  // Create an entities factory
  EntitiesFactory entitiesFactory;

  // Create a shape builder
  ShapesFactory shapeBuilder;
  // Create a models loader
  ModelsLoader modelsLoader;

  //entities_.push_back(entitiesFactory.createMainRoom(lights_));

  lnfw::Entity *cone = entitiesFactory.createCone(lights_);
  cone->transform.position.set(10.f, 10.f, 10.f);
  cone->transform.rotation.set(5.f, 5.f, 5.f);
  entities_.push_back(cone);

  entities_.push_back(entitiesFactory.createBoxRoom());
  //entities_.push_back(entitiesFactory.createTeapot());
  //entities_.push_back(entitiesFactory.createMetallicDisk());
  //entities_.push_back(entitiesFactory.createSandTimer());
  //entities_.push_back(entitiesFactory.createRotatingArchane());

  

  


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

  // Update input
  input_->update();

  // Update camera
  camera_->update();

  // Update systems
  animatedTextureSys_.update(entities_);
  movementSys_.update(entities_);
  raypickingSys_->update(entities_);
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

  // Disable lights
  setLights_(false);
  // Render geometry
  renderingSystem_.update(entities_);
  // Re-enable lights
  setLights_(true);

  // Render shadows
  shadowingSys_.update(entities_);

  
  glEnable(GL_STENCIL_TEST);
  // Set the stencil to not change
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  // Set the stencil function
  glStencilFunc(GL_EQUAL, 0, 1);
  // Render lighted geometry
  renderingSystem_.update(entities_);
  // Disable stencil test
  glDisable(GL_STENCIL_TEST);
    



  // Save the position of the light
  GLvector4f lightPos = { 
    6,
    6,
    0,
    1};

        // Work variables
        GLmatrix16f Minv;
        GLvector4f wlp, lp;

        // Compute the position with respect to the entity's local
        // coordinates system
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        // Apply transformations in inverse order
       /* glRotatef(-transform.rotation.getZ(), 0.f, 0.f, 1.f);
    glRotatef(-transform.rotation.getY(), 0.f, 1.f, 0.f);
        glRotatef(-transform.rotation.getX(), 1.f, 0.f, 0.f);*/

        glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Retrieve ModelView Matrix From Minv
        lp[0] = lightPos[0];								// Store Light Position X In lp[0]
        lp[1] = lightPos[1];								// Store Light Position Y In lp[1]
        lp[2] = lightPos[2];								// Store Light Position Z In lp[2]
        lp[3] = lightPos[3];								// Store Light Direction In lp[3]
        vMat4Mult_(Minv, lp);									// Store Rotated Light Vector In 'lp' Array
      
        glTranslatef(0.f,
          -4.f,
          0.f);
        glGetFloatv(GL_MODELVIEW_MATRIX, Minv);				// Retrieve ModelView Matrix From Minv
        wlp[0] = 0.0f;										// World Local Coord X To 0
        wlp[1] = 0.0f;										// World Local Coord Y To 0
        wlp[2] = 0.0f;										// World Local Coord Z To 0
        wlp[3] = 1.0f;
        vMat4Mult_(Minv, wlp);								// Store The Position Of The World Origin Relative To The
                                              // Local Coord. System In 'wlp' Array
        lp[0] += wlp[0];									// Adding These Two Gives Us The
        lp[1] += wlp[1];									// Position Of The Light Relative To
        lp[2] += wlp[2];									// The Local Coordinate System
        glPopMatrix();

        // Create a light with position in the object's local coordinates
        Light workLight(0);
        workLight.setPosition(lp);



    // Matrix to store the projection matrix
    GLmatrix16f projMatrix;




    Vec3 normalLightShape(lp[0], 
      lp[1], 
      lp[2]);
    normalLightShape.normalize();
    normalLightShape = normalLightShape.scale(-1);

    // Compute the projection matrix
    generateShadowMatrix_(projMatrix, lp, normalLightShape, normalLightShape.scale(50));

   // //glDisable(GL_DEPTH_TEST);
   // glDisable(GL_LIGHTING);
   // glDisable(GL_TEXTURE_2D);
   // glColor3f(0.3f, 0.3f, 0.3f); // Shadow's color

   // // Apply the projection
   // //glMultMatrixf((GLfloat *) projMatrix);

   ///* GLvector4f testPt = {
   //   shapeComp->getVertices()[0].getX(),
   //   shapeComp->getVertices()[0].getY(),
   //   shapeComp->getVertices()[0].getZ(),
   //   1.f
   // };*/

   // //vMat4Mult_(projMatrix, testPt);

   // // Apply the projection
   // //glMultMatrixf((GLfloat *) projMatrix);


   // // Read matrix after the multiplication
   // GLmatrix16f result;
   // glGetFloatv(GL_MODELVIEW_MATRIX, result);

   // glPushMatrix();

   // //vMat4Mult_(result, testPt);

   // // Apply the geometry transformations before to apply shadowing, as the
   // // same way as in the rendering system
   // //setupRendering(&transform, shapeComp); 

   // /*BaseRendererComp *rendererComp = (BaseRendererComp *)(*entityitor)->
   // getComp(abfw::CRC::GetICRC("BaseRendererComp"))*/;

   // glTranslatef(0.f, 4.f, 0.f);

   // glMultMatrixf((GLfloat *) projMatrix);
   // 

   // // Render the object
   // gluSphere(gluNewQuadric(), 1.f, 10.f, 10.f);

   // glPopMatrix();

   // glPushMatrix();

   // //vMat4Mult_(result, testPt);

   // // Apply the geometry transformations before to apply shadowing, as the
   // // same way as in the rendering system
   // //setupRendering(&transform, shapeComp); 

   // /*BaseRendererComp *rendererComp = (BaseRendererComp *)(*entityitor)->
   // getComp(abfw::CRC::GetICRC("BaseRendererComp"))*/;

   // glTranslatef(0.f, 4.f, 0.f);

   // //glMultMatrixf((GLfloat *) projMatrix);
   // 

   // // Render the object
   // gluSphere(gluNewQuadric(), 1.f, 10.f, 10.f);

   // glPopMatrix();


   // glColor3f(1.0f, 1.0f, 1.0f);
   // //glEnable(GL_DEPTH_TEST);
   // glEnable(GL_LIGHTING);
   // glEnable(GL_TEXTURE_2D);




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
  gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.001, 300.0);
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

void Scene::setLights_(const bool setting) {
  for (int i = 0; i < lights_.size(); ++i) {
    lights_[i]->setEnabled(setting);
    lights_[i]->apply();
  }
}

}
// EO Namespace