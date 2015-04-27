#ifndef _AABB_COMP_H
#define _AABB_COMP_H

// Includes
#include <lnfw\components\component.h>
#include <lnfw\physics\Vector3\Vec3.h>
#include <vector>


namespace lnfw {

	class AABBComp : public Component
	{
  public:

    REGISTER_COMPONENT(AABBComp);

    // Ctor
    AABBComp(const Vec3 &minLeft, const Vec3 &maxRight);
    // Second ctor
    AABBComp(const std::vector<Vec3> &vertices);

    // Dtor
    ~AABBComp() {};

    // Getters
    inline const Vec3 &getMinL() const {
      return minLeft_;
    }
    inline const Vec3 &getMaxR() const {
      return maxRight_;
    }

    inline const bool isClicked() const {
      return clicked_;
    }

    inline void setClicked(const bool val) {
      clicked_ = val;
    }

  private:
    // Top right and bottom left of the AABB
    Vec3 minLeft_;
    Vec3 maxRight_;

    // Whether the object has been clicked
    bool clicked_;
	};
  // EO Class

}
// EO Namespace


#endif