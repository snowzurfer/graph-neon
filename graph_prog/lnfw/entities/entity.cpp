
// Includes
#include "entity.h"



namespace lnfw {


Entity::Entity() : comps_(), 
	ID_(-1) {
}

Entity::Entity(const int ID) : comps_(),
	ID_(ID) {
}

Component* Entity::getComp(const int type) const {
	// Declare an itor pointing to the element being searched
	std::map<int, Component*>::const_iterator itor = comps_.find(type);

	// If the component is attached to the entity
	if(itor != comps_.end()) {
		// Return it
		return itor->second;
	}

	// If not, return a NULL ptr
	return NULL;
}

void Entity::attachComp(Component *comp) {
	// If the component doesn't already exist
	if(comps_.find(comp->getType()) == comps_.end()) {
		// Add it
		comps_[comp->getType()] = comp;
	}
}

void Entity::detachComp(const int type) {
	// If the component is attached
	if(comps_.find(type) == comps_.end()) {
		// Remove it
		comps_.erase(type);
	}

	// Need to update the nodes and therefore, the systems too
}

const bool Entity::hasComp(const int type) const {
	// If the component is attached
	if(getComp(type) != NULL) {
		return true;
	}

	// If not
	return false;
}

}
// EO Namespace