#include "Texture.h"

#include "Bind.h"

// stb
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string& path, int textureUnit)
    : textureUnit(textureUnit) {
    int width, height, nrChannels;
    unsigned char* data =
        stbi_load(path.data(), &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load texture: " + path);
    }
    // generate the texture
    glGenTextures(1, &texture);

    // bind the texture
    auto tb = Bind(*this);

    // enable blocky textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);

    // free the image data
    stbi_image_free(data);
}
Texture::~Texture() { glDeleteTextures(1, &texture); }

void Texture::bind() {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind() {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}
