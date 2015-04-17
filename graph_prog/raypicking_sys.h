#ifndef _RAYPICKING_SYS_H
#define _RAYPICKING_SYS_H


// Includes
#include <lnfw/systems/system.h>
#include <list>
#include <camera.h>


namespace winapp {

  class RaypickingSys : public lnfw::System
  {
  public:
    //Ctor
    RaypickingSys(const Camera *camera);

    // Update the entities if they have a movement component
    void update(const std::list<lnfw::Entity *> &entities);

  private:
    // Reference to the camera to obtain the ray
    const Camera *cam_;
  };
  // EO Class

}
// EO Namespace

#endif