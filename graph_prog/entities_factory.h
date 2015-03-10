#ifndef _ENTITIES_FACTORY_H
#define _ENTITIES_FACTORY_H

#include <lnfw/entities/entity.h>

namespace winapp {

  class EntitiesFactory
  {
  public:
    // Create a skybox
    lnfw::Entity *createSkyBox();
   
    // Create a basic cone
    lnfw::Entity *createCone();
  };

}
// EO Namespace

#endif