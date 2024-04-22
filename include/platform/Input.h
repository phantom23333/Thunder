#pragma once

#include <GLFW/glfw3.h>

namespace platform {
	struct Button {
		char pressed = 0;
		char released = 0;
		char held = 0;
		char newState = -1;
		float typedTime = 0;
		char typed = 0;

		enum {
			A = 0,
			B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			NR0, NR1, NR2, NR3, NR4, NR5, NR6, NR7, NR8, NR9,
			Space,
			Enter,
			Escape,
			Up,
			Down,
			Left,
			Right,
			LeftCtrl,
			Tab,
			BUTTONS_COUNT, //
		};

		void merge(Button& other) {
			pressed |= other.pressed;
			released |= other.released;
			held |= other.held;
		}
	};

	int isButtonHeld(int key);
	int isButtonPressed(int key);
	int isButtonReleased(int key);
	int isButtonTyped(int key);

	int isLMousePressed();
	int isRMousePressed();

	int isLMouseHeld();
	int isRMouseHeld();

	namespace internal {
		void setButtonState(int button, int newState);

		void setLeftMouseState(int newState);
		void setRightMouseState(int newState);

		inline void processEventButton(Button& b, bool newState) {
			b.newState = newState;
		}

		inline void updateButton(Button& b, float deltaTime) {
			if (b.newState == 1) {
				if (b.held) {
					b.pressed = 0;
				}
				else {
					b.pressed = 1;
				}

				b.held = 1;
				b.released = 0;

			}
			// released
			else if (b.newState == 0) {
				b.held = 0;
				b.released = 1;
				b.pressed = 0;

			}
			// initial state
			else {
				b.released = 0;
				b.pressed = 0;
			}


			if (b.pressed) {
				b.typed = true;
				b.typedTime = 0;
			}
			else if (b.held) {
				b.typedTime -= deltaTime;
				if (b.typedTime < 0) {
					b.typed = true;
					b.typedTime += 0.07f;
				}
				else {
					b.typed = false;
				}
			}
			else {
				b.typedTime = 0;
				b.typed = false;
			}
			b.newState = -1;
		}


		void updateAllButtons(float deltaTime);
		void resetInputsToZero();

		void addToTypedInput(char c);
		void clearTypedInput();

		void resetTypedInput();
	}




}
