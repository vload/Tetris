#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <glm/glm.hpp>
#include <span>

#include "WindowContext.h"

class Program {
   private:
    unsigned int program;

   public:
    explicit Program(const char* compute_shader_path);
    Program(const char* vertex_shader_path, const char* fragment_shader_path);
    Program(const char* vertex_shader_path, const char* geometry_shader_path,
            const char* fragment_shader_path);
    ~Program();

    void bind() const;
    void unbind() const;

    void set_uniform(const char* name, int value) const;
    void set_uniform(const char* name, float value) const;
    void set_uniform(const char* name, glm::vec2 value) const;
    void set_uniform(const char* name, glm::vec3 value) const;
    void set_uniform(const char* name, glm::vec4 value) const;
    void set_uniform(const char* name, glm::ivec3 value) const;
    void set_uniform(const char* name, glm::mat4 value) const;
    void set_uniform(const char* name, std::span<glm::mat2> value) const;

    Program(const Program&) = delete;
    Program(Program&&) = delete;
    auto operator=(const Program&) -> Program& = delete;
    auto operator=(Program&&) -> Program& = delete;
};

#endif  // PROGRAM_H_
