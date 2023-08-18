#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>


#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Device.hpp"
#include "Window.hpp"

namespace Forge{

class Display{
public:
    Display();
    Display(VkInstance instance, Window* window);
    ~Display();
    Display(const Display& other);
    Display& operator=(const Display& other);


private:
    void CopyFrom(const Display& other);

    void CreateSurface();
    void CreateSwapchain();

    Window* pWindow;
    VkDevice* pDevice;//for convenience

    VkInstance instance;

    VkSwapchainKHR swapchain;
    VkSurfaceKHR surface;
    std::vector<VkImageView> swapchainImageViews;
    std::vector<VkFramebuffer> swapchainFramebuffers;

    std::shared_ptr<char> instanceCount;

    VkExtent2D surfaceExtent;

    const uint32_t desiredSwapchainImageCount = 2;
    uint32_t swapchainImageCount;
};


};