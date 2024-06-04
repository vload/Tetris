#ifndef VERTEXBUFFER_H_
#define VERTEXBUFFER_H_

#include <iostream>
#include <span>

#include "Bind.h"
#include "WindowContext.h"

template <class Vertex>
class VertexBuffer {
   private:
    unsigned int VAO{};
    unsigned int VBO{};

   public:
    VertexBuffer() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
    ~VertexBuffer() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void bind() {
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    }
    void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void copy_static_data(std::span<const Vertex> vertices) {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     vertices.data(), GL_STATIC_DRAW);
    }

    void copy_dynamic_data(std::span<const Vertex> vertices) {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     vertices.data(), GL_DYNAMIC_DRAW);
    }

    template <class U>
    void add_attribute(int /*position*/, U Vertex::* /*attribute*/) {
        throw std::runtime_error("Unsupported array attribute type: " +
                                 std::string(typeid(U).name()));
    }

    template <>
    void add_attribute(int position, int Vertex::*attribute) {
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 1, GL_INT, GL_FALSE, sizeof(Vertex),
                              &(((Vertex*)nullptr)->*attribute));  // NOLINT
    }

    template <>
    void add_attribute(int position, float Vertex::*attribute) {
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              &(((Vertex*)nullptr)->*attribute));  // NOLINT
    }

    template <>
    void add_attribute(int position, glm::vec2 Vertex::*attribute) {
        glEnableVertexAttribArray(position);
        glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              &(((Vertex*)nullptr)->*attribute));  // NOLINT
    }

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&) = delete;
    auto operator=(const VertexBuffer&) -> VertexBuffer& = delete;
    auto operator=(VertexBuffer&&) -> VertexBuffer& = delete;
};

#endif  // VERTEXBUFFER_H_
