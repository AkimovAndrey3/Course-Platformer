#pragma once

#include <SFML/Graphics.hpp>
#include <map>

#include "ISceneManager.h"
#include "Scene.h"

class SceneManager : public	ISceneManager
{
public:
	SceneManager() {}

	void SetInitialScene(SceneTypes type);

	void AddScene(Scene* scene);

	void SwitchScene(SceneTypes nextScene) override;

	void CheckSceneSwitch();

	void Update();

	void Draw(sf::RenderWindow& window);

private:
	void ChangeScene();

	bool isToChange;
	SceneTypes sceneToChange;

	Scene* activeScene;
	std::map<SceneTypes, Scene*> scenes;
};

