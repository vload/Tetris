#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "WindowContext.h"

class Texture {
   private:
    unsigned int texture{};
    int texture_unit;

   public:
    explicit Texture(const std::string& path, int texture_unit = 0);
    ~Texture();

    void bind() const;
    void unbind() const;

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    auto operator=(const Texture&) -> Texture& = delete;
    auto operator=(Texture&&) -> Texture& = delete;
};

#endif  // TEXTURE_H_
