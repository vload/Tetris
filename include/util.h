#pragma once

// glad and glfw
#include <glad/glad.h>
// This order is important
#include <GLFW/glfw3.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// std
#include <array>
#include <iostream>
#include <string>
#include <vector>

// include TetrisState.h
struct TetrisState;
#include "TetrisState.h"

#define VAR_NAME(var) #var

int random_int(int min, int max);

// the returned string must be freed
char* read_file(std::string filename);
