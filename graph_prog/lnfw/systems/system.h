#ifndef	_ISYSTEM_H
#define _ISYSTEM_H

// Includes
#include <list>
#include <algorithm>
#include <lnfw/entities/entity.h>

namespace lnfw {

class System 
{
public:
	// Ctor
  System() {};

	// Dtor
	virtual ~System() {};

	// Update the system
	virtual void update(const std::list<Entity *> &entities) = 0;

protected:
	typedef std::list<lnfw::Entity *>::iterator entitiesItor_;
	typedef std::list<lnfw::Entity *>::const_iterator constEntitiesItor_;
};
// EO Class

}
// EO Namespace


#endif