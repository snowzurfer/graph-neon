
#include "display_list.h"

namespace winapp {

  DisplayListGen::DisplayListGen() {

  }

  DisplayListGen::~DisplayListGen() {

  }
  
  // Generate diplay lists
  GLuint DisplayListGen::generateDispList(FunctionName *myFunc) {

  }

  // Create unit cube
  GLuint DisplayListGen::genUnitCube() {
    // Create the display list
    GLuint dispListNum = glGenLists(1);

    // Compile the new list
    glNewList(dispListNum, GL_COMPILE);

      drawUnitCube();

    glEndList();
    // End compilation of list

    return dispListNum;
  }


}
// EO Namespace