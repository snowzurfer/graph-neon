#ifndef _SHAPE_BUILDER_H
#define _SHAPE_BUILDER_H

// Includes
#include <shape_comp.h>

namespace winapp {

  class ShapeBuilder
  {
  public:
    
    // Create a cube with a given level of detail (in power of 2)
    ShapeComp *buildCube(unsigned int detail);

    // TODO Create a cone

    // TODO Create a cylinder

    // TODO Create a sphere

    // Create a disk
    ShapeComp *buildDisk(unsigned int detail);

  private:



  };

}
// EO Namespace

#endif