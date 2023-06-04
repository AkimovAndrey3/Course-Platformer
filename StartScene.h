#pragma once

#include "Scene.h"
#include "ISceneManager.h"

#include "Input.h"
#include "Statistics.h"

class StartScene : public Scene
{
public:
	StartScene(sf::RenderWindow& window);

	void SetISceneManagerPtr(ISceneManager* sceneManager) { this->sceneManager = sceneManager; }

	void Initialize() override {}

	void Update() override;

	void Draw(sf::RenderWindow& window) override;

	void Destroy() override {}

private:
	sf::RenderWindow& window;
	ISceneManager* sceneManager;

	sf::Font font;
	sf::Text textLabel;
	sf::Text textStart;
	sf::Text textDifficulties;

	short textSizeLabel = 50;
	short textSizeStart = 40;
	short textSizeDifficulties = 30;

	sf::Color textColor = sf::Color::White;

	sf::Vector2f offsetLabel = { SCREEN_WIDTH / 2, 50.f };
	sf::Vector2f offsetStart = { SCREEN_WIDTH / 2, 400.f };
	sf::Vector2f offsetDifficulties = { SCREEN_WIDTH / 2, 500.f };
};

