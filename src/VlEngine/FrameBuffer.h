#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <glad/glad.h>

#include "Events/EventBus.h"
#include "Events/WindowEvents.h"
#include "Texture.h"

class FrameBuffer {
   private:
    unsigned int ID{};
    Texture& texture;

   public:
    explicit FrameBuffer(EventBus& event_bus, Texture& texture)
        : texture(texture) {
        texture.set_linear();

        glGenFramebuffers(1, &ID);

        // subscribe to window resize events and update the texture size
        event_bus.subscribe<WindowResizeEvent>(
            [this](const WindowResizeEvent& event) {
                auto texture_bind = Bind(this->texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, event.width,
                             event.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                             nullptr);
            });

        auto framebuffer_bind = Bind(*this);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, texture.get(), 0);
    }
    ~FrameBuffer() { glDeleteFramebuffers(1, &ID); }

    void bind() const {
        texture.bind();
        glBindFramebuffer(GL_FRAMEBUFFER, ID);
    }
    void unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void bind(GLenum target) const { glBindFramebuffer(target, ID); }
    void unbind(GLenum target) const { glBindFramebuffer(target, 0); }

    FrameBuffer(const FrameBuffer&) = delete;
    FrameBuffer(FrameBuffer&&) = delete;
    auto operator=(const FrameBuffer&) -> FrameBuffer& = delete;
    auto operator=(FrameBuffer&&) -> FrameBuffer& = delete;
};

#endif  // FRAMEBUFFER_H_
