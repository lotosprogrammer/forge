#pragma once


#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


namespace Forge{

class Window{
public:
    Window();
    Window(glm::vec2 dimensions, std::string title, bool fullscreen);

    Window(const Window& other);
    Window& operator=(const Window& other);
    ~Window();

    glm::vec2 GetDimensions();
    GLFWwindow* GetWindow();
private:
    void CopyFrom(const Window& other);

    GLFWwindow* pWindow = nullptr;
    glm::vec2 dimensions = {0, 0};
    std::string title = {};
    bool isFullscreen = false;

    static uint32_t instanceCount;
};

};

