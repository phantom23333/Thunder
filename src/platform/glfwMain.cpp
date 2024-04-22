#include <glad/glad.h>
#include "gl2d/gl2d.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <ctime>

#include <raudio.h>
#include "Input.h"
#include "otherPlatformFunctions.h"
#include "gameLayer.h"
#include <fstream>
#include <chrono>
#include <thread>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imguiThemes.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include "assertion.h"

#undef min
#undef max

#pragma region globals 
bool currentFullScreen = 0;
bool fullScreen = 0;

#pragma endregion

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((action == GLFW_REPEAT || action == GLFW_PRESS) && key == GLFW_KEY_BACKSPACE)
	{
		platform::internal::addToTypedInput(8);
	}

	bool state = false;

	if (action == GLFW_PRESS)
	{
		state = true;
	}
	else if (action == GLFW_RELEASE)
	{
		state = false;
	} else
	{
		return;
	}

	if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z)
	{
		int index = key - GLFW_KEY_A;
		platform::internal::setButtonState(platform::Button::A + index, state);
	} else if (key >= GLFW_KEY_0 &&key <= GLFW_KEY_9)
	{
		int index = key - GLFW_KEY_0;
		platform::internal::setButtonState(platform::Button::NR0 + index, state);
	} else
	{
		if (key == GLFW_KEY_SPACE)
		{
			platform::internal::setButtonState(platform::Button::Space, state);
		}
		else
		if (key == GLFW_KEY_ENTER)
		{
			platform::internal::setButtonState(platform::Button::Enter, state);
		}
		else
		if (key == GLFW_KEY_ESCAPE)
		{
			platform::internal::setButtonState(platform::Button::Escape, state);
		}
		else
		if (key == GLFW_KEY_UP)
		{
			platform::internal::setButtonState(platform::Button::Up, state);
		}
		else
		if (key == GLFW_KEY_DOWN)
		{
			platform::internal::setButtonState(platform::Button::Down, state);
		}
		else
		if (key == GLFW_KEY_LEFT)
		{
			platform::internal::setButtonState(platform::Button::Left, state);
		}
		else
		if (key == GLFW_KEY_RIGHT)
		{
			platform::internal::setButtonState(platform::Button::Right, state);
		}
		else
		if (key == GLFW_KEY_LEFT_CONTROL)
		{
			platform::internal::setButtonState(platform::Button::LeftCtrl, state);
		}
		if (key == GLFW_KEY_TAB)
		{
			platform::internal::setButtonState(platform::Button::Tab, state);
		}
	}
}

void mouseCallBack(GLFWwindow* window, int key, int action, int mods)
{
	bool state = false;
	if (action == GLFW_PRESS)
	{
		state = 1;
	}
	else if (action == GLFW_RELEASE)
	{
		state = 0;
	}
	else return;

	if (key == GLFW_MOUSE_BUTTON_LEFT)
	{
		platform::internal::setLeftMouseState(state);
	} else if (key == GLFW_MOUSE_BUTTON_RIGHT)
	{
		platform::internal::setRightMouseState(state);
	}
}

bool windowFocus = 1;

void windowFocusCallback(GLFWwindow* window, int focused)
{
	if (focused)
	{
		windowFocus = 1;
	} else
	{
		windowFocus = 0;
		platform::internal::resetInputsToZero();
	}
}

void windowsSizeCallBack(GLFWwindow* window, int x, int y)
{
	platform::internal::resetInputsToZero();
}

int mouseMoveFlag = 0;

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	mouseMoveFlag = 1;
}

void characterCallback(GLFWwindow* window, unsigned int codepoint)
{
	if (codepoint < 127) platform::internal::addToTypedInput(codepoint);
}

GLFWwindow* wind = 0;

namespace platform
{
	void setRelMousePosition(int x, int y)
	{
		glfwSetCursorPos(wind, x, y);
	}


	bool isFullScreen() { return fullScreen; }

	void setFullScreen(bool f) { fullScreen = f; }

	glm::ivec2 getFrameBufferSize()
	{
		int x = 0, y = 0;
		glfwGetFramebufferSize(wind, &x, &y);
		return { x, y };
	}

	glm::ivec2 getRelMousePosition()
	{
		double x = 0, y = 0;
		glfwGetCursorPos(wind, &x, &y);
		return { x, y };
	}


	glm::ivec2 getWindowSize()
	{
		int x = 0; int y = 0;
		glfwGetWindowSize(wind, &x, &y);
		return { x, y };
	}


	bool isFocused()
	{

		return windowFocus;
	}

	bool mouseMoved()
	{
		return mouseMoveFlag;
	}

	bool writeEntireFile(const char* name, void* buffer, size_t size)
	{
		std::ofstream f(name, std::ios::binary);

		if (!f.is_open())
		{
			return false;
		}
		f.write((char*)buffer, size);
		f.close();
		return true;

	}

	bool readEntireFile(const char* name, void* buffer, size_t size)
	{
		std::ifstream f(name, std::ios::binary);

		if (!f.is_open())
		{
			return false;
		}
		f.read((char*)buffer, size);
		f.close();
		return true;
	}
}


int main()
{

	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	int w = 500, h = 500;
	wind = glfwCreateWindow(w, h, "thunder", nullptr, nullptr);
	glfwMakeContextCurrent(wind);
	glfwSwapInterval(1);

	// binding the callback function
	glfwSetKeyCallback(wind, keyCallback);
	glfwSetMouseButtonCallback(wind, mouseCallBack);
	glfwSetWindowFocusCallback(wind, windowFocusCallback);
	glfwSetWindowSizeCallback(wind, windowsSizeCallBack);
	glfwSetCursorPosCallback(wind, cursorPositionCallback);
	glfwSetCharCallback(wind, characterCallback);

	permaAssertComment(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "err initializing glad");

	ImGui::CreateContext();
	//ImGui::StyleColorsDark();
	imguiThemes::embraceTheDarkness();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.f;
		style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
	}

	ImGui_ImplGlfw_InitForOpenGL(wind, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	InitAudioDevice();

	if (!initGame())
	{
		return 0;
	}

	auto stop = std::chrono::high_resolution_clock::now();

	// enter the game loop
	while (!glfwWindowShouldClose(wind))
	{
		auto start = std::chrono::high_resolution_clock::now();
		// change it back to seconds
		float deltaTime = (std::chrono::duration_cast<std::chrono::nanoseconds>(start - stop)).count() / 1000000000.0;
		stop = std::chrono::high_resolution_clock::now();
		float augmentedDeltaTime = deltaTime;

		// that is 60 frame per second
		if (augmentedDeltaTime > 1.f / 60)
		{
			
		} else if (augmentedDeltaTime <= 1.f / 60) {
			// delay the time
			float timeToWait = 1.f / 60 - augmentedDeltaTime;
			std::this_thread::sleep_for(std::chrono::duration<float>(timeToWait));
			augmentedDeltaTime = 1.f / 60;
		}

		// ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		// Game logic

		if (!gameLogic(augmentedDeltaTime))
		{
			closeGame();
			return 0;
		}

		if (platform::isFocused() && currentFullScreen != fullScreen)
		{
			static int lastW = w;
			static int lastH = w;
			static int lastPosX = 0;
			static int lastPosY = 0;

			if (fullScreen)
			{
				lastW = w;
				lastH = h;

				glfwGetWindowPos(wind, &lastPosX, &lastPosY);
				auto monitor = getCurrentMonitor(wind);
				const GLFWvidmode* mod = glfwGetVideoMode(monitor);
				glfwSetWindowMonitor(wind, monitor, 0, 0, mod->width, mod->height, mod->refreshRate);
				currentFullScreen = 1;

			} else
			{
				glfwSetWindowMonitor(wind, nullptr, lastPosX, lastPosY, lastW, lastH, 0);
				currentFullScreen = false;
			}
		}

		mouseMoveFlag = 0;
		platform::internal::updateAllButtons(augmentedDeltaTime);
		platform::internal::resetTypedInput();

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(wind, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);

		}

		glfwSwapBuffers(wind);
		glfwPollEvents();

	}

	

	return 1;
}