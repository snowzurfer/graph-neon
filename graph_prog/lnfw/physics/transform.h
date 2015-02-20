#ifndef _TRANSFORM_H
#define _TRANSFORM_H


namespace lnfw {
  
  template <typename V>
  class Transform
  {
  public:
	  // Ctor
	  Transform() :
		  position(0.f, 0.f, 0.f),
		  scale(1.f, 1.f, 1.f),
		  rotation(0.f, 0.f, 0.f) {};

	  // Components of the transform
	  V position;
	  V scale;
	  V rotation;
  };
  // EO Class

}
// EO Namespace

#endif