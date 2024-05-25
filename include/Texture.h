#pragma once

#include "util.h"

class Texture{
   private:
    unsigned int texture;

   public:
    Texture(const std::string& path);
    ~Texture();

    void bind();
    void bind(int placeholder);
    void unbind();
    void unbind(int placeholder);

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;
};