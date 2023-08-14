#pragma once

#include <iostream>
#include <vector>
#include <array>

#include <string.h>

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <glm/glm.hpp>

#include "Window.hpp"
#include "Device.hpp"

namespace Forge{

class App{
public:
    App();
    ~App();
    App(const App& other);
    App& operator=(const App& other);

    void CreateWindow(glm::vec2 dimensions, std::string title, bool fullscreen);
private:
    void CopyFrom(const App& other);
    void CreateInstance();
    void CheckExtensionSupport();

    Window window;
    Device device;

    VkInstance instance = VK_NULL_HANDLE;
    std::vector<const char*> extensions = {
        
    };
    std::vector<const char*> validationLayers = { //TODO check validation layer support
        "VK_LAYER_KHRONOS_validation"
    };

    static bool glfwInitialized;
};
};
