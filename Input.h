#pragma once

#include "SFML/Graphics.hpp"
#include "Bitmask.h"

using Keyboard = sf::Keyboard;

class Input
{
public:
	enum class Key
	{
		None = 0,
		Left,
		Right,
		Up,
		Down,
		Horizontal,
		Vertical,
		Space,
		Esc,
		LShift,
		RShift,
		Enter,
		Num1,
		Num2,
		Num3,
		W,
		A,
		S,
		D,
	};

	static void Update();

	static sf::Vector2f GetInputAxes();

	static bool IsKeyPressed(Key keycode);
	static bool IsKeyDown(Key keycode);
	static bool IsKeyUp(Key keycode);

	static bool IsInputAxesChanged();
	static bool IsInputAxesEnabled();
	static bool IsInputAxesDisabled();

private:
	static Bitmask thisFrameKeys;
	static Bitmask lastFrameKeys;
	static sf::Vector2f inputAxes;
	static sf::Vector2f lastFrameInputAxes;
};