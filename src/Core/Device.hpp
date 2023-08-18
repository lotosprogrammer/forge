#pragma once

#include <iostream>
#include <vector>
#include <array>

#include <vulkan/vulkan.h>

namespace Forge{

#define GRAPHICS_QUEUE 1
#define TRANSFER_QUEUE 2
#define COMPUTE_QUEUE 4

struct DeviceQueueFamily{
    std::vector<uint32_t> queueFamilyIndices = {};
};

class Device{
public:
    static VkPhysicalDevice& GetPhysicalDevice();
    static VkDevice& GetLogicalDevice();

    static DeviceQueueFamily GetQueueFamilyIndices(uint32_t queueBits);
    static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


    static void SetupDevice(VkInstance instance);
    static void CleanupDevice();

private:

    static std::vector<VkQueue> graphicsQueues;
    static std::vector<VkQueue> transferQueues;
    
    static std::vector<uint32_t> graphicsQueueFamilyIndices;
    static std::vector<uint32_t> transferQueueFamilyIndices;
    static uint32_t atGraphicsQueueFamilyIndex;
    static uint32_t atTransferQueueFamilyIndex;

    static std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
    static std::vector<std::vector<float>> queuePriorities;
    static std::vector<VkQueueFamilyProperties> queueFamilyProperties;


    static void PickPhysicalDevice();
    static void GetQueueFamilyIndices();
    static void GetQueues();


    static VkPhysicalDevice physicalDevice;
    static VkDevice logicalDevice;

    static VkInstance instance;
};//statik members go BRRRRRR

};