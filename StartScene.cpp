#include "StartScene.h"

StartScene::StartScene(sf::RenderWindow& window) : Scene(SceneTypes::StartScene), window(window)
{
	font.loadFromFile(FONT_FILE_PATH);

	{
		textLabel.setFont(font);
		textLabel.setCharacterSize(textSizeStart);
		textLabel.setFillColor(textColor);

		textLabel.setPosition(offsetLabel.x, offsetLabel.y);

		textLabel.setString("SWEET HUNTER");
		auto bounds = textLabel.getGlobalBounds();
		textLabel.setOrigin(bounds.width / 2, 0.f);
	}

	{
		textStart.setFont(font);
		textStart.setCharacterSize(textSizeLabel);
		textStart.setFillColor(textColor);

		textStart.setPosition(offsetStart.x, offsetStart.y);

		textStart.setString("Choose difficulty to start game");
		auto bounds = textStart.getGlobalBounds();
		textStart.setOrigin(bounds.width / 2, 0.f);
	}

	{
		textDifficulties.setFont(font);
		textDifficulties.setCharacterSize(textSizeDifficulties);
		textDifficulties.setFillColor(textColor);

		textDifficulties.setPosition(offsetDifficulties.x, offsetDifficulties.y);

		textDifficulties.setString("1\t-\tEASY :\t\tLives:\t" + std::to_string(LIVES_MAXIMUM_EASY) + "\tMinimum sweets:\t" + std::to_string(SWEETS_MINIMUM_EASY) + "\n"
			"2\t-\tMEDIUM :\tLives:\t" + std::to_string(LIVES_MAXIMUM_MEDIUM) + "\tMinimum sweets:\t" + std::to_string(SWEETS_MINIMUM_MEDIUM) + "\n"
			"3\t-\tHARD :\t\tLives:\t" + std::to_string(LIVES_MAXIMUM_HARD) + "\tMinimum sweets:\t" + std::to_string(SWEETS_MINIMUM_HARD));
		auto bounds = textDifficulties.getGlobalBounds();
		textDifficulties.setOrigin(bounds.width / 2, 0.f);
	}

	auto view = window.getView();
	view.setCenter(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	window.setView(view);
}

void StartScene::Update()
{

	if (Input::IsKeyDown(Input::Key::Esc))
	{
		window.close();
	}
	if (Input::IsKeyDown(Input::Key::Num1))
	{
		ISceneManager::SetDifficulty(GameDifficulty::Easy);
		sceneManager->SwitchScene(SceneTypes::GameScene);
	}
	if (Input::IsKeyDown(Input::Key::Num2))
	{
		ISceneManager::SetDifficulty(GameDifficulty::Medium);
		sceneManager->SwitchScene(SceneTypes::GameScene);
	}
	if (Input::IsKeyDown(Input::Key::Num3))
	{
		ISceneManager::SetDifficulty(GameDifficulty::Hard);
		sceneManager->SwitchScene(SceneTypes::GameScene);
	}
}

void StartScene::Draw(sf::RenderWindow& window)
{
	window.draw(textLabel);
	window.draw(textStart);
	window.draw(textDifficulties);
}
