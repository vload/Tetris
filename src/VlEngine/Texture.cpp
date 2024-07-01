#include "Texture.h"

#include <glad/glad.h>

#include <stdexcept>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Bind.h"

void Texture::load_image(const char* const path) {
    int width{};
    int height{};
    int nrChannels{};
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        throw std::runtime_error("Failed to load texture: " +
                                 std::string(path));
    }

    // bind the texture
    auto texture_bind = Bind(*this);

    // load the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    // free the image data
    stbi_image_free(data);
}