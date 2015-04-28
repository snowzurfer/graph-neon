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
  skyBox_(NULL),
  camera_(NULL),
  lights_(),
  shadowingSys_(lights_)
  {
  
};

Scene::~Scene() {
  delete sphereCapSys_;
  sphereCapSys_ = NULL;

  delete skyBox_;
  skyBox_ = NULL;

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

  // Create a default camera
  camera_ = new Camera(hwnd_, &screenRect_);
  camera_->setPos(Vec3(0.f, 0.f, 12.f));
  camera_->setUp(Vec3(0.f, 1.f, 0.f));
  camera_->setForward(Vec3(0.f, 0.f, -1.f));
  camera_->updateVectors();


  // Create the raypicking system
  raypickingSys_ = new RaypickingSys(camera_, input_, entitiesToAdd_);

  // Load skybox texture
  GLuint skyboxTxt = 0;
  skyboxTxt = SOIL_load_OGL_texture  (
                      "media/skybox_img.png",
                      SOIL_LOAD_AUTO,
                      SOIL_CREATE_NEW_ID,
                      SOIL_FLAG_MIPMAPS | 
                      SOIL_FLAG_NTSC_SAFE_RGB | 
                      SOIL_FLAG_COMPRESS_TO_DXT
  );

  // If the texture has been loaded
  if(skyboxTxt != 0) {
	  printf("SKYBOX TEXTURE LOADED");

    // Create the skybox
    skyBox_ = new Skybox(skyboxTxt);
  }

  // Setup lights
  Light *light = new Light(GL_LIGHT0, true);
  light->setPosition(-40.f, 44.f, -43.f, 1.f); // Point light
  light->setLinAttenuation(0.01f);
  //light->setConstAttenuation(100.f);
  lights_.push_back(light);
  light = new Light(GL_LIGHT1, false);
  light->setPosition(1.f, 1.f, 1.f, 1.0f); // Directional light
  light->setLinAttenuation(10.f);
  lights_.push_back(light);
  light = new Light(GL_LIGHT2, false);
  light->setPosition(108.f, 11.f, -3.f, 1.0f); // Point light, red
  light->setLinAttenuation(0.005f);
  light->setQuadAttenuation(0.001f);
  light->setAmbient(0.3f, 0.f, 0.f, 1.f);
  light->setDiffuse(1.f, 0.f, 0.f, 1.f);
  lights_.push_back(light);
  light = new Light(GL_LIGHT3, false);
  light->setPosition(45.f, 18.f, 65.f, 1.0f); // Spot light, gold yellow
  light->setSpotAngle(15.f);
  light->setSpotDirection(0.f, -0.09f, -1.f);
  light->setAmbient(0.4, 0.243137, 0.f, 1.f);
  light->setDiffuse(1.f, 0.843137f, 0.f, 1.f);
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

  entities_.push_back(entitiesFactory.createMainRoom(lights_));
  std::cout << "Loaded main room" << std::endl;

  lnfw::Entity *cone = entitiesFactory.createCone(lights_);
  cone->transform.position.set(-4.f, 18.f, -17.f);
  entities_.push_back(cone);


  entities_.push_back(entitiesFactory.createBoxRoom());
  //entities_.push_back(entitiesFactory.createTeapot());
  lnfw::Entity *mDisk = entitiesFactory.createMetallicDisk(Vec3(0.7f, 0.7f, 0.7f), 15.f);
  mDisk->transform.position.set(50.f, 35.f, -45.3f);
  entities_.push_back(mDisk);
  mDisk = entitiesFactory.createMetallicDisk(Vec3(0.6f, 0.6f, 0.9f), 15.f);
  mDisk->transform.position.set(65.f, 35.f, -45.2f);
  entities_.push_back(mDisk);
  mDisk = entitiesFactory.createMetallicDisk(Vec3(0.6f, 0.9f, 0.9f), 15.f);
  mDisk->transform.position.set(80.f, 35.f, -45.0f);
  entities_.push_back(mDisk);

  lnfw::Entity *skull = entitiesFactory.createSkull();
  skull->transform.position.set(103.424f, 10.116f, 6.526f);
  skull->transform.rotation.set(-20.f, -45.f, 0.f);
  skull->transform.scale.set(22.f, 22.f, 22.f);
  entities_.push_back(skull);

  // Create a sand timer and place it
  lnfw::Entity *sandTimer = entitiesFactory.createSandTimer();
  sandTimer->transform.position.set(42.5f, 25.f, 89.f);
  sandTimer->transform.rotation.set(0.f, 180.f, 0.f);
  entities_.push_back(sandTimer);

  lnfw::Entity *archane = entitiesFactory.createRotatingArchane();
  archane->transform.position.set(-16.f, -2.f, -28.f);
  archane->transform.scale.set(0.88f, 0.88f, 0.88f);
  entities_.push_back(archane);

  /*lnfw::Entity *testSphere = entitiesFactory.createMaterialSphere(Vec3(0.9f, 0.f, 0.f), 10.f);
  testSphere->transform.position.setY(50.f);
  entities_.push_back(testSphere);*/


  lnfw::Entity *chair = entitiesFactory.createChair();
  chair->transform.position.set(85.f, 20.f, 25.f);
  chair->transform.scale.set(13.f, 13.f, 13.f);
  entities_.push_back(chair);

  lnfw::Entity *ottoman = entitiesFactory.createOttoman();
  ottoman->transform.position.set(68.483f, 4.284f, -40.778f);
  ottoman->transform.scale.set(12.651f, 12.651f, 12.651f);
  entities_.push_back(ottoman);

  lnfw::Entity *desk = entitiesFactory.createDesk();
  desk->transform.position.set(78.614f, 0.746f, -1.495f);
  desk->transform.scale.set(9.f, 9.f, 9.f);
  desk->transform.rotation.set(0.f, 90.f, 0.f);
  entities_.push_back(desk);
  
  lnfw::Entity *pot = entitiesFactory.createPot();
  pot->transform.position.set(-4.433f, 1.f, 19.238f);
  pot->transform.scale.set(14.f, 14.f, 14.f);
  pot->transform.rotation.set(0.f, 45.f, 0.f);
  entities_.push_back(pot);


  lnfw::Entity *ceiling = entitiesFactory.createPlane(Vec3(0.8f, 0.8f, 0.8f));
  ceiling->transform.position.set(29.f, 58.f, 4.f);
  ceiling->transform.scale.set(200.f, 150.f, 1.f);
  ceiling->transform.rotation.set(90.f, 0.f, 0.f);
  entities_.push_back(ceiling);

  lnfw::Entity *bookShelf = entitiesFactory.createBookShelf();
  bookShelf->transform.position.set(9.383f, 1.f, 11.379f);
  bookShelf->transform.scale.set(12.f, 12.f, 12.f);
  bookShelf->transform.rotation.set(0.f, -45.f, 0.f);
  entities_.push_back(bookShelf);

  bookShelf = entitiesFactory.createBookShelf();
  bookShelf->transform.position.set(-24.243f, 1.f, 15.814f);
  bookShelf->transform.scale.set(12.f, 12.f, 12.f);
  bookShelf->transform.rotation.set(0.f, 264.771f, 0.f);
  entities_.push_back(bookShelf);

  bookShelf = entitiesFactory.createBookShelf();
  bookShelf->transform.position.set(28.919f, 1.f, -26.911f);
  bookShelf->transform.scale.set(12.f, 12.f, 12.f);
  bookShelf->transform.rotation.set(0.f, 13.908f, 0.f);
  entities_.push_back(bookShelf);


  sphereCapSys_ = new SphereCapSys(entitiesToDelete_);
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
  // Update input
  input_->update();

  // Update camera
  camera_->update();

  // Update systems
  animatedTextureSys_.update(entities_);
  movementSys_.update(entities_);
  raypickingSys_->update(entities_);
  sphereCapSys_->update(entities_);

  // If there are entities to be destroyed
  if(!entitiesToDelete_.empty()) {
    // Destroy entities
    std::for_each(entitiesToDelete_.begin(), entitiesToDelete_.end(), DeleteEntity_(entities_));

    // Empty the container
    entitiesToDelete_.clear();
  }

  // If there are entitites to be added
  if(!entitiesToAdd_.empty()) {
    // Destroy entities
    std::for_each(entitiesToAdd_.begin(), entitiesToAdd_.end(), AddEntity_(entities_));

    // Empty the container
    entitiesToAdd_.clear();
  }
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

  // Save current matrix
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

    // Translate to cam position
    Vec3 camPos = camera_->getPos();
    glTranslatef(camPos.getX(), camPos.getY(), camPos.getZ());

    // Render the skybox
    skyBox_->draw();

  glPopMatrix();

  // Apply the lights every frame after the viewing transform
  // multiplication
  for(int i = 0; i < lights_.size(); ++i) {
    lights_[i]->apply();
  }

  #ifdef _DEBUG
  for(int i = 0; i < lights_.size(); ++i) {
    // Render position of lights
    glPushMatrix();

    lights_[i]->draw();

    glPopMatrix();
  }
  #endif // _DEBUG

  


  // Disable lights
  setLights_(false);
  // Render geometry
  renderingSystem_.update(entities_);
  // Re-enable lights
  setLights_(true);

  //// Render shadows
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
  //GLvector4f lightPos = { 
  //  6,
  //  6,
  //  0,
  //  1};

  //      // Work variables
  //      GLmatrix16f Minv;
  //      GLvector4f wlp, lp;

  //      // Compute the position with respect to the entity's local
  //      // coordinates system
  //      glMatrixMode(GL_MODELVIEW);
  //      glPushMatrix();
  //      glLoadIdentity();

  //      // Apply transformations in inverse order
  //     /* glRotatef(-transform.rotation.getZ(), 0.f, 0.f, 1.f);
  //  glRotatef(-transform.rotation.getY(), 0.f, 1.f, 0.f);
  //      glRotatef(-transform.rotation.getX(), 1.f, 0.f, 0.f);*/

  //      glGetFloatv(GL_MODELVIEW_MATRIX,Minv);				// Retrieve ModelView Matrix From Minv
  //      lp[0] = lightPos[0];								// Store Light Position X In lp[0]
  //      lp[1] = lightPos[1];								// Store Light Position Y In lp[1]
  //      lp[2] = lightPos[2];								// Store Light Position Z In lp[2]
  //      lp[3] = lightPos[3];								// Store Light Direction In lp[3]
  //      vMat4Mult_(Minv, lp);									// Store Rotated Light Vector In 'lp' Array
  //    
  //      glTranslatef(0.f,
  //        -4.f,
  //        0.f);
  //      glGetFloatv(GL_MODELVIEW_MATRIX, Minv);				// Retrieve ModelView Matrix From Minv
  //      wlp[0] = 0.0f;										// World Local Coord X To 0
  //      wlp[1] = 0.0f;										// World Local Coord Y To 0
  //      wlp[2] = 0.0f;										// World Local Coord Z To 0
  //      wlp[3] = 1.0f;
  //      vMat4Mult_(Minv, wlp);								// Store The Position Of The World Origin Relative To The
  //                                            // Local Coord. System In 'wlp' Array
  //      lp[0] += wlp[0];									// Adding These Two Gives Us The
  //      lp[1] += wlp[1];									// Position Of The Light Relative To
  //      lp[2] += wlp[2];									// The Local Coordinate System
  //      glPopMatrix();

  //      // Create a light with position in the object's local coordinates
  //      Light workLight(0);
  //      workLight.setPosition(lp);



  //  // Matrix to store the projection matrix
  //  GLmatrix16f projMatrix;




  //  Vec3 normalLightShape(lp[0], 
  //    lp[1], 
  //    lp[2]);
  //  normalLightShape.normalize();
  //  normalLightShape = normalLightShape.scale(-1);

  //  // Compute the projection matrix
  //  generateShadowMatrix_(projMatrix, lp, normalLightShape, normalLightShape.scale(50));

  // // //glDisable(GL_DEPTH_TEST);
  // // glDisable(GL_LIGHTING);
  // // glDisable(GL_TEXTURE_2D);
  // // glColor3f(0.3f, 0.3f, 0.3f); // Shadow's color

  // // // Apply the projection
  // // //glMultMatrixf((GLfloat *) projMatrix);

  // ///* GLvector4f testPt = {
  // //   shapeComp->getVertices()[0].getX(),
  // //   shapeComp->getVertices()[0].getY(),
  // //   shapeComp->getVertices()[0].getZ(),
  // //   1.f
  // // };*/

  // // //vMat4Mult_(projMatrix, testPt);

  // // // Apply the projection
  // // //glMultMatrixf((GLfloat *) projMatrix);


  // // // Read matrix after the multiplication
  // // GLmatrix16f result;
  // // glGetFloatv(GL_MODELVIEW_MATRIX, result);

  // // glPushMatrix();

  // // //vMat4Mult_(result, testPt);

  // // // Apply the geometry transformations before to apply shadowing, as the
  // // // same way as in the rendering system
  // // //setupRendering(&transform, shapeComp); 

  // // /*BaseRendererComp *rendererComp = (BaseRendererComp *)(*entityitor)->
  // // getComp(abfw::CRC::GetICRC("BaseRendererComp"))*/;

  // // glTranslatef(0.f, 4.f, 0.f);

  // // glMultMatrixf((GLfloat *) projMatrix);
  // // 

  // // // Render the object
  // // gluSphere(gluNewQuadric(), 1.f, 10.f, 10.f);

  // // glPopMatrix();

  // // glPushMatrix();

  // // //vMat4Mult_(result, testPt);

  // // // Apply the geometry transformations before to apply shadowing, as the
  // // // same way as in the rendering system
  // // //setupRendering(&transform, shapeComp); 

  // // /*BaseRendererComp *rendererComp = (BaseRendererComp *)(*entityitor)->
  // // getComp(abfw::CRC::GetICRC("BaseRendererComp"))*/;

  // // glTranslatef(0.f, 4.f, 0.f);

  // // //glMultMatrixf((GLfloat *) projMatrix);
  // // 

  // // // Render the object
  // // gluSphere(gluNewQuadric(), 1.f, 10.f, 10.f);

  // // glPopMatrix();


  // // glColor3f(1.0f, 1.0f, 1.0f);
  // // //glEnable(GL_DEPTH_TEST);
  // // glEnable(GL_LIGHTING);
  // // glEnable(GL_TEXTURE_2D);




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
  pfd.cAccumBits = COLOUR_DEPTH;
 
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