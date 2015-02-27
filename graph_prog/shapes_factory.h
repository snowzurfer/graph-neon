#ifndef _SHAPE_BUILDER_H
#define _SHAPE_BUILDER_H

// Includes
#include <shape_comp.h>

namespace winapp {

  class ShapesFactory
  {
  public:
    
    // Create a cube with a given level of detail (in power of 2)
    ShapeComp *buildCube(unsigned int detail);

    // Create a cone with a given level of detail (in number of vertices at base)
    ShapeComp *buildCone(unsigned int detail);

    // Create a cylinder
    ShapeComp *buildCylinder(unsigned int detail);

    // TODO Create a sphere

    // Create a disk with a given level of detail (in number of vertices)
    ShapeComp *buildDisk(unsigned int detail);
   
    // Create a plane with a given level of detail (in power of 2)
    ShapeComp *buildPlane(unsigned int detail);
    
  private:



  };

}
// EO Namespace

#endif