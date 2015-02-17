#ifndef _GAMESTATE_H
#define _GAMESTATE_H


// Includes
#include <lnfw\observer\subject.h>


namespace lnfw {


class GameState
{
public:
	// Ctor
	GameState(const int stateID, const int nextStateID) :
		subject(),
		stateID_(stateID),
		nextStateID_(nextStateID),
		changeState_(false) {};

	// Dtor
	virtual ~GameState() {};

	virtual void init() = 0;

	virtual void cleanup() = 0;

	virtual bool update(float ticks) = 0;

	virtual void render() = 0;


	// Subject used to send messages to observer statemanagers
	Subject subject;


	// Getters and setters
	inline const int getNextStateID() const {
		return nextStateID_;
	};
	void setNextStateID(const int nextStateID) {
		nextStateID_ = nextStateID;
	};
	inline const int getStateID() const {
		return stateID_;
	};
	void setStateID(const int stateID) {
		stateID_ = stateID;
	};

protected:
	// ID of the state
	int stateID_;

	// The ID of the next state
	int nextStateID_;

	// Whether the manager should change state or not
	bool changeState_;
};
// EO Class

}
// EO Namespace

#endif