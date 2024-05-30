#pragma once

#include <iostream>

#include "Bind.h"
#include "TetrisBoard.h"
#include "WindowContext.h"

template <class Vertex>
class VertexBuffer {
   private:
    unsigned int VAO;
    unsigned int VBO;

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

    void copy_static_data(const std::vector<Vertex>& vertices) {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     vertices.data(), GL_STATIC_DRAW);
    }

    void copy_dynamic_data(const std::vector<Vertex>& vertices) {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     vertices.data(), GL_DYNAMIC_DRAW);
    }

    template <class U>
    void add_attribute(int position, U Vertex::*attribute) {
        throw std::runtime_error("Unsupported array attribute type: " +
                                 std::string(typeid(U).name()));
    }

    template <>
    void add_attribute(int position, int Vertex::*attribute) {
        glEnableVertexAttribArray(position);
        ptrdiff_t offset = (char*)&(((Vertex*)0)->*attribute) - (char*)0;
        glVertexAttribPointer(position, 1, GL_INT, GL_FALSE, sizeof(Vertex),
                              (void*)offset);
    }

    template <>
    void add_attribute(int position, float Vertex::*attribute) {
        glEnableVertexAttribArray(position);
        ptrdiff_t offset = (char*)&(((Vertex*)0)->*attribute) - (char*)0;
        glVertexAttribPointer(position, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offset);
    }

    template <>
    void add_attribute(int position, glm::vec2 Vertex::*attribute) {
        glEnableVertexAttribArray(position);
        ptrdiff_t offset = (char*)&(((Vertex*)0)->*attribute) - (char*)0;
        glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void*)offset);
    }

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer& operator=(VertexBuffer&&) = delete;
};