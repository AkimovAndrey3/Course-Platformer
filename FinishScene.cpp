#include "FinishScene.h"

void FinishScene::Initialize()
{
	font.loadFromFile(FONT_FILE_PATH);

	textLabel.setFont(font);
	textLabel.setCharacterSize(textSizeLabel);
	textLabel.setFillColor(textColor);

	textLabel.setPosition(positionOffsetLabel);

	if (Statistics::IsGameWon())
	{
		textLabel.setString("Congratulations!\nYou've won the game");
	}
	else
	{
		textLabel.setString("Game over");
	}

	auto bounds = textLabel.getGlobalBounds();
	textLabel.setOrigin(bounds.width / 2, 0.f);


	textInfo.setFont(font);
	textInfo.setCharacterSize(textSize);
	textInfo.setFillColor(textColor);

	textInfo.setPosition(positionOffsetInfo);

	textInfo.setString("Press 'Enter' to try again\nOr press 'ESC' to live the game\n\nSweets collected: " +
		std::to_string(Statistics::GetCollectedSweets()) + " / " + std::to_string(SWEETS_MAXIMUM) +
		"\nTime spent: " + Statistics::GetTimePlayedString());
	bounds = textInfo.getGlobalBounds();
	textInfo.setOrigin(bounds.width / 2, 0.f);
}

void FinishScene::Update()
{
	if (Input::IsKeyDown(Input::Key::Enter))
	{
		sceneManager->SwitchScene(SceneTypes::GameScene);
	}
	if (Input::IsKeyDown(Input::Key::Esc))
	{
		window.close();
	}
}

void FinishScene::Draw(sf::RenderWindow& window)
{
	window.draw(textLabel);
	window.draw(textInfo);
}
