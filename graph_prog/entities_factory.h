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

    // Create one of the two metallic disks
    lnfw::Entity *createMetallicDisk(const Vec3 &col, const float scale);

    // Create the sandtimer
    lnfw::Entity *createSandTimer();
   
    // Create the rotating archane
    lnfw::Entity *createRotatingArchane();

    // Create the rotating books
    lnfw::Entity *createRotatingBook();

    // Create a teapot
    lnfw::Entity *createTeapot();

    // Create a sandcone from the model
    lnfw::Entity *createSandCone();

    // Create a cubemapped-sphere of the given colour of the given radius
    lnfw::Entity *createMaterialSphere(const Vec3 &colour, const float radius);

    // Create a stone pillar
    lnfw::Entity *createStonePillar();

    // Create a chair
    lnfw::Entity *createChair();

    // Create 4 barrels
    lnfw::Entity *createOttoman();

    // Create a desk
    lnfw::Entity *createDesk();

    // Create a bookshelf
    lnfw::Entity *createBookShelf();

    // Create a skull
    lnfw::Entity *createSkull();

    // Create a cooking pot
    lnfw::Entity *createPot();

    // Create a plane
    lnfw::Entity *createPlane(const Vec3 &col);

    // Sphere for pot
    lnfw::Entity *createMaterialSpherePot();
  };

}
// EO Namespace

#endif