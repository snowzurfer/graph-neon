#ifndef _SPERE_CAP_SYS_H
#define _SPERE_CAP_SYS_H


// Includes
#include <lnfw/systems/system.h>
#include <list>


namespace winapp {

  class SphereCapSys : public lnfw::System
  {
  public:

    // Ctor
    SphereCapSys(std::list<lnfw::Entity *> &entitiesToDelete);

    // Update the entities if they have a movement component
    void update(const std::list<lnfw::Entity *> &entities);

    
  private:
    std::list<lnfw::Entity *> &entitiesToDelete_;
  };
  // EO Class

}
// EO Namespace

#endif