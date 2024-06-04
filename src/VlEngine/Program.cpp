#include "Program.h"

#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

constexpr int MAX_INFO_LOG_LENGTH = 512;

auto read_file(const std::string& file_path) -> std::string {
    const std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + file_path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

auto compile_shader(const std::string& shader_path,
                    unsigned int shader_type) -> unsigned int {
    const std::string shader_source = read_file(shader_path);
    const unsigned int shader = glCreateShader(shader_type);
    const char* shader_source_ptr = shader_source.c_str();
    glShaderSource(shader, 1, &shader_source_ptr, nullptr);
    glCompileShader(shader);

    int success = 0;
    std::array<char, MAX_INFO_LOG_LENGTH> info_log{};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderInfoLog(shader, MAX_INFO_LOG_LENGTH, nullptr,
                           info_log.data());
        throw std::runtime_error("ERROR::SHADER::" + shader_path +
                                 "::COMPILATION_FAILED\n" + info_log.data());
    }

    return shader;
}

auto link_program(unsigned int program) {
    glLinkProgram(program);

    int success = 0;
    std::array<char, MAX_INFO_LOG_LENGTH> info_log{};
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        glGetProgramInfoLog(program, MAX_INFO_LOG_LENGTH, nullptr,
                            info_log.data());
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINK_FAILED\n" +
                                 std::string(info_log.data()));
    }
}

Program::Program(const std::string vertex_shader_path,    // NOLINT
                 const std::string fragment_shader_path)  // NOLINT
    : program(glCreateProgram()) {
    const unsigned int vertex_shader =
        compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
    const unsigned int fragment_shader =
        compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    link_program(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Program::Program(const std::string vertex_shader_path,    // NOLINT
                 const std::string geometry_shader_path,  // NOLINT
                 const std::string fragment_shader_path)  // NOLINT
    : program(glCreateProgram()) {
    const unsigned int vertex_shader =
        compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
    const unsigned int geometry_shader =
        compile_shader(geometry_shader_path, GL_GEOMETRY_SHADER);
    const unsigned int fragment_shader =
        compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, geometry_shader);
    glAttachShader(program, fragment_shader);
    link_program(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);
}

Program::~Program() { glDeleteProgram(program); }

void Program::bind() { glUseProgram(program); }  // NOLINT

void Program::unbind() { glUseProgram(0); }  // NOLINT

template <>
void Program::set_uniform(const char* const name, int value) {
    int const location = glGetUniformLocation(program, name);
    glUniform1i(location, value);
}

template <>
void Program::set_uniform(const char* const name, float value) {
    int const location = glGetUniformLocation(program, name);
    glUniform1f(location, value);
}

template <>
void Program::set_uniform(const char* const name, glm::vec2 value) {
    int const location = glGetUniformLocation(program, name);
    glUniform2fv(location, 1, glm::value_ptr(value));
}

template <>
void Program::set_uniform(const char* const name, glm::vec4 value) {
    int const location = glGetUniformLocation(program, name);
    glUniform4fv(location, 1, glm::value_ptr(value));
}

template <>
void Program::set_uniform(const char* const name, glm::mat4 value) {
    int const location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template <typename T>
void Program::set_uniform(const char* const /*name*/, T /*value*/) {
    throw std::runtime_error("Unsupported uniform type: " +
                             std::string(typeid(T).name()));
}