#ifndef BUFFEROBJECT_H_
#define BUFFEROBJECT_H_

#include <glad/glad.h>

#include <span>

#include "Bind.h"

// https://www.khronos.org/opengl/wiki/Buffer_Object
template <class T>
class BufferObject {
   private:
    GLuint ID{};

   public:
    BufferObject() { glGenBuffers(1, &ID); }

    ~BufferObject() { glDeleteBuffers(1, &ID); }

    void bind(GLenum target) const { glBindBuffer(target, ID); }

    void unbind(GLenum target) const { glBindBuffer(target, 0); }

    void bind(GLenum target, GLuint index) const {
        glBindBuffer(target, ID);
        glBindBufferBase(target, index, ID);
    }

    void unbind(GLenum target, GLuint index) const {
        glBindBufferBase(target, index, 0);
        glBindBuffer(target, 0);
    }

    void copy_data(const std::span<const T> data, GLenum usage) const {
        auto copy_buffer_bind = Bind(*this, GL_COPY_WRITE_BUFFER);
        glBufferData(GL_COPY_WRITE_BUFFER, data.size() * sizeof(T), data.data(),
                     usage);
    }

    BufferObject(const BufferObject&) = delete;
    BufferObject(BufferObject&&) = delete;
    auto operator=(const BufferObject&) -> BufferObject& = delete;
    auto operator=(BufferObject&&) -> BufferObject& = delete;
};

#endif  // BUFFEROBJECT_H_