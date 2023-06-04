#pragma once

#include "SFML/Graphics.hpp"

#include "ConstData.h"
#include "Statistics.h"

#include "ISceneManager.h"

class PlayerUI
{
public:
	PlayerUI();

	void SetPostion(sf::Vector2f position);

	void SetPostion(float x, float y) {	SetPostion(sf::Vector2f(x, y));	}

	void TakeDamage();

	bool IsDied() { return isDied; }

	void CollectSweet();

	void Draw(sf::RenderWindow& window);

private:
	sf::Font font;
	sf::Text textLives;
	sf::Text textSweets;
	sf::Text textTime;

	sf::Texture heartTexture;
	sf::Sprite hearts[LIVES_MAXIMUM];

	short textSize = 30;
	sf::Color textColor = sf::Color::White;
	sf::Color textOutlineColor = sf::Color::Black;
	float textOutlineThickness = 2.f;

	short livesLeft;
	short sweetsCollected = 0;

	sf::Vector2f position = {0.f, 0.f};
	sf::Vector2f positionOffsetLives = {LEVEL_WIDTH / 2, 20.f};
	sf::Vector2f positionOffsetSweets = { 100.f, 20.f };
	sf::Vector2f positionOffsetTime = { LEVEL_WIDTH - 200.f, 20.f };
	float heartOffsetX = LEVEL_WIDTH / 2 - 100.f;
	float heartOffsetY = 10.f;

	bool isDied = false;
};

