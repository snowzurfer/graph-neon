
#include "display_list.h"

namespace winapp {

  DisplayListGen::DisplayListGen() {

  }

  DisplayListGen::~DisplayListGen() {

  }
  
  // Generate diplay lists
  GLuint DisplayListGen::generateDispList() {
    // TODO return 0;
    return 0;
  }

  // Create unit cube
  GLuint DisplayListGen::genUnitCube() {
    // Create the display list
    GLuint dispListNum = glGenLists(1);

    // Compile the new list
    glNewList(dispListNum, GL_COMPILE);

      

    glEndList();
    // End compilation of list

    return dispListNum;
  }


}
// EO Namespace