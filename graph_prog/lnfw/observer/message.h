#ifndef _MESSAGE_H
#define _MESSAGE_H


namespace lnfw {

class Message
{
public:
	// Ctor
	Message(const int type, const unsigned int stickData = 0) :
		type_(type),
		stickData_(stickData) {};

	// Dtor
	virtual ~Message() {};

	// Getters
	inline const int getType() const {
		return type_;
	};
	inline const unsigned int getStickData() const {
		return stickData_;
	};

protected:
	// Type of message
	int type_;

	// Stick data
	unsigned int stickData_;
};
// EO Class

}
// EO Namespace

#endif