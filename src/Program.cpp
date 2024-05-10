#include "Program.h"

unsigned int Program::compile_shader(std::string shader_path,
                                            unsigned int shader_type) {
    char* shader_source = read_file(shader_path);
    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_source, NULL);
    glCompileShader(shader);

    free(shader_source);  // free the memory allocated by read_file()

    int success;
    char info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::" << shader_path
                  << "::COMPILATION_FAILED\n"
                  << info_log << std::endl;
        std::cout << shader_source << std::endl;
    }

    return shader;
}

Program::Program(std::string vertex_shader_path,
                 std::string fragment_shader_path) {
    unsigned int vertex_shader =
        compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
    unsigned int fragment_shader =
        compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    int success;
    char info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
                  << info_log << std::endl;
    }
}

Program::Program(std::string vertex_shader_path,
                 std::string geometry_shader_path,
                 std::string fragment_shader_path) {
    unsigned int vertex_shader =
        compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
    unsigned int geometry_shader =
        compile_shader(geometry_shader_path, GL_GEOMETRY_SHADER);
    unsigned int fragment_shader =
        compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, geometry_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
                  << infoLog << std::endl;
    }
}

// Program::Program(Shader computeShader) {
//     program = glCreateProgram();
//     glAttachShader(program, computeShader.get());
//     glLinkProgram(program);

//     int success;
//     char infoLog[512];
//     glGetProgramiv(program, GL_LINK_STATUS, &success);
//     if (!success) {
//         glGetProgramInfoLog(program, 512, NULL, infoLog);
//         std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
//                   << infoLog << std::endl;
//     }
// }

Program::~Program() { glDeleteProgram(program); }

void Program::use() { glUseProgram(program); }

void Program::set_uniform(std::string name, int i) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, i);
}

void Program::set_uniform(std::string name, float f) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform1f(location, f);
}

void Program::set_uniform(std::string name, glm::vec2 v) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform2f(location, v.x, v.y);
}

void Program::set_uniform(std::string name, glm::ivec2 v) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform2i(location, v.x, v.y);
}

void Program::set_uniform(std::string name, glm::vec4 v) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform4f(location, v.x, v.y, v.z, v.w);
}

void Program::set_uniform(std::string name, glm::mat4 m) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

unsigned int Program::get() { return program; }