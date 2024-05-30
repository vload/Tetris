#ifndef PROGRAM_H_
#define PROGRAM_H_

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
    void set_uniform(const char* const name, T value); // NOLINT

    Program(const Program&) = delete;
    Program(Program&&) = delete;
    auto operator=(const Program&) -> Program& = delete;
    auto operator=(Program&&) -> Program& = delete;
};

#endif  // PROGRAM_H_
