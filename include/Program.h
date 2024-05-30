#pragma once

#include "WindowContext.h"

class Program {
   private:
    unsigned int program;

   public:
    Program(std::string vertex_shader_path, std::string fragment_shader_path);
    Program(std::string vertex_shader_path, std::string geometry_shader_path,
            std::string fragment_shader_path);
    ~Program();

    void bind();
    void unbind();

    template <typename T>
    void set_uniform(std::string, T);

    Program(const Program&) = delete;
    Program(Program&&) = delete;
    Program& operator=(const Program&) = delete;
    Program& operator=(Program&&) = delete;
};