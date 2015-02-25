#ifndef _RENDERING_SYS_H
#define _RENDERING_SYS_H


// Includes
#include <lnfw/systems/system.h>
#include <list>


namespace winapp {

  class RenderingSys : public lnfw::System
  {
  public:

    // Render the entities if they have a renderer attached
    void render(const std::list<lnfw::Entity *> &entities);
  };
  // EO Class

}
// EO Namespace

#endif