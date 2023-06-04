#pragma once

#include <box2d/box2d.h>
#include <map>
#include <string>
#include <vector>

#include "ConstData.h"

#include "Bitmask.h"
#include "ContactListener.h"


class WorldContactListener : public b2ContactListener
{
public:
	~WorldContactListener();

	static void SetWorldPtr(b2World* worldPtr) { world = worldPtr; }

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;

	static void AddHandler(const Bitmask& bitmask, ContactListener* listener);
	static void DeleteHandler(const Bitmask& bitmask, ContactListener* listener);

	static void ClearRemovables();

	static void AddBodyRemovable(b2Fixture* fixture);
	static void ClearBodyRemovables();

private:
	static std::multimap<Bitmask, std::pair<ListenerId, ContactListener*>> handlers;

	static std::vector<std::multimap<Bitmask, std::pair<ListenerId, ContactListener*>>::iterator> handlersRemovables;
	static std::vector<b2Fixture*> bodyRemovables;

	static b2World* world;
};

