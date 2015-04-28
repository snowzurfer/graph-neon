#ifndef _RAYPICKING_SYS_H
#define _RAYPICKING_SYS_H


// Includes
#include <lnfw/systems/system.h>
#include <list>
#include <camera.h>
#include <aabb_comp.h>
#include <lnfw\physics\transform.h>
#include <input.h>

namespace winapp {

  class RaypickingSys : public lnfw::System
  {
  public:
    //Ctor
    RaypickingSys(const Camera *camera, Input *input, std::list<lnfw::Entity *> &entitiesToAdd);

    // Update the entities if they have a movement component
    void update(const std::list<lnfw::Entity *> &entities);

  private:
    // Reference to the camera to obtain the ray
    const Camera *cam_;

    // Reference to the input system
    Input *input_;

    // Check intersection of ray with AABB
    const bool testRayOOBB_(const Vec3 &rOrigin, const Vec3 &rDir, const lnfw::AABBComp *aabbComp,
      const lnfw::Transform<Vec3> *transform, float &intersectionDist);


    // Reference to the list of entities to be added
    std::list<lnfw::Entity *> &entitiesToAdd_;
  };
  // EO Class

}
// EO Namespace

#endif