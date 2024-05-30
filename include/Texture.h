#pragma once

#include "WindowContext.h"

class Texture {
   private:
    unsigned int texture;
    int textureUnit;

   public:
    Texture(const std::string& path, int textureUnit = 0);
    ~Texture();

    void bind();
    void unbind();

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;
};