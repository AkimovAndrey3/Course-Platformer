#pragma once

#include "ConstData.h"

#include "Input.h"
#include "Time.h"

#include "LevelManager.h"
#include "SceneManager.h"

#include "StartScene.h"
#include "GameScene.h"
#include "FinishScene.h"

class Game
{
public:
	Game(const b2Vec2& gravity);
	~Game() {}

	void Init();

	void Update();

	void Draw();

	void CalculateDeltaTime();

	const sf::RenderWindow& GetWindow() const { return window; }

private:
	b2World* world;
	b2Vec2 gravity;
	sf::RenderWindow window;
	sf::Event ev;

	WorldContactListener* worldContactListener;

	Player* player;
	PlayerUI* playerUi;

	LevelManager* levelManager;
	SceneManager* sceneManager;
};