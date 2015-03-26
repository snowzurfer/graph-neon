#ifndef _ENTITIES_FACTORY_H
#define _ENTITIES_FACTORY_H

#include <lnfw/entities/entity.h>
#include <light.h>
#include <vector>

namespace winapp {

  class EntitiesFactory
  {
  public:
    // Create a skybox
    lnfw::Entity *createSkyBox();

    // Create a box room
    lnfw::Entity *createBoxRoom();
   
    // Create a basic cone
    lnfw::Entity *createCone(std::vector<Light *> &lights);

	  // Create a wizard room
	  lnfw::Entity *createMainRoom(std::vector<Light *> &lights);

    lnfw::Entity *createTexturedCube(std::vector<Light *> &lights);
  };

}
// EO Namespace

#endif