
// Includes
#include "cube_shape.h"

namespace winapp {

void CubeShape::drawShape() {
  // Front face
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0.f, 0.f, 0.5f); 
    planeShape_.drawShape();
  glPopMatrix();

  // Right face
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0.5f, 0.f, 0.f); glRotatef(90.f, 0.f, 1.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();

  // Back face
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0.f, 0.f, -0.5f); glRotatef(180.f, 0.f, 1.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();

  // Left face
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(-0.5f, 0.f, 0.f); glRotatef(-90.f, 0.f, 1.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();

  // Top face
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0.f, 0.5f, 0.0f); glRotatef(-90.f, 1.f, 0.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();

  // Bottom face
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0.f, -0.5f, 0.0f); glRotatef(90.f, 1.f, 0.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();
}

}
// EO Namespace