#ifndef VERTEXARRAYOBJECT_H_
#define VERTEXARRAYOBJECT_H_

#include <glm/glm.hpp>
#include <iostream>
#include <span>

#include "Bind.h"
#include "BufferObject.h"
#include "WindowContext.h"

class VertexArrayObject {
   private:
    unsigned int ID{};

   public:
    VertexArrayObject() { glGenVertexArrays(1, &ID); }
    ~VertexArrayObject() { glDeleteVertexArrays(1, &ID); }

    void bind() const { glBindVertexArray(ID); }
    void unbind() const { glBindVertexArray(0); }

    template <class Vertex>
    void add_attribute(BufferObject<Vertex>& vbo, int position,
                       int Vertex::*attribute) {
        auto voa_bind = Bind(*this);
        auto vbo_bind = Bind(vbo, GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 1, GL_INT, GL_FALSE, sizeof(Vertex),
                              &(((Vertex*)nullptr)->*attribute));  // NOLINT
    }

    template <class Vertex>
    void add_attribute(BufferObject<Vertex>& vbo, int position,
                       float Vertex::*attribute) {
        auto voa_bind = Bind(*this);
        auto vbo_bind = Bind(vbo, GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              &(((Vertex*)nullptr)->*attribute));  // NOLINT
    }

    template <class Vertex>
    void add_attribute(BufferObject<Vertex>& vbo, int position,
                       glm::vec2 Vertex::*attribute) {
        auto voa_bind = Bind(*this);
        auto vbo_bind = Bind(vbo, GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              &(((Vertex*)nullptr)->*attribute));  // NOLINT
    }

    template <class Vertex>
    void add_attribute(BufferObject<Vertex>& vbo, int position,
                       glm::vec3 Vertex::*attribute) {
        auto voa_bind = Bind(*this);
        auto vbo_bind = Bind(vbo, GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              &(((Vertex*)nullptr)->*attribute));  // NOLINT
    }

    template <class Vertex>
    void add_attribute(BufferObject<Vertex>& vbo, int position,
                       glm::vec4 Vertex::*attribute) {
        auto voa_bind = Bind(*this);
        auto vbo_bind = Bind(vbo, GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              &(((Vertex*)nullptr)->*attribute));  // NOLINT
    }

    VertexArrayObject(const VertexArrayObject&) = delete;
    VertexArrayObject(VertexArrayObject&&) = delete;
    auto operator=(const VertexArrayObject&) -> VertexArrayObject& = delete;
    auto operator=(VertexArrayObject&&) -> VertexArrayObject& = delete;
};

#endif  // VERTEXARRAYOBJECT_H_