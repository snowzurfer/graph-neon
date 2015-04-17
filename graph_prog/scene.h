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

  // Tutorial variables
  int s_wdith, s_height;
  float rot0_, rot1_, rot2_, speed_, cubeRot;
  Light light0;
  GLuint crateSolidTex_; // Texture
  GLuint crateTransparentTex_;
  Vec3 txPos_, txRot_, txScl_; // Texture position, rotation, scaling

  // Unit cube disp list
  GLuint unitCubeDList_;

	// Unit cube shape
  // cubeShape_;

  // Skybox
  //Skybox *skyBox_;

  // Camera
  Camera *camera_;
  
  // List of lights
  std::vector<Light *> lights_;

  // Entities list
  std::list<lnfw::Entity *> entities_;
  typedef std::list<lnfw::Entity *>::iterator entityItor_;
  typedef std::list<lnfw::Entity *>::const_iterator constEntityItor_;


  // Systems
  RenderingSys renderingSystem_;
  AnimatedTextureSys animatedTextureSys_;
  MovementSys movementSys_;
  ShadowingSys shadowingSys_;
  RaypickingSys *raypickingSys_;

  // For moving object
  unsigned int xpos, ypos;


  void DrawGLRoom()                           // Draw The Room (Box)
{
    glBegin(GL_QUADS);                      // Begin Drawing Quads
        // Floor
        glNormal3f(0.0f, 1.0f, 0.0f);               // Normal Pointing Up
        glVertex3f(-10.0f,-10.0f,-20.0f);           // Back Left
        glVertex3f(-10.0f,-10.0f, 20.0f);           // Front Left
        glVertex3f( 10.0f,-10.0f, 20.0f);           // Front Right
        glVertex3f( 10.0f,-10.0f,-20.0f);           // Back Right
        // Ceiling
        glNormal3f(0.0f,-1.0f, 0.0f);               // Normal Point Down
        glVertex3f(-10.0f, 10.0f, 20.0f);           // Front Left
        glVertex3f(-10.0f, 10.0f,-20.0f);           // Back Left
        glVertex3f( 10.0f, 10.0f,-20.0f);           // Back Right
        glVertex3f( 10.0f, 10.0f, 20.0f);           // Front Right
        // Front Wall
        glNormal3f(0.0f, 0.0f, 1.0f);               // Normal Pointing Away From Viewer
        glVertex3f(-10.0f, 10.0f,-20.0f);           // Top Left
        glVertex3f(-10.0f,-10.0f,-20.0f);           // Bottom Left
        glVertex3f( 10.0f,-10.0f,-20.0f);           // Bottom Right
        glVertex3f( 10.0f, 10.0f,-20.0f);           // Top Right
        // Back Wall
        glNormal3f(0.0f, 0.0f,-1.0f);               // Normal Pointing Towards Viewer
        glVertex3f( 10.0f, 10.0f, 20.0f);           // Top Right
        glVertex3f( 10.0f,-10.0f, 20.0f);           // Bottom Right
        glVertex3f(-10.0f,-10.0f, 20.0f);           // Bottom Left
        glVertex3f(-10.0f, 10.0f, 20.0f);           // Top Left
        // Left Wall
        glNormal3f(1.0f, 0.0f, 0.0f);               // Normal Pointing Right
        glVertex3f(-10.0f, 10.0f, 20.0f);           // Top Front
        glVertex3f(-10.0f,-10.0f, 20.0f);           // Bottom Front
        glVertex3f(-10.0f,-10.0f,-20.0f);           // Bottom Back
        glVertex3f(-10.0f, 10.0f,-20.0f);           // Top Back
        // Right Wall
        glNormal3f(-1.0f, 0.0f, 0.0f);              // Normal Pointing Left
        glVertex3f( 10.0f, 10.0f,-20.0f);           // Top Back
        glVertex3f( 10.0f,-10.0f,-20.0f);           // Bottom Back
        glVertex3f( 10.0f,-10.0f, 20.0f);           // Bottom Front
        glVertex3f( 10.0f, 10.0f, 20.0f);           // Top Front
    glEnd();                            // Done Drawing Quads
}

};
// EO Class

}
// EO Namespace

#endif