# **THUNDER**

- **THUNDER** is a 2D shoot'em all game that is built on top of OpenGL and glfw.
- Made purely with c++ and without any refernce and help from chatGPT or any other AI.

## **DEMO**
**Demo of the engine is shown in the video below:**

[![Watch the video](https://img.youtube.com/vi/M7gHERrPfYA/maxresdefault.jpg)](https://www.youtube.com/watch?v=M7gHERrPfYA)

## **Art work credit**
- https://lowich.itch.io/free-spaceship-sprites
- https://wolf-viciox.itch.io/health-bar
- https://deep-fold.itch.io/space-background-generator

## How To Build
- Build By CMake
```shell
cmake -S . -B build
# Only Support Windows right now
```
## How To Run
  
  0. Make sure you have all the prerequisites installed.
  1. Open Visual Studio and build the project.
  2. Run the project.


## Prerequisites
- [glad](https://glad.dav1d.de/): OpenGL loader
- [gl2d](https://github.com/meemknight/gl2d): simple 2D rendering library made with OpenGL
- [glm](https:://github.com/g-truc/glm): header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
- [ImGui](https://github.com/ocornut/imgui): Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies
- [glfw](https://github.com/glfw/glfw): A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input
- [stb](https://github.com/nothings/stb): single-file public domain libraries for C/C++