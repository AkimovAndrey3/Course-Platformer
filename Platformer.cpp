#include "Game.h"
#include "level.h"
#include "stdafx.h"
#include <Box2D\Box2D.h>

int main()
{
	srand(time(0));

	b2Vec2 gravity = {0.f, 5.f};

	Game game(gravity);

	while (game.GetWindow().isOpen()) {
		game.Update();
		game.Draw();
		game.CalculateDeltaTime();
	}

	return 0;
}