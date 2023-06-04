#include "SceneManager.h"

void SceneManager::SetInitialScene(SceneTypes type)
{
	auto itr = scenes.find(type);
	if (itr != scenes.end())
	{
		activeScene = itr->second;
		activeScene->Initialize();
	}
}

void SceneManager::AddScene(Scene* scene)
{
	auto sceneType = scene->GetSceneType();
	auto itr = scenes.find(sceneType);
	if (itr == scenes.end())
	{
		scenes.emplace(sceneType, scene);
	}
}

void SceneManager::SwitchScene(SceneTypes nextScene)
{
	isToChange = true;
	sceneToChange = nextScene;
}

void SceneManager::CheckSceneSwitch()
{
	if (isToChange)
	{
		ChangeScene();
	}
}

void SceneManager::Update()
{
	activeScene->Update();
}

void SceneManager::Draw(sf::RenderWindow& window)
{
	activeScene->Draw(window);
}

void SceneManager::ChangeScene()
{
	auto itr = scenes.find(sceneToChange);
	if (itr != scenes.end())
	{
		activeScene->Destroy();
		activeScene = itr->second;
		activeScene->Initialize();
	}
	isToChange = false;
}
