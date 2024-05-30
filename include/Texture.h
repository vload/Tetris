#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "WindowContext.h"

class Texture {
   private:
    unsigned int texture{};
    int textureUnit;

   public:
    explicit Texture(const std::string& path, int textureUnit = 0);
    ~Texture();

    void bind();
    void unbind();

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    auto operator=(const Texture&) -> Texture& = delete;
    auto operator=(Texture&&) -> Texture& = delete;
};

#endif  // TEXTURE_H_
