#ifndef _AABB_COMP_H
#define _AABB_COMP_H

// Includes
#include <lnfw\components\component.h>
#include <lnfw\physics\Vector3\Vec3.h>


namespace lnfw {

	class AABBComp : public Component
	{
  public:

    REGISTER_COMPONENT(AABBComp);

    // Ctor
    AABBComp(const Vec3 &minLeft, const Vec3 &maxRight);

    // Dtor
    ~AABBComp() {};

    // Getters
    inline const Vec3 &getMinL() const {
      return minLeft_;
    }
    inline const Vec3 &getMaxR() const {
      return maxRight_;
    }

  private:
    // Top right and bottom left of the AABB
    Vec3 minLeft_;
    Vec3 maxRight_;
	};
  // EO Class

}
// EO Namespace


#endif