#pragma once

#include <Box2D\Box2D.h>
#include "Time.h"
#include "Input.h"

#include "ContactListener.h"
#include "WorldContactListener.h"
#include "LevelManager.h"
#include "PlayerUI.h"
#include "ISceneManager.h"

#include "ConstData.h"
#include "Statistics.h"


class Player : public sf::Sprite, public ContactListener
{
public:
	Player();
	~Player();

	void OnCollisionEnter(b2Contact* contact) override;
	void OnCollisionExit(b2Contact* contact) override {}

	void SetLevelManagerPtr(LevelManager* levelManager) { this->levelManager = levelManager;}

	void Update();

	void CheckDirectionSwap(sf::Vector2f& direction);

	void UpdateAnimation();

	void Draw(sf::RenderTarget& window);

	void SetSceneManagerPtr(ISceneManager* sceneManager) { this->sceneManager = sceneManager; }
	void SetPlayerUiPtr(PlayerUI* playerUi) { this->playerUi = playerUi; }
	void SetWorldPtr(b2World* world) { this->world = world; }
	void SetWindow(sf::RenderWindow* window) { this->window = window; }

	void CreateBody();

private:
	b2World* world;
	b2Body* playerBody;
	sf::RenderWindow* window;

	sf::Texture texture;

	PlayerUI* playerUi;

	ISceneManager* sceneManager;
	LevelManager* levelManager;

	Bitmask itemCollisionMask;
	Bitmask spikesCollisionMask;
	Bitmask enemyCollisionMask;
	Bitmask finishCollisionMask;

	std::string textureFilename;

	b2Vec2 spawnPositon = PLAYER_SPAWN_POSITION;

	float levelSwitcherSensorWidth = 2.f;

	float velocity;
	float sprintVelocity = PLAYER_SPRINT_VELOCTIY;
	float normalVelocity = PLAYER_NORMAL_VELOCITY;

	float jumpTime = 0.f;
	float jumpMaxTime = PLAYER_JUMP_MAX_TIME;

	float preJumpGravityScale = 0.1f;
	float normalGravityScale = 0.4f;

	float jumpSpeed = PLAYER_JUMP_SPEED;

	FaceDirection faceDirection = FaceDirection::Right;
	FaceDirection lastFaceDirection;

	bool isDamaged;

	short currentAnimFrame = 0;
	float currentFrameTime = 0.f;
	AnimStates currentAnimState = AnimStates::Idle;
};