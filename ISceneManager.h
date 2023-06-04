#pragma once

#include "ConstData.h"

class ISceneManager
{
public:
	virtual ~ISceneManager()
	{

	}

	virtual void SwitchScene(SceneTypes nextScene) = 0;

	static void SetDifficulty(GameDifficulty difficulty) { gameDifficulty = difficulty; }
	static GameDifficulty GetDifficulty() { return gameDifficulty; }

private:
	static GameDifficulty gameDifficulty;

};

