#include "Input.h"

#include <string>

platform::Button keyboard[platform::Button::BUTTONS_COUNT];
platform::Button mouse[2];

std::string typedInput;

int ::platform::isButtonHeld(int key)
{
	if (key < Button::A || key >= Button::BUTTONS_COUNT)
	{
		return 0;
	}
	return keyboard[key].held;
}

int ::platform::isButtonPressed(int key)
{
	if (key < Button::A || key >= Button::BUTTONS_COUNT)
	{
		return 0;
	}
	return keyboard[key].pressed;
}

int ::platform::isButtonReleased(int key)
{
	if (key < Button::A || key >= Button::Button::BUTTONS_COUNT)
	{
		return 0;
	}
	return keyboard[key].released;
}

int ::platform::isButtonTyped(int key)
{
	if (key < Button::A || key >= Button::Button::BUTTONS_COUNT)
	{
		return 0;
	}
	return keyboard[key].typed;
}

int ::platform::isLMousePressed()
{
	return mouse[0].pressed;
}

int ::platform::isRMousePressed()
{
	return mouse[1].pressed;
}

int ::platform::isLMouseHeld()
{
	return mouse[0].held;
}

int ::platform::isRMouseHeld()
{
	return mouse[1].held;
}

void ::platform::internal::setButtonState(int button, int newState)
{
	processEventButton(keyboard[button], newState);
}

void ::platform::internal::setLeftMouseState(int newState)
{
	processEventButton(mouse[0], newState);
}

void ::platform::internal::setRightMouseState(int newState)
{
	processEventButton(mouse[1], newState);
}

void ::platform::internal::updateAllButtons(float deltaTime)
{
	for (int i = 0; i < Button::BUTTONS_COUNT; i++)
	{
		updateButton(keyboard[i], deltaTime);
	}
	updateButton(mouse[0], deltaTime);
	updateButton(mouse[1], deltaTime);
}

void ::platform::internal::resetInputsToZero()
{
	resetTypedInput();
	auto resetButtonToZero = [](Button& button)
	{
		button.held = 0;
		button.pressed = 0;
		button.released = 0;
		button.typed = 0;
	};

	for (int i = 0; i < Button::BUTTONS_COUNT; i++)
	{
		resetButtonToZero(keyboard[i]);
	}

	resetButtonToZero(mouse[0]);
	resetButtonToZero(mouse[1]);

}

void ::platform::internal::addToTypedInput(char c)
{
	typedInput += c;
}

void platform::internal::resetTypedInput()
{
	typedInput.clear();
}


