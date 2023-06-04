#pragma once

#include "Scene.h"
#include "ISceneManager.h"

#include "Input.h"
#include "Statistics.h"

class FinishScene : public Scene
{
public:
	FinishScene(sf::RenderWindow& window) : Scene(SceneTypes::FinishScene) , window(window) {}

	void SetISceneManagerPtr(ISceneManager* sceneManager) { this->sceneManager = sceneManager; }

	void Initialize() override;

	void Update() override;

	void Draw(sf::RenderWindow& window) override;

	void Destroy() override {}

private:
	sf::RenderWindow& window;
	ISceneManager* sceneManager;

	sf::Font font;
	sf::Text textLabel;
	sf::Text textInfo;

	sf::Vector2f positionOffsetLabel = { (float)SCREEN_WIDTH / 2, 200.f };
	sf::Vector2f positionOffsetInfo = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2 };

	short textSizeLabel = 50;
	short textSize = 30;
	sf::Color textColor = sf::Color::White;
};

