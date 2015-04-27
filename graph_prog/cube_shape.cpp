
// Includes
#include "cube_shape.h"

namespace winapp {

void CubeShape::drawShape() {
  // Front face
  glMatrixMode(GL_TEXTURE); // Set texture
  glPushMatrix();
    // Zoom into the texture
    glScalef(0.25f, 0.25f, 0.f);
    // Navigate to face
    glTranslatef(1.0f, 1.0f, 0.f);

  glMatrixMode(GL_MODELVIEW); // Set geometry
  glPushMatrix();
    glTranslatef(0.f, 0.f, 0.5f); 
    planeShape_.drawShape();
  glPopMatrix();

  glMatrixMode(GL_TEXTURE);
  glPopMatrix();

  // Right face
  glMatrixMode(GL_TEXTURE); // Set texture
  glPushMatrix();
    // Zoom into the texture
    glScalef(0.25f, 0.25f, 0.f);
    // Navigate to face
    glTranslatef(2.0f, 1.0f, 0.f);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0.5f, 0.f, 0.f); glRotatef(90.f, 0.f, 1.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();

  glMatrixMode(GL_TEXTURE);
  glPopMatrix();

  // Back face
  glMatrixMode(GL_TEXTURE); // Set texture
  glPushMatrix();
    // Zoom into the texture
    glScalef(0.25f, 0.25f, 0.f);
    // Navigate to face
    glTranslatef(3.0f, 1.0f, 0.f);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0.f, 0.f, -0.5f); glRotatef(180.f, 0.f, 1.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();

  glMatrixMode(GL_TEXTURE);
  glPopMatrix();

  // Left face
  glMatrixMode(GL_TEXTURE); // Set texture
  glPushMatrix();
    // Zoom into the texture
    glScalef(0.25f, 0.25f, 0.f);
    // Navigate to face
    glTranslatef(0.0f, 1.0f, 0.f);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(-0.5f, 0.f, 0.f); glRotatef(-90.f, 0.f, 1.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();

  glMatrixMode(GL_TEXTURE);
  glPopMatrix();

  // Top face
  glMatrixMode(GL_TEXTURE); // Set texture
  glPushMatrix();
    // Zoom into the texture
    glScalef(0.25f, 0.25f, 0.f);
    // Navigate to face
    glTranslatef(1.0f, 0.0f, 0.f);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0.f, 0.5f, 0.0f); glRotatef(-90.f, 1.f, 0.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();

  glMatrixMode(GL_TEXTURE);
  glPopMatrix();

  // Bottom face
  glMatrixMode(GL_TEXTURE); // Set texture
  glPushMatrix();
    // Zoom into the texture
    glScalef(0.25f, 0.25f, 0.f);
    // Navigate to face
    glTranslatef(1.0f, 2.0f, 0.f);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glTranslatef(0.f, -0.5f, 0.0f); glRotatef(90.f, 1.f, 0.f, 0.f);
    planeShape_.drawShape();
  glPopMatrix();

  glMatrixMode(GL_TEXTURE);
  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
}

}
// EO Namespace