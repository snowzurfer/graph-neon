
// Includes
#include "plane_shape.h"

namespace winapp {

void PlaneShape::drawShape() {
  // Begin drawing
  glBegin(GL_TRIANGLES);

    glNormal3f(0.f, 0.f, 1.f); glTexCoord2f(0.f, 0.f); glVertex3f(-0.5f, -0.5f, 0.f);    // TL
    glNormal3f(0.f, 0.f, 1.f); glTexCoord2f(0.f, 1.f); glVertex3f(-0.5f, 0.5f, 0.f);    // BL
    glNormal3f(0.f, 0.f, 1.f); glTexCoord2f(1.f, 1.f); glVertex3f(0.5f, 0.5f, 0.0f);    // BR
    glNormal3f(0.f, 0.f, 1.f); glTexCoord2f(0.f, 0.f); glVertex3f(-0.5f, -0.5f, 0.f);    // TL
    glNormal3f(0.f, 0.f, 1.f); glTexCoord2f(1.f, 1.f); glVertex3f(0.5f, 0.5f, 0.f);    // BR
    glNormal3f(0.f, 0.f, 1.f); glTexCoord2f(1.f, 0.f); glVertex3f(0.5f, -0.5f, 0.f);    // TR

  glEnd();
  // End drawing

}


}
// EO Namespace