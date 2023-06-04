#pragma once

#include <box2d/box2d.h>
#include "ConstData.h"

class ContactListener
{
public:
	ContactListener() : listenerId(idCounter++) {}
	virtual ~ContactListener() {}

	ListenerId GetListenerId() { return listenerId; }

	virtual void OnCollisionEnter(b2Contact* contact) = 0;
	virtual void OnCollisionExit(b2Contact* contact) = 0;

private:
	ListenerId listenerId;

	static ListenerId idCounter;
};

