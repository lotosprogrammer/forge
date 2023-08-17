

#include "Window.hpp"

namespace Forge{




uint32_t Window::instanceCount = 0;

Window::Window(){instanceCount++;}

Window::Window(glm::vec2 dimensions, std::string title, bool fullscreen){
    instanceCount++;
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    pWindow = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if(pWindow == nullptr){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m\t\t failed to create a glfw window");
    }

    this->dimensions = dimensions;
    this->title = title;
    isFullscreen = fullscreen;
}

Window::Window(const Window& other){
    CopyFrom(other);
}

Window& Window::operator=(const Window& other){
    if(this == &other){return *this;}

    CopyFrom(other);

    return *this;
}

Window::~Window(){
    instanceCount--;

    glfwDestroyWindow(pWindow);

    if(instanceCount == 0){
        glfwTerminate();
    }
}

void Window::CopyFrom(const Window& other){
    dimensions = other.dimensions;
    title = other.title;
    isFullscreen = other.isFullscreen;
    pWindow = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), isFullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    instanceCount++;
}

GLFWwindow* Window::GetWindow(){
    return pWindow;
}

glm::vec2 Window::GetDimensions(){
    return dimensions;
}

};
