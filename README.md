# Tetris
> A simple Tetris game made from scratch with C++ and OpenGL.

This is a simple Tetris game made from scratch with C++ and OpenGL for the graphics. The game is made with the help of the following libraries:
- [GLFW](https://www.glfw.org/)
- [GLAD](https://glad.dav1d.de/)
- [GLM](https://github.com/g-truc/glm)
- [ImGUI](https://github.com/ocornut/imgui)

## Installation

### Prerequisites
- CMake which can be downloaded [here](https://cmake.org/download/)
- A C++ compiler (MSVC shown in the example below)
- OpenGL compatible graphics card

Clone the repository and run the following commands to install prerequisites and build the project:
```sh
git clone https://github.com/vload/Tetris.git
cd Tetris
git submodule update --init --recursive
mkdir build
cd build
cmake ..
msbuild Tetris.sln /p:Configuration=Release # change this to your compiler
```

## Usage example

The game is controlled with the following keys:
- up arrow: rotate the tetromino
- left arrow: move the tetromino to the left
- right arrow: move the tetromino to the right
- down arrow: move the tetromino down
- escape: close the game
There is also a menu that allows tweaking the certain delays in the game.

The binary can be found in the `build\Release` directory if you followed the installation steps above.

## Features
- Minimum Viable Product (MVP) for the Tetris game.
- Improve user input handling by introducing a delay mechanism instead of using ticks.
- Implement a feature where pressing down accelerates the automatic falling of Tetrominos.
- Preview of next tetromino
- Imgui score display
- On-demand rendering to reduce CPU and GPU usage.

## Screenshots

![Gameplay](/screenshots/screenshot-1.png?raw=true "Gameplay")

## Meta

Vlad Stefanescu - vladtudorstefanescu@gmail.com
[https://github.com/vload](https://github.com/vload)
