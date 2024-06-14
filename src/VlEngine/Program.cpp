#include "Program.h"

#include <glad/glad.h>

#include <fstream>
#include <glm/ext/matrix_float2x2.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

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
    const int shader_source_length = static_cast<int>(shader_source.length());
    const char* const shader_source_data = shader_source.data();
    const unsigned int shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &shader_source_data, &shader_source_length);
    glCompileShader(shader_id);

#ifndef NDEBUG
    int success = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        int info_log_length = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::string info_log(info_log_length, 0);
        glGetShaderInfoLog(shader_id, MAX_INFO_LOG_LENGTH, nullptr,
                           info_log.data());
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR,
                             0, GL_DEBUG_SEVERITY_HIGH, info_log_length,
                             info_log.data());
    }
#endif

    return shader_id;
}

void link_program(unsigned int program) {
    glLinkProgram(program);

#ifndef NDEBUG
    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        int info_log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        std::string info_log(info_log_length, 0);
        glGetProgramInfoLog(program, MAX_INFO_LOG_LENGTH, nullptr,
                            info_log.data());
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR,
                             0, GL_DEBUG_SEVERITY_HIGH, info_log_length,
                             info_log.data());
    }
#endif
}

Program::Program(const char* const compute_shader_path)
    : program(glCreateProgram()) {
    const unsigned int compute_shader =
        compile_shader(compute_shader_path, GL_COMPUTE_SHADER);

    glAttachShader(program, compute_shader);
    link_program(program);

    glDeleteShader(compute_shader);
}

Program::Program(const char* const vertex_shader_path,
                 const char* const fragment_shader_path)
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

Program::Program(const char* const vertex_shader_path,
                 const char* const geometry_shader_path,
                 const char* const fragment_shader_path)
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

void Program::bind() const { glUseProgram(program); }

void Program::unbind() const { glUseProgram(0); }

void Program::set_uniform(const char* const name, int value) const {
    int const location = glGetUniformLocation(program, name);
    glUniform1i(location, value);
}

void Program::set_uniform(const char* const name, float value) const {
    int const location = glGetUniformLocation(program, name);
    glUniform1f(location, value);
}

void Program::set_uniform(const char* const name, glm::vec2 value) const {
    int const location = glGetUniformLocation(program, name);
    glUniform2fv(location, 1, glm::value_ptr(value));
}

void Program::set_uniform(const char* const name, glm::vec4 value) const {
    int const location = glGetUniformLocation(program, name);
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Program::set_uniform(const char* const name, glm::mat4 value) const {
    int const location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Program::set_uniform(const char* const name,
                          std::span<glm::mat2> value) const {
    int const location = glGetUniformLocation(program, name);
    std::vector<float> values(value.size() * 4);
    for (int i = 0; i < value.size(); i++) {
        for (int j = 0; j < 4; j++) {
            values[i * 4 + j] = value[i][j / 2][j % 2];
        }
    }

    glUniformMatrix2fv(location, static_cast<GLsizei>(value.size()), GL_FALSE,
                       values.data());
}
