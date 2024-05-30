#include "Program.h"

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

unsigned int compile_shader(std::string shader_path, unsigned int shader_type) {
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

    glDeleteShader(vertex_shader);
    glDeleteShader(geometry_shader);
    glDeleteShader(fragment_shader);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n"
                  << infoLog << std::endl;
    }
}

Program::~Program() { glDeleteProgram(program); }

void Program::bind() { glUseProgram(program); }

void Program::unbind() { glUseProgram(0); }

template <>
void Program::set_uniform(std::string name, int i) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, i);
}

template <>
void Program::set_uniform(std::string name, float f) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform1f(location, f);
}

template <>
void Program::set_uniform(std::string name, glm::vec2 v) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform2f(location, v.x, v.y);
}

template <>
void Program::set_uniform(std::string name, glm::ivec2 v) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform2i(location, v.x, v.y);
}

template <>
void Program::set_uniform(std::string name, glm::vec4 v) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniform4f(location, v.x, v.y, v.z, v.w);
}

template <>
void Program::set_uniform(std::string name, glm::mat4 m) {
    int location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

template <typename T>
void Program::set_uniform(std::string name, T t) {
    throw std::runtime_error("Unsupported uniform type: " +
                             std::string(typeid(T).name()));
}