#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "Device.hpp"

namespace Forge{

class Display{
public:
    Display();
    Display(Device& device, VkInstance instance);
    ~Display();
    Display(const Display& other);
    Display& operator=(const Display& other);


private:
    void CopyFrom(const Display& other);

    Device* device;
    VkInstance instance;

    std::shared_ptr<VkSwapchainKHR> swapchain;
    std::shared_ptr<VkSurfaceKHR> surface;
    std::shared_ptr<std::vector<VkImageView>> swapchainImageViews;
    std::shared_ptr<std::vector<VkFramebuffer>> swapchainFramebuffers;
};


};