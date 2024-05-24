#pragma once

#include "util.h"

class Program {
   private:
    unsigned int program;

   public:
    Program(std::string vertex_shader_path, std::string fragment_shader_path);
    Program(std::string vertex_shader_path, std::string geometry_shader_path,
            std::string fragment_shader_path);
    ~Program();

    void use();

    void set_uniform(std::string name, int i);
    void set_uniform(std::string name, glm::ivec2 v);
    void set_uniform(std::string name, float f);
    void set_uniform(std::string name, glm::vec2 v);
    void set_uniform(std::string name, glm::vec4 v);
    void set_uniform(std::string name, glm::mat4 m);

    Program(const Program&) = delete;
    Program(Program&&) = delete;
    Program& operator=(const Program&) = delete;
    Program& operator=(Program&&) = delete;

   private:
    static unsigned int compile_shader(std::string shader_path,
                                       unsigned int shader_type);
};