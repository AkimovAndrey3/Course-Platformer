#pragma once

#include <box2d/box2d.h>
#include "ConstData.h"

#include "Level.h"
#include "PlayerUI.h"

#include "Enemy.h"

#include "ContactListener.h"
#include "Bitmask.h"
#include "WorldContactListener.h"
#include "ConstData.h"

class LevelManager : public ContactListener
{
public:
	LevelManager(sf::RenderWindow& window);
	~LevelManager();

	void OnCollisionEnter(b2Contact* contact) override { SwitchLevel(actualFaceDirection); }
	void OnCollisionExit(b2Contact* contact) override {}

	void Update();

	void GenerateLevels(unsigned short levelsAmount);

	Level* AddLevel(Level* level);

	void SetFirstLevel(Level* level) { activeLevel = level; }
	void SetFirstLevel(LevelId levelId);

	void SetWorldPtr(b2World* world) { this->world = world; }
	void SetPlayerUiPtr(PlayerUI* playerUi) { this->playerUi = playerUi; }

	void DeleteObject(ObjectId objectId);

	void SwitchFaceDirection(FaceDirection direction) { actualFaceDirection = direction; }

	void SwitchLevel(FaceDirection direction);

	void ReloadLevel();

	void ResetCameraView();

	void SwitchUIPosition() { playerUi->SetPostion(activeLevel->GetPosition().x, 0.f); }

	void SwitchCameraView();

	void Draw();

private:
	b2World* world;
	sf::RenderWindow& window;
	PlayerUI* playerUi;

	Level* activeLevel;
	std::map<LevelId, Level*> levels;

	Bitmask collisionMask;

	FaceDirection actualFaceDirection;

	sf::Vector2f finalOffset = { LEVELS_AMOUNT * LEVEL_WIDTH - TILE_WIDTH / 2, SCREEN_HEIGHT / 2 };

	float sensorWidth = 2.f;
	float boundColliderWidth = 2.f;
	float boundColliderHeight = 2.f;

	std::vector<Enemy*> enemies;
};

