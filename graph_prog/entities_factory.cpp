
// Includes
#include <entities_factory.h>
#include <shapes_factory.h>
#include <lnfw/entities/entity.h>

namespace winapp {

  lnfw::Entity *EntitiesFactory::createCone() {
    // Create a shapes factory to create the shapes required
    ShapesFactory shapesFactory;

    ShapeComp *cubeShape = shapesFactory.buildCube(5);

    return new lnfw::Entity();
  }

}
// EO Namespace