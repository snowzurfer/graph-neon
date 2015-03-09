#ifndef _MOVEMENT_SYS_H
#define _MOVEMENT_SYS_H


// Includes
#include <lnfw/systems/system.h>
#include <list>


namespace winapp {

  class MovementSys : public lnfw::System
  {
  public:

    // Update the entities if they have a movement component
    void update(const std::list<lnfw::Entity *> &entities);
  };
  // EO Class

}
// EO Namespace

#endif