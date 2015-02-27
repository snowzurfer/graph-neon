#ifndef	_ENTITY_H
#define _ENTITY_H


// Includes
#include <map>
#include <lnfw/components/component.h>
#include <lnfw/physics/transform.h>
#include <list>
#include <lnfw/physics/Vector3/Vec3.h>

namespace lnfw {

class Entity
{
public:
	// Default Ctor
	Entity();

	// Ctor
	Entity(const int ID);

  // Dtor
  ~Entity();

	// Get a component depending on its type
	Component* getComp(const unsigned int type);

	// Attach a component
	void attachComp(Component *comp);

	// Detach a component depending on its type
	void detachComp(const unsigned int type);

	// Whether the entity has a component of a given type
	const bool hasComp(const unsigned int type) const;

	// Transform of the entity
	Transform<Vec3> transform;

  // Define a functor to delete components in a map
  struct DeleteComponentPointer_
  {
    void operator()(std::map<int, Component *>::value_type value) {
      delete value.second;
    }
  };

  // Add a child
  void addChild(Entity *child);

  inline const std::list<Entity *> &getChildrenList() const {
    return children_;
  }

  // Retrieve the parent of this entity
  inline Entity *getParent() {
    return parent_; 
  };

private:
	// Map of components attached to the entity
	std::map<unsigned int, Component *> comps_;

	// Unique ID of the entity
	int ID_;

  // Children of the entity
  std::list<Entity *> children_;

  // Parent entity
  Entity *parent_;
};
// EO Class

}
// EO Namespace

#endif