#ifndef	_ENTITY_H
#define _ENTITY_H


// Includes
#include <map>
#include "../components/component.h"
#include "../physics/transform.h"

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

	// Child of the entity

private:
	// Map of components attached to the entity
	std::map<int, Component*> comps_;

	// Unique ID of the entity
	int ID_;
};
// EO Class

}
// EO Namespace

#endif