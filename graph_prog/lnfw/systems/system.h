#ifndef	_ISYSTEM_H
#define _ISYSTEM_H

// Includes
#include <list>
#include <algorithm>
#include "../game_object.h"

namespace lnfw {

class ISystem 
{
public:
	// Ctor
	ISystem(const int type) :
		type_(type) {};

	// Dtor
	virtual ~ISystem() {};

	// Update the system and relative nodes
	virtual void update(std::list<GameObject *> entities) = 0;

protected:
	typedef std::list<lnfw::GameObject *>::iterator gameObjsItor_;
	typedef std::list<lnfw::GameObject *>::const_iterator gameObjsConstItor_;

	// Type of component treated by the system
	int type_;
};
// EO Class

}
// EO Namespace


#endif