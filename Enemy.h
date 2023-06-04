#pragma once

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

#include "ConstData.h"

#include "Time.h"

#include "ContactListener.h"
#include "WorldContactListener.h"

class Enemy : public sf::Sprite, public ContactListener
{
public:
	Enemy(b2World* world, sf::Vector2f spawnPosition);

	~Enemy();

	void Update();

	void UpdateAnimation();

	void Draw(sf::RenderWindow& window);

	void OnCollisionEnter(b2Contact* contact) override;
	void OnCollisionExit(b2Contact* contact) override {}

private:
	b2World* world;
	b2Body* body;
	sf::Texture texture;

	Bitmask sensorCollisionMask;

	b2Vec2 velocity = { -ENEMY_VELOCITY, 0.f };

	FaceDirection faceDirection = FaceDirection::Right;

	short currentAnimFrame = 0;
	float currentFrameTime = 0.f;

	EnemyId enemyId;
	static EnemyId idCounter;
};

