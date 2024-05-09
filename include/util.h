#pragma once

#include <glad/glad.h>
// This order is important
#include <GLFW/glfw3.h>

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// imgui
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

// std
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#define VAR_NAME(var) #var

// random float
auto random_float = []() -> float {
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
};

auto random_int = [](int min, int max) -> int {
    return rand() % (max - min + 1) + min;
};

// the return value must be freed
char* read_file(std::string filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename.c_str(), "rb");
    if (err != 0) {
        std::cout << "Failed to open file: " << filename << std::endl;
        std::cout << "Error code: " << err << std::endl;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(sizeof(char) * file_size + 1);
    if (buffer == NULL) {
        std::cout << "Failed to allocate memory for file: " << filename
                  << std::endl;
        return NULL;
    }
    size_t result = fread(buffer, 1, file_size, file);
    if (result != file_size) {
        std::cout << "Failed to read file: " << filename << std::endl;
        return NULL;
    }

    buffer[file_size] = '\0';
    fclose(file);
    return buffer;
}

// template <typename T>
// void print_raw_bits(const std::vector<T>& data) {
//     // Reinterpret the vector's data as an array of char
//     const char* rawData = reinterpret_cast<const char*>(data.data());

//     // Calculate the total size in bytes
//     size_t totalSize = data.size() * sizeof(T);

//     // Print the raw bits
//     for (size_t i = 0; i < totalSize; ++i) {
//         if (i % sizeof(T) == 0) {
//             std::cout << "\n" << i / sizeof(T) << ": ";
//         } else {
//             std::cout << " ";
//         }

//         // std::cout << std::hex << std::setw(2) << std::setfill('0')
//         //   << static_cast<int>(rawData[i]) << std::dec << " ";
//         std::cout << (short) rawData[i] << " ";
//         std::cout << std::flush;
//     }
//     std::cout << std::endl;
// }