#include "WorldContactListener.h"

std::multimap<Bitmask, std::pair<ListenerId, ContactListener*>> WorldContactListener::handlers = {};
std::vector<std::multimap<Bitmask, std::pair<ListenerId, ContactListener*>>::iterator> WorldContactListener::handlersRemovables = {};
std::vector<b2Fixture*> WorldContactListener::bodyRemovables = {};

b2World* WorldContactListener::world = {};

WorldContactListener::~WorldContactListener()
{
	handlers.clear();
	handlersRemovables.clear();
	bodyRemovables.clear();
}

void WorldContactListener::BeginContact(b2Contact* contact)
{

	auto fixtureA = contact->GetFixtureA();
	auto fixtureB = contact->GetFixtureB();

	uint16 collisionMask = fixtureA->GetFilterData().categoryBits | fixtureB->GetFilterData().categoryBits;

	for (auto& pair : handlers)
	{
		if ((collisionMask | pair.first.GetMask16()) == pair.first.GetMask16())
		{
			pair.second.second->OnCollisionEnter(contact);
		}
	}
	ClearRemovables();
}

void WorldContactListener::EndContact(b2Contact* contact)
{
	auto fixtureA = contact->GetFixtureA();
	auto fixtureB = contact->GetFixtureB();

	uint16 collisionMask = fixtureA->GetFilterData().categoryBits | fixtureB->GetFilterData().categoryBits;

	for (auto& pair : handlers)
	{
		if ((collisionMask | pair.first.GetMask16()) == pair.first.GetMask16())
		{
			pair.second.second->OnCollisionExit(contact);
		}
	}
	ClearRemovables();
}

void WorldContactListener::AddHandler(const Bitmask& bitmask, ContactListener* listener)
{
	handlers.emplace(std::make_pair(bitmask, std::make_pair(listener->GetListenerId(), listener)));
}

void WorldContactListener::DeleteHandler(const Bitmask& bitmask, ContactListener* listener)
{
	auto listenerId = listener->GetListenerId();
	auto itr = handlers.find(bitmask);
	for (auto itr = handlers.lower_bound(bitmask); itr != handlers.upper_bound(bitmask); ++itr)
	{
		if (listenerId == itr->second.first)
		{
			handlersRemovables.push_back(itr);
			return;
		}
	}
}

void WorldContactListener::ClearRemovables()
{
	if (!handlersRemovables.empty())
	{
		for (auto& itr : handlersRemovables)
		{
			handlers.erase(itr);
		}
		handlersRemovables.clear();
	}
}

void WorldContactListener::AddBodyRemovable(b2Fixture* fixture)
{
	bodyRemovables.push_back(fixture);
}

void WorldContactListener::ClearBodyRemovables()
{
	if (!bodyRemovables.empty())
	{
		for (auto& itr : bodyRemovables)
		{
			world->DestroyBody(itr->GetBody());
		}
		bodyRemovables.clear();
	}
}
