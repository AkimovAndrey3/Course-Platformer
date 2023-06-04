#include "Input.h"


void Input::Update()
{
    lastFrameKeys.SetMask(thisFrameKeys);
    lastFrameInputAxes = inputAxes;

    float horizontal = (int)thisFrameKeys.GetBit((int)Key::Right) - (int)thisFrameKeys.GetBit((int)Key::Left);
    float vertical = (int)thisFrameKeys.GetBit((int)Key::Down) - (int)thisFrameKeys.GetBit((int)Key::Up);

    inputAxes.x = horizontal;
    inputAxes.y = vertical;


    thisFrameKeys.SetBit((int)Key::Left,
        (Keyboard::isKeyPressed(Keyboard::Left)) || (Keyboard::isKeyPressed(Keyboard::A)));

    thisFrameKeys.SetBit((int)Key::Right,
        (Keyboard::isKeyPressed(Keyboard::Right)) || (Keyboard::isKeyPressed(Keyboard::D)));

    thisFrameKeys.SetBit((int)Key::Up,
        (Keyboard::isKeyPressed(Keyboard::Up)) || (Keyboard::isKeyPressed(Keyboard::W)));

    thisFrameKeys.SetBit((int)Key::Down,
        (Keyboard::isKeyPressed(Keyboard::Down)) || (Keyboard::isKeyPressed(Keyboard::S)));

    thisFrameKeys.SetBit((int)Key::Horizontal,
        (thisFrameKeys.GetBit((int)Key::Left) || thisFrameKeys.GetBit((int)Key::Right)));

    thisFrameKeys.SetBit((int)Key::Vertical,
        (thisFrameKeys.GetBit((int)Key::Up) || thisFrameKeys.GetBit((int)Key::Down)));

    thisFrameKeys.SetBit((int)Key::Esc, Keyboard::isKeyPressed(Keyboard::Escape));

    thisFrameKeys.SetBit((int)Key::Space, Keyboard::isKeyPressed(Keyboard::Space));
    thisFrameKeys.SetBit((int)Key::LShift, Keyboard::isKeyPressed(Keyboard::LShift));
    thisFrameKeys.SetBit((int)Key::RShift, Keyboard::isKeyPressed(Keyboard::RShift));

    thisFrameKeys.SetBit((int)Key::Enter, Keyboard::isKeyPressed(Keyboard::Enter));
    thisFrameKeys.SetBit((int)Key::Num1, Keyboard::isKeyPressed(Keyboard::Num1));
    thisFrameKeys.SetBit((int)Key::Num2, Keyboard::isKeyPressed(Keyboard::Num2));
    thisFrameKeys.SetBit((int)Key::Num3, Keyboard::isKeyPressed(Keyboard::Num3));

    thisFrameKeys.SetBit((int)Key::W, Keyboard::isKeyPressed(Keyboard::W));
    thisFrameKeys.SetBit((int)Key::A, Keyboard::isKeyPressed(Keyboard::A));
    thisFrameKeys.SetBit((int)Key::S, Keyboard::isKeyPressed(Keyboard::S));
    thisFrameKeys.SetBit((int)Key::D, Keyboard::isKeyPressed(Keyboard::D));
}

sf::Vector2f Input::GetInputAxes()
{
    return inputAxes;
}

bool Input::IsKeyPressed(Key keycode)
{
    return thisFrameKeys.GetBit((int)keycode);
}

bool Input::IsKeyDown(Key keycode)
{
    bool lastFrame = lastFrameKeys.GetBit((int)keycode);
    bool thisFrame = thisFrameKeys.GetBit((int)keycode);

    return thisFrame && !lastFrame;
}

bool Input::IsKeyUp(Key keycode)
{
    bool lastFrame = lastFrameKeys.GetBit((int)keycode);
    bool thisFrame = thisFrameKeys.GetBit((int)keycode);

    return !thisFrame && lastFrame;
}

bool Input::IsInputAxesChanged()
{
    return inputAxes != lastFrameInputAxes;
}

bool Input::IsInputAxesEnabled()
{
    return inputAxes != sf::Vector2f(0.f, 0.f) && lastFrameInputAxes == sf::Vector2f(0.f, 0.f);
}

bool Input::IsInputAxesDisabled()
{
    return inputAxes == sf::Vector2f(0.f, 0.f) && lastFrameInputAxes != sf::Vector2f(0.f, 0.f);
}

Bitmask Input::thisFrameKeys;
Bitmask Input::lastFrameKeys;
sf::Vector2f Input::inputAxes;
sf::Vector2f Input::lastFrameInputAxes;
