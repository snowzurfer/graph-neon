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
#include "app_globals.h"
#include <SOIL.h>
//#include "cube_shape.h"
#include "skybox.h"
#include <list>
#include <lnfw/entities/entity.h>
#include <rendering_sys.h>
#include <animated_textures_sys.h>
#include <movement_sys.h>
#include <shadowing_sys.h>
#include <raypicking_sys.h>
#include <sphere_cap_sys.h>


namespace winapp {

// Colour depth
const int COLOUR_DEPTH = 16;  

class Scene 
{
public:
  // Ctor
  Scene();

  // Dtor
  ~Scene();

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


  // Disable/Enable lights, but not lighting
  void setLights_(const bool setting);

  // HDC Used with the drawsprite function to manipulate/use the sprite
  HDC bitmapHDC_;

  // Win32 related variables
  HWND *hwnd_;          // Pointer to the window handle
  Input *input_;          // Pointer to the input object
  RECT screenRect_;        // Section of the screen to be drawn on
  HGLRC hrc_;            // Hardware rendering context
  HDC  hdc_;            // Device context for window being used

  // Unit cube disp list
  GLuint unitCubeDList_;

	// Unit cube shape
  // cubeShape_;

  // Skybox
  Skybox *skyBox_;

  // Camera
  Camera *camera_;
  
  // List of lights
  std::vector<Light *> lights_;

  // Entities list
  std::list<lnfw::Entity *> entities_;
  typedef std::list<lnfw::Entity *>::iterator entityItor_;
  typedef std::list<lnfw::Entity *>::const_iterator constEntityItor_;

  std::list<lnfw::Entity *> entitiesToDelete_;
  // Define a functor to delete gameobjects
  struct DeleteEntity_ 
  {
    DeleteEntity_(std::list<lnfw::Entity *> &entities) :
    entities_(entities) {

    }

    void operator() (lnfw::Entity *entityToDelete) {
      // If the pointer points to a valid address
      if(entityToDelete != NULL) {
        entities_.erase(std::find(entities_.begin(), entities_.end(), entityToDelete));
        delete entityToDelete;
        entityToDelete = NULL;
      }
    }

    std::list<lnfw::Entity *> &entities_;

  };

  // List of entities to be added at the end of the frame
  std::list<lnfw::Entity *> entitiesToAdd_;
  struct AddEntity_ 
  {
    AddEntity_(std::list<lnfw::Entity *> &entities) :
    entities_(entities) {

    }

    void operator() (lnfw::Entity *entityToAdd) {
      // If the pointer points to a valid address
      if(entityToAdd != NULL) {
        entities_.push_back(entityToAdd);
      }
    }

    std::list<lnfw::Entity *> &entities_;

  };


  // Systems
  RenderingSys renderingSystem_;
  AnimatedTextureSys animatedTextureSys_;
  MovementSys movementSys_;
  ShadowingSys shadowingSys_;
  RaypickingSys *raypickingSys_;
  SphereCapSys *sphereCapSys_;

};
// EO Class

}
// EO Namespace

#endif