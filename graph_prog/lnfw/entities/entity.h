#ifndef	_ENTITY_H
#define _ENTITY_H


// Includes
#include <map>
#include <lnfw/components/component.h>
#include <lnfw/physics/transform.h>
#include <list>

namespace lnfw {

class Entity
{
public:
	// Default Ctor
	Entity();

	// Ctor
	Entity(const int ID);

	// Dtor
	virtual ~Entity();

	// Get a component depending on its type
	Component* getComp(int type) const;

	// Attach a component
	void attachComp(Component *comp);

	// Detach a component depending on its type
	void detachComp(const int type);

	// Whether the entity has a component of a given type
	const bool hasComp(const int type) const;

	// Transform of the entity
	Transform transform;

  // Add a child
  void addChild(Entity *child);

  // Retrieve the parent of this entity
  inline Entity *getParent() {
    return parent_; 
  };

private:
	// Map of components attached to the entity
	std::map<int, Component*> comps_;

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