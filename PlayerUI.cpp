#include "PlayerUI.h"

PlayerUI::PlayerUI()
{
	livesLeft = GAME_DIFFICULTY_LIVES[(short)ISceneManager::GetDifficulty()];

	font.loadFromFile(FONT_FILE_PATH);

	heartTexture.loadFromFile(LIVE_SPRITE_FILE_PATH);

	for (size_t i = 0; i < LIVES_MAXIMUM; i++)
	{
		sf::Sprite sprite;
		sprite.setTexture(heartTexture);
		sprite.setPosition(position.x + heartOffsetX + 50.f * i, heartOffsetY);
		sprite.setScale(1.f / 6, 1.f / 6);
		hearts[i] = sprite;
	}

	textSweets.setFont(font);
	textSweets.setCharacterSize(textSize);
	textSweets.setFillColor(textColor);
	textSweets.setOutlineColor(textOutlineColor);
	textSweets.setOutlineThickness(textOutlineThickness);


	textSweets.setPosition(position + positionOffsetSweets);

	textSweets.setString("Sweets : " + std::to_string(sweetsCollected));
	auto bounds = textSweets.getGlobalBounds();
	textSweets.setOrigin(bounds.width / 2, 0.f);


	textTime.setFont(font);
	textTime.setCharacterSize(textSize);
	textTime.setFillColor(textColor);
	textTime.setOutlineColor(textOutlineColor);
	textTime.setOutlineThickness(textOutlineThickness);

	textTime.setPosition(position + positionOffsetTime);

	textTime.setString(Statistics::GetTimePlayedString());
	bounds = textTime.getGlobalBounds();
	textTime.setOrigin(bounds.width / 2, 0.f);
}

void PlayerUI::SetPostion(sf::Vector2f position)
{
	this->position = position;
	textLives.setPosition(position + positionOffsetLives);
	textSweets.setPosition(position + positionOffsetSweets);
	textTime.setPosition(position + positionOffsetTime);

	for (size_t i = 0; i < LIVES_MAXIMUM; i++)
	{
		hearts[i].setPosition(position.x + heartOffsetX + 50.f * i, heartOffsetY);
	}
}

void PlayerUI::TakeDamage()
{
	--livesLeft;
	if (livesLeft <= 0)
	{
		livesLeft = 0;
		isDied = true;
	}
	textLives.setString("Lives left : " + std::to_string(livesLeft));
}

void PlayerUI::CollectSweet()
{
	++sweetsCollected;
	Statistics::AddSweet();
	textSweets.setString("Sweets : " + std::to_string(sweetsCollected));
}

void PlayerUI::Draw(sf::RenderWindow& window)
{
	textTime.setString(Statistics::GetTimePlayedString());

	for (size_t i = 0; i < livesLeft; i++)
	{
		window.draw(hearts[i]);
	}

	window.draw(textLives);
	window.draw(textSweets);
	window.draw(textTime);
}
