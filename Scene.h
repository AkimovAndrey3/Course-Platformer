#pragma once

#include <SFML/Graphics.hpp>
#include "ConstData.h"

class Scene
{
public:
	Scene(SceneTypes type) : sceneType(type) {}

	SceneTypes GetSceneType() const { return sceneType; }

	virtual void Initialize() = 0;

	virtual void Update() = 0;
	virtual void Draw(sf::RenderWindow& window) = 0;

	virtual void Destroy() = 0;

private:
	SceneTypes sceneType;
};

