#pragma once

#include <iostream>
#include <vector>
#include <memory>


#include <vulkan/vulkan.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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

    Window* pWindow;
    VkDevice* pDevice;//for convenience

    VkInstance instance;

    std::shared_ptr<VkSwapchainKHR> swapchain;
    std::shared_ptr<VkSurfaceKHR> surface;
    std::shared_ptr<std::vector<VkImageView>> swapchainImageViews;
    std::shared_ptr<std::vector<VkFramebuffer>> swapchainFramebuffers;
};


};