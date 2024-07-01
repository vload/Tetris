#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <glad/glad.h>

#include "Bind.h"

class Texture {
   private:
    GLenum target;
    int texture_unit;
    GLuint ID{};

   public:
    explicit Texture(GLenum target, int texture_unit = 0)
        : target(target), texture_unit(texture_unit) {
        glGenTextures(1, &ID);
    }
    ~Texture() { glDeleteTextures(1, &ID); }

    void load_image(const char* const path);

    void bind() const {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(target, ID);
    }

    void unbind() const {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(target, 0);
        glActiveTexture(GL_TEXTURE0);
    }

    void bind(GLenum access) {
        glBindImageTexture(texture_unit, ID, 0, GL_FALSE, 0, access, GL_RGBA8);
    }
    void unbind(GLenum access) {
        glBindImageTexture(texture_unit, 0, 0, GL_FALSE, 0, access, GL_RGBA8);
    }

    GLuint get() const { return ID; }

    void set_parameter(GLenum pname, GLint param) {
        auto texture_bind = Bind(*this);
        glTexParameteri(target, pname, param);
    }
    void set_parameter(GLenum pname, GLfloat param) {
        auto texture_bind = Bind(*this);
        glTexParameterf(target, pname, param);
    }

    // parameter presests
    void set_linear() {
        set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void set_nearest() {
        set_parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        set_parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    auto operator=(const Texture&) -> Texture& = delete;
    auto operator=(Texture&&) -> Texture& = delete;
};

#endif  // TEXTURE_H_
