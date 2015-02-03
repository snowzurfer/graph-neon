#ifndef _PLANE_SHAPE_H
#define _PLANE_SHAPE_H

#include "base_shape.h"


namespace winapp {

class PlaneShape : public BaseShape
{
public:
  // Realise the drawing of an unit plane on the X-Y plane, at the centre
  // of the screen (0.5 -> -0.5). It also get textured on the front face,
  // positive z-axis.
  virtual void drawShape();

};
// EO Class

}
// EO Namespace


#endif