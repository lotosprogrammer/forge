#pragma once

#include <iostream>
#include <vector>
#include <array>

#include <vulkan/vulkan.h>

namespace Forge{

class Device{
public:
    Device();

    static VkPhysicalDevice& GetPhysicalDevice();
    static VkDevice& GetLogicalDevice();

    void SetupDevice(VkInstance instance);
    void CleanupDevice();

private:

    std::vector<VkQueue> graphicsQueues;
    std::vector<VkQueue> transferQueues;

    std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
    std::vector<std::vector<float>> queuePriorities;
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;


    void PickPhysicalDevice();
    void GetQueueFamilyIndices();
    void GetQueues();


    static VkPhysicalDevice physicalDevice;
    static VkDevice logicalDevice;

    VkInstance instance = VK_NULL_HANDLE;
};

};