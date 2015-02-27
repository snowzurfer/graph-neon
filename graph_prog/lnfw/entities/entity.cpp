
// Includes
#include "entity.h"
#include <algorithm>


namespace lnfw {


  Entity::Entity() : 
    transform(),
    comps_(), 
	  ID_(-1),
    children_(),
    parent_(NULL) {
  }

  Entity::Entity(const int ID) :  transform(),
    comps_(), 
    ID_(ID),
    children_(),
    parent_(NULL) {
}

  Component* Entity::getComp(const unsigned int type) {
  // If the entity has the component of the given type
  if(hasComp(type)) {
    // Return the component
    return comps_[type];
  }
  // If not
  else {
    return NULL;
  }
}

  void Entity::attachComp(Component *comp) {
  // Assign the new component to the entity
  comps_[comp->GetComponentType()] = comp;

  UInt32 lol = comp->GetComponentType();

}

  void Entity::detachComp(const unsigned int type) {
	comps_.erase(comps_.find(type));
}

  const bool Entity::hasComp(const unsigned int type) const {
  // If the entity has the component of type type
  if(comps_.find(type) != comps_.end()) {
    return true;
  }
  // If not
  else {
    return false;
  }
}

  void Entity::addChild(Entity *child) {
  // If the child is not null
  if(child != NULL) {
    children_.push_back(child);
  }
}

  Entity::~Entity() {
    // Delete components
    std::for_each(comps_.begin(), comps_.end(), DeleteComponentPointer_());

    // Clear the container
    comps_.clear();
  }

}
// EO Namespace