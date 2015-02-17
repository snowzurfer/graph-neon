#ifndef _SUBJECT_H
#define _SUBJECT_H

// Includes
#include <list>
#include <algorithm>
#include "observer_component.h"
#include "message.h"

namespace lnfw {

class Subject
{
public:
	// Ctor
	Subject();

	// Add an observer
	void addObserver(ObserverComponent *observer);

	// Remove an observer
	void removeObserver(ObserverComponent *observer);

	// Send a message to the observers
	void sendMessage(lnfw::Message *message) const;

protected:
	// List of observers to send messages to
	std::list<ObserverComponent *> observers_;

	// Define two handy types for the sake of readability
	typedef std::list<ObserverComponent *>::iterator observersItor;
	typedef std::list<ObserverComponent *>::const_iterator constObserversItor;
};
// EO Class

}
// EO Namespace

#endif