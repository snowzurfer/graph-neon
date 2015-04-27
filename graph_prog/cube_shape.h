#ifndef _CUBE_SHAPE_H
#define _CUBE_SHAPE_H

#include "shape_comp.h"
#include "plane_shape.h"
#include "app_globals.h"

namespace winapp {

class CubeShape
{
public:
  // Realise the drawing of an unit cube centred on the origin of axis
  // of side 0.5. Uses the plane shape to draw its sides
  virtual void drawShape();

private:
  // Plane shape renderer
  PlaneShape planeShape_;

};
// EO Namespace

}
// EO Namespace

#endif