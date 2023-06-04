#pragma once

#include "Input.h"
#include "WorldContactListener.h"

#include "ISceneManager.h"
#include "Scene.h"

#include "LevelManager.h"
#include "Player.h"
#include "PlayerUI.h"

#include "Statistics.h"

class GameScene : public Scene
{
public:
	GameScene(sf::RenderWindow& window, b2Vec2 gravity, WorldContactListener* worldContactListener) 
		: Scene(SceneTypes::GameScene), window(window), gravity(gravity), worldContactListener(worldContactListener) {}
	~GameScene() {}
	
	void Initialize() override;


	void SetISceneManagerPtr(ISceneManager* sceneManager) { this->sceneManager = sceneManager; }

	void Update() override;

	void Draw(sf::RenderWindow& window) override;

	void Destroy() override;

private:
	b2World* world;
	b2Vec2 gravity;
	sf::RenderWindow& window;

	WorldContactListener* worldContactListener;
	ISceneManager* sceneManager;

	Player* player;
	PlayerUI* playerUi;
	LevelManager* levelManager;
};

