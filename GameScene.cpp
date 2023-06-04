#include "GameScene.h"

void GameScene::Initialize()
{
	Statistics::Refresh();

	world = new b2World(gravity);

	worldContactListener = new WorldContactListener;
	worldContactListener->SetWorldPtr(world);

	world->SetContactListener(worldContactListener);

	levelManager = new LevelManager(window);

	player = new Player;
	playerUi = new PlayerUI;

	levelManager->SetWorldPtr(world);
	levelManager->GenerateLevels(LEVELS_AMOUNT);
	levelManager->SetPlayerUiPtr(playerUi);

	player->SetSceneManagerPtr(sceneManager);
	player->SetLevelManagerPtr(levelManager);
	player->SetWorldPtr(world);
	player->SetPlayerUiPtr(playerUi);
	player->SetWindow(&window);
	player->CreateBody();
}

void GameScene::Update()
{
	world->Step(Time::FixedDeltaTime(), 1, 1);
	worldContactListener->ClearBodyRemovables();
	worldContactListener->ClearRemovables();

	if (Input::IsKeyDown(Input::Key::Esc))
	{
		sceneManager->SwitchScene(SceneTypes::StartScene);
	}

	levelManager->Update();
	player->Update();
	Statistics::AddTime();

}

void GameScene::Draw(sf::RenderWindow& window)
{
	levelManager->Draw();

	player->Draw(window);
	playerUi->Draw(window);

}

void GameScene::Destroy()
{
	delete player;
	delete playerUi;
	levelManager->ResetCameraView();
	delete levelManager;

	delete worldContactListener;
	delete world;

}
