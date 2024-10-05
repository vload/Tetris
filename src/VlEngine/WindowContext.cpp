#include "WindowContext.h"

#include <glad/glad.h>
// This order is important
#include <GLFW/glfw3.h>

#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Events/KeyboardEvents.h"
#include "Events/WindowEvents.h"

constexpr const char* DEFAULT_WINDOW_TITLE = "Tetris";
constexpr int DEFAULT_WINDOW_WIDTH = 1100;
constexpr int DEFAULT_WINDOW_HEIGHT = 1100;
constexpr int OPENGL_VERSION_MAJOR = 4;
constexpr int OPENGL_VERSION_MINOR = 6;

void APIENTRY glDebugOutput(GLenum source, GLenum type,
                            unsigned int debug_message_id, GLenum severity,
                            GLsizei length, const char* message,
                            const void* userParam);

std::vector<WindowContext::FramebufferSizeCallback>
    framebuffer_size_callback_chain;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

void glfw_error_callback(int error, const char* description) {
    throw std::runtime_error("GLFW Error " + std::to_string(error) + ": " +
                             std::string(description));
}

WindowContext::WindowContext() {
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize GLFW
    if (glfwInit() == 0) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Tell GLFW which version of OpenGL to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // Create a GLFW window object
    window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
                              DEFAULT_WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD before calling any OpenGL function
    // clang-format off
    // NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting)
    // clang-format on
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        // clang-format off
        // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting)
        // clang-format on
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow* window, int width, int height) {
            for (auto& callback : framebuffer_size_callback_chain) {
                callback(window, width, height);
            }
        });

    FramebufferSizeCallback default_framebuffer_size_callback =
        [](GLFWwindow*, int width, int height) {
            glViewport(0, 0, width, height);
        };
    add_framebuffer_size_callback(default_framebuffer_size_callback);

    int flags{};
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0) {
        // initialize debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);
    }
}

WindowContext::WindowContext(EventBus& event_bus, int width, int height) {
    glfwSetErrorCallback(glfw_error_callback);

    // Initialize GLFW
    if (glfwInit() == 0) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    // Tell GLFW which version of OpenGL to use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    // Create a GLFW window object
    window =
        glfwCreateWindow(width, height, DEFAULT_WINDOW_TITLE, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD before calling any OpenGL function
    // clang-format off
    // NOLINTBEGIN(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting)
    // clang-format on
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        // clang-format off
        // NOLINTEND(cppcoreguidelines-pro-type-cstyle-cast, google-readability-casting)
        // clang-format on
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);

    int flags{};
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if ((flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0) {
        // initialize debug output
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              nullptr, GL_TRUE);
    }

    glfwSetWindowUserPointer(window, &event_bus);

    // setup callbacks to post key events:
    glfwSetKeyCallback(
        window, [](GLFWwindow* window, int key, int, int action, int) {
            EventBus& event_bus =
                *static_cast<EventBus*>(glfwGetWindowUserPointer(window));

            if (action == GLFW_PRESS) {
                event_bus.post<KeyPressEvent>(key);
            } else if (action == GLFW_RELEASE) {
                event_bus.post<KeyReleaseEvent>(key);
            }
        });

    // setup callbacks for window resizing
    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow* window, int width, int height) {
            EventBus& event_bus =
                *static_cast<EventBus*>(glfwGetWindowUserPointer(window));

            event_bus.post<WindowResizeEvent>(width, height);
        });

    // setup callback for viewport resizing
    event_bus.subscribe<WindowResizeEvent>([](WindowResizeEvent event) {
        glViewport(0, 0, event.width, event.height);
    });

    event_bus.post<WindowResizeEvent>(width, height);
}

void WindowContext::loop() {
    // swap the buffers
    glfwSwapBuffers(window);
    // check and call events
    glfwPollEvents();
}

WindowContext::~WindowContext() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void WindowContext::trigger_framebuffer_size_callbacks() {
    auto [width, height] = get_window_size();
    trigger_framebuffer_size_callbacks(width, height);
}

void WindowContext::trigger_framebuffer_size_callbacks(int width, int height) {
    for (auto& callback : framebuffer_size_callback_chain) {
        callback(window, width, height);
    }
}

auto WindowContext::get_window_size() -> std::pair<int, int> {
    int width{};
    int height{};
    glfwGetFramebufferSize(window, &width, &height);
    return std::make_pair(width, height);
}

auto WindowContext::get_time() -> double { return glfwGetTime(); }

auto WindowContext::should_close() -> bool {
    return glfwWindowShouldClose(window) != 0;
}

void WindowContext::add_framebuffer_size_callback(
    FramebufferSizeCallback& callback) {
    framebuffer_size_callback_chain.push_back(callback);

    trigger_framebuffer_size_callbacks();
}

auto WindowContext::is_key_pressed(int key) -> bool {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

void APIENTRY glDebugOutput(GLenum source, GLenum type,
                            unsigned int debug_message_id, GLenum severity,
                            [[maybe_unused]] GLsizei length,
                            const char* message,
                            [[maybe_unused]] const void* userParam) {
    std::stringstream msg;

    msg << "---------------\n";
    msg << "Debug message (" << debug_message_id << "): " << message << "\n";

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            msg << "Source: API\n";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            msg << "Source: Window System\n";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            msg << "Source: Shader Compiler\n";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            msg << "Source: Third Party\n";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            msg << "Source: Application\n";
            break;
        default:
            msg << "Source: Other\n";
            break;
    }

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            msg << "Type: Error\n";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            msg << "Type: Deprecated Behaviour\n";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            msg << "Type: Undefined Behaviour\n";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            msg << "Type: Portability\n";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            msg << "Type: Performance\n";
            break;
        case GL_DEBUG_TYPE_MARKER:
            msg << "Type: Marker\n";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            msg << "Type: Push Group\n";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            msg << "Type: Pop Group\n";
            break;
        default:
            msg << "Type: Other\n";
            break;
    }

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            msg << "Severity: high\n";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            msg << "Severity: medium\n";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            msg << "Severity: low\n";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            msg << "Severity: notification\n";
            break;
        default:
            msg << "Severity: unknown\n";
            break;
    }
    msg << "\n";

    throw std::runtime_error(msg.str());
}