#include "LevelManager.h"

LevelManager::LevelManager(sf::RenderWindow& window) : window(window), activeLevel(nullptr), actualFaceDirection(FaceDirection::Right)
{
	collisionMask.SetBit((uint16)CollisionBits::PlayerLevelSwitcher);
	collisionMask.SetBit((uint16)CollisionBits::LevelSensor);

	WorldContactListener::AddHandler(collisionMask, this);

	ResetCameraView();
}

LevelManager::~LevelManager()
{
	for (auto& enemy : enemies)
	{
		delete enemy;
	}

	activeLevel->ResetIdCounter();
	for (auto& level : levels)
	{
		delete level.second;
	}
	levels.clear();
	WorldContactListener::DeleteHandler(collisionMask, this);


}

void LevelManager::Update()
{
	for (auto& enemy : enemies)
	{
		enemy->Update();
	}
}

void LevelManager::GenerateLevels(unsigned short levelsAmount)
{
	for (int positionOffset = 0; positionOffset < LEVEL_WIDTH * LEVELS_AMOUNT; positionOffset += LEVEL_WIDTH)
	{
		auto levelNumber = rand() % CHUNKS_AMOUNT + 1;
		auto level = new Level;
		level->SetPostion((float)positionOffset, (float)SCREEN_HEIGHT / 2);
		AddLevel(level);

		auto filename = INITIAL_MAP_FILENAME_START + std::to_string(levelNumber) + INITIAL_MAP_FILENAME_END;
		level->LoadFromFile(filename, positionOffset);

		sf::Vector2i tileSize = level->GetTileSize();

		std::vector<Object> block = level->GetObjects("earth");
		for (int i = 0; i < block.size(); i++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(
				positionOffset + block[i].rect.left + tileSize.x / 2 * (block[i].rect.width / tileSize.x - 1),
				block[i].rect.top + tileSize.y / 2 * (block[i].rect.height / tileSize.y - 1));

			b2Body* body = world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(block[i].rect.width / 2, block[i].rect.height / 2);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.friction = 1.f;
			fixtureDef.shape = &shape;

			body->CreateFixture(&fixtureDef);
		}

		std::vector<Object> items = level->GetObjects("item");
		for (int i = 0; i < items.size(); i++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(
				positionOffset + items[i].rect.left + tileSize.x / 2 * (items[i].rect.width / tileSize.x - 1),
				items[i].rect.top + tileSize.y / 2 * (items[i].rect.height / tileSize.y - 1) - TILE_HEIGHT);

			b2Body* body = world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(items[i].rect.width / 2, items[i].rect.height / 2);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.friction = 1.f;
			fixtureDef.isSensor = true;
			fixtureDef.shape = &shape;
			fixtureDef.userData.pointer = items[i].objectId;
			fixtureDef.filter.categoryBits = (1 << ((uint16)CollisionBits::Item));

			body->CreateFixture(&fixtureDef);
		}

		std::vector<Object> spikes = level->GetObjects("idle");
		for (int i = 0; i < spikes.size(); i++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(
				positionOffset + spikes[i].rect.left + tileSize.x / 2 * (spikes[i].rect.width / tileSize.x - 1),
				spikes[i].rect.top + tileSize.y / 2 * (spikes[i].rect.height / tileSize.y - 1));

			b2Body* body = world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(spikes[i].rect.width / 2, spikes[i].rect.height / 2);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.friction = 1.f;
			fixtureDef.isSensor = true;
			fixtureDef.shape = &shape;
			fixtureDef.userData.pointer = spikes[i].objectId;
			fixtureDef.filter.categoryBits = (1 << ((uint16)CollisionBits::Spikes));

			body->CreateFixture(&fixtureDef);
		}

		std::vector<Object> enemySensors = level->GetObjects("solid");
		for (int i = 0; i < enemySensors.size(); i++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(
				positionOffset + enemySensors[i].rect.left + tileSize.x / 2 * (enemySensors[i].rect.width / tileSize.x - 1),
				enemySensors[i].rect.top + tileSize.y / 2 * (enemySensors[i].rect.height / tileSize.y - 1));

			b2Body* body = world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(enemySensors[i].rect.width / 2, enemySensors[i].rect.height / 2);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.friction = 1.f;
			fixtureDef.isSensor = true;
			fixtureDef.shape = &shape;
			fixtureDef.filter.categoryBits = (1 << ((uint16)CollisionBits::EnemySensor));

			body->CreateFixture(&fixtureDef);
		}
		std::vector<Object> enemy = level->GetObjects("enemy");
		for (int i = 0; i < enemy.size(); i++)
		{
			auto enemyBody = new Enemy(world, sf::Vector2f(positionOffset + enemy[i].rect.left + tileSize.x / 2 * (enemy[i].rect.width / tileSize.x - 1),
				enemy[i].rect.top + tileSize.y / 2 * (enemy[i].rect.height / tileSize.y - 1)));
			enemies.push_back(enemyBody);
		}

		{
			/// level sensors
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set((float)positionOffset + LEVEL_WIDTH - TILE_WIDTH / 2, (float)SCREEN_HEIGHT / 2);

			b2Body* body = world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(sensorWidth, (float)SCREEN_HEIGHT / 2);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.isSensor = true;
			fixtureDef.shape = &shape;
			fixtureDef.filter.categoryBits = (1 << ((uint16)CollisionBits::LevelSensor));

			body->CreateFixture(&fixtureDef);
		}

	}


	///level bound left
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(-TILE_WIDTH / 2, SCREEN_HEIGHT / 2);

		b2Body* body = world->CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(boundColliderWidth, SCREEN_HEIGHT / 2);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.f;
		fixtureDef.shape = &shape;

		body->CreateFixture(&fixtureDef);
	}

	///level bound right
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(LEVELS_AMOUNT * LEVEL_WIDTH + LEVEL_WIDTH * 2 - TILE_WIDTH / 2, SCREEN_HEIGHT / 2);

		b2Body* body = world->CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(boundColliderWidth, SCREEN_HEIGHT / 2);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.f;
		fixtureDef.shape = &shape;

		body->CreateFixture(&fixtureDef);
	}

	///level bound bottom
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(LEVELS_AMOUNT * LEVEL_WIDTH / 2 - TILE_WIDTH / 2, SCREEN_HEIGHT);

		b2Body* body = world->CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(LEVELS_AMOUNT * LEVEL_WIDTH / 2, boundColliderHeight);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.f;
		fixtureDef.shape = &shape;

		body->CreateFixture(&fixtureDef);
	}

	///level bound up
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(LEVELS_AMOUNT * LEVEL_WIDTH / 2 - TILE_WIDTH / 2, -TILE_HEIGHT);

		b2Body* body = world->CreateBody(&bodyDef);

		b2PolygonShape shape;
		shape.SetAsBox(LEVELS_AMOUNT * LEVEL_WIDTH / 2, boundColliderHeight);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 1.f;
		fixtureDef.shape = &shape;

		body->CreateFixture(&fixtureDef);
	}

	{
		//final chunk
		auto level = new Level;
		level->SetPostion(finalOffset);
		AddLevel(level);

		auto filename = FINAL_MAP_FILENAME;
		level->LoadFromFile(filename, finalOffset.x);

		sf::Vector2i tileSize = level->GetTileSize();

		std::vector<Object> block = level->GetObjects("earth");
		for (int i = 0; i < block.size(); i++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(
				finalOffset.x + block[i].rect.left + tileSize.x / 2 * (block[i].rect.width / tileSize.x - 1),
				block[i].rect.top + tileSize.y / 2 * (block[i].rect.height / tileSize.y - 1));

			b2Body* body = world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(block[i].rect.width / 2, block[i].rect.height / 2);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.friction = 1.f;
			fixtureDef.shape = &shape;

			body->CreateFixture(&fixtureDef);
		}

		std::vector<Object> finalSensor = level->GetObjects("final");
		for (int i = 0; i < finalSensor.size(); i++)
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(
				finalOffset.x + finalSensor[i].rect.left + tileSize.x / 2 * (finalSensor[i].rect.width / tileSize.x - 1),
				finalSensor[i].rect.top + tileSize.y / 2 * (finalSensor[i].rect.height / tileSize.y - 1));

			b2Body* body = world->CreateBody(&bodyDef);

			b2PolygonShape shape;
			shape.SetAsBox(finalSensor[i].rect.width / 2, finalSensor[i].rect.height / 2);

			b2FixtureDef fixtureDef;
			fixtureDef.density = 1.f;
			fixtureDef.friction = 1.f;
			fixtureDef.isSensor = true;
			fixtureDef.shape = &shape;
			fixtureDef.filter.categoryBits = (1 << ((uint16)CollisionBits::FinishSensor));

			body->CreateFixture(&fixtureDef);
		}
	}

	SetFirstLevel((LevelId)0);
}

Level* LevelManager::AddLevel(Level* level)
{
	auto levelId = level->GetLevelId();
	levels.emplace(levelId, level);
	return level;
}

void LevelManager::SetFirstLevel(LevelId levelId)
{
	auto itr = levels.find(levelId);
	if (itr != levels.end())
	{
		activeLevel = itr->second;
	}
}

void LevelManager::DeleteObject(ObjectId objectId)
{
	if (activeLevel != nullptr)
	{
		activeLevel->DestroyObject(objectId);
	}
}

void LevelManager::SwitchLevel(FaceDirection direction)
{
	auto activeLevelId = activeLevel->GetLevelId();
	auto levelToSwitchId = activeLevelId + (short)direction;
	if (levelToSwitchId > LEVELS_AMOUNT)
	{
		return;
	}
	else
	{
		auto itr = levels.find(levelToSwitchId);
		if (itr != levels.end())
		{
			activeLevel = itr->second;
			SwitchCameraView();
			SwitchUIPosition();
		}
	}
}

void LevelManager::ReloadLevel()
{
	auto itr = levels.find(0);
	if (itr != levels.end())
	{
		activeLevel = itr->second;
		SwitchCameraView();
		SwitchUIPosition();
	}
}

void LevelManager::ResetCameraView()
{
	auto view = window.getView();
	view.setCenter(LEVEL_WIDTH / 2 - TILE_WIDTH / 2, (float)SCREEN_HEIGHT / 2);
	window.setView(view);
}

void LevelManager::SwitchCameraView()
{
	auto view = window.getView();
	view.setCenter(activeLevel->GetPosition().x + LEVEL_WIDTH / 2 - TILE_WIDTH / 2, (float)SCREEN_HEIGHT / 2);
	window.setView(view);
}

void LevelManager::Draw()
{
	activeLevel->Draw(window);
	for (auto& enemy : enemies)
	{
		enemy->Draw(window);
	}
}
