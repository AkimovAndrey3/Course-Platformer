#include "Game.h"

Game::Game(const b2Vec2& gravity) : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SWEET HUNTER", sf::Style::Close | sf::Style::Titlebar), gravity(gravity)
{
	Time::Restart();

	Init();
}

void Game::Init()
{
	sceneManager = new SceneManager;

	auto startScene = new StartScene(window);
	auto gameScene = new GameScene(window, gravity, worldContactListener);
	auto finishScene = new FinishScene(window);

	startScene->SetISceneManagerPtr(sceneManager);
	gameScene->SetISceneManagerPtr(sceneManager);
	finishScene->SetISceneManagerPtr(sceneManager);

	sceneManager->AddScene(startScene);
	sceneManager->AddScene(gameScene);
	sceneManager->AddScene(finishScene);

	sceneManager->SetInitialScene(SceneTypes::StartScene);
}

void Game::Update()
{
	while (window.pollEvent(ev)) {
		if (ev.type == sf::Event::Closed) {
			window.close();
		}
	}
	Input::Update();

	sceneManager->Update();
}

void Game::Draw()
{
	window.clear(sf::Color::Black);

	sceneManager->Draw(window);

	window.display();

	sceneManager->CheckSceneSwitch();
}

void Game::CalculateDeltaTime()
{
	Time::Restart();
}