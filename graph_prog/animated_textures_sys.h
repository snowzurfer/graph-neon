#ifndef _ANIMATED_TEXTURE_SYS_H
#define _ANIMATED_TEXTURE_SYS_H

// Includes
#include <lnfw/systems/system.h>
#include <list>

namespace winapp {

  class AnimatedTextureSys : public lnfw::System
  {
  public:

    // Render the entities if they have a renderer attached
    void update(const std::list<lnfw::Entity *> &entities);

  };
  // EO Class
}
// EO Namespace

#endif