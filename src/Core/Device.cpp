#include "Device.hpp"

namespace Forge{

VkDevice Device::logicalDevice = VK_NULL_HANDLE;
VkPhysicalDevice Device::physicalDevice = VK_NULL_HANDLE;

VkInstance Device::instance = VK_NULL_HANDLE;

std::vector<VkQueue> Device::graphicsQueues = {};
std::vector<VkQueue> Device::transferQueues = {};
std::vector<VkDeviceQueueCreateInfo> Device::deviceQueueCreateInfos = {};
std::vector<std::vector<float>> Device::queuePriorities = {};
std::vector<VkQueueFamilyProperties> Device::queueFamilyProperties = {};

std::vector<uint32_t> Device::transferQueueFamilyIndices = {};
std::vector<uint32_t> Device::graphicsQueueFamilyIndices = {};
uint32_t Device::atGraphicsQueueFamilyIndex = 0;
uint32_t Device::atTransferQueueFamilyIndex = 0;

void Device::SetupDevice(VkInstance instance_, std::vector<const char*>& extensions){
    instance = instance_;

    PickPhysicalDevice();
    GetQueueFamilyIndices();

    VkDeviceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.flags = 0; 
    createInfo.pNext = nullptr;
    createInfo.pEnabledFeatures = nullptr;
    createInfo.queueCreateInfoCount = deviceQueueCreateInfos.size();
    createInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
    createInfo.enabledExtensionCount = extensions.size();
    createInfo.ppEnabledExtensionNames = extensions.data();

    vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice);
    GetQueues();
}

void Device::PickPhysicalDevice(){
    uint32_t physicalDeviceCount;

    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);

    vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());
 
    VkPhysicalDeviceType currentDeviceType{};

    for(const VkPhysicalDevice& device : physicalDevices){
        VkPhysicalDeviceProperties properties{};
        
        vkGetPhysicalDeviceProperties(device, &properties);

        if(currentDeviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
            currentDeviceType = properties.deviceType; //ensure that a gpu is always picked and for it to at least be discrete
            physicalDevice = device;
        }
    }

    if(physicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m  Failed to find a GPU, I have no idea why");
    }

}

void Device::GetQueueFamilyIndices(){
    uint32_t queueFamilyCount;
  
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    queueFamilyProperties.resize(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());


    uint32_t queueFamilyIndex = 0;


    VkDeviceQueueCreateInfo deviceQueueCreateInfo;
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.pNext = nullptr;
    deviceQueueCreateInfo.flags = 0;

    for(const VkQueueFamilyProperties& queueFamily : queueFamilyProperties){
        if((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT){
            deviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            deviceQueueCreateInfo.queueCount = queueFamily.queueCount;
            
            std::vector<float> queuePriority(queueFamily.queueCount);
            std::fill(queuePriority.begin(), queuePriority.end(), 1.0f);

            queuePriorities.push_back(queuePriority);

            deviceQueueCreateInfo.pQueuePriorities = queuePriorities[queuePriorities.size()-1].data();

            deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
        } // all graphics queues are also transfer queues
        queueFamilyIndex++;
    }

}

void Device::GetQueues(){
    uint32_t queueFamilyIndex = 0;
    for(const VkQueueFamilyProperties& queueFamily : queueFamilyProperties){
        if((queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) != VK_QUEUE_GRAPHICS_BIT){
            transferQueueFamilyIndices.push_back(queueFamilyIndex);
            for(uint32_t i = 0; i < queueFamily.queueCount; i++){
                VkQueue queue;
                vkGetDeviceQueue(logicalDevice, queueFamilyIndex, i, &queue);
                transferQueues.push_back(queue);
            }
        }
        else if((queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT){
            graphicsQueueFamilyIndices.push_back(queueFamilyIndex);
            for(uint32_t i = 0; i < queueFamily.queueCount; i++){
                VkQueue queue;
                vkGetDeviceQueue(logicalDevice, queueFamilyIndex, i, &queue);
                graphicsQueues.push_back(queue);
            }
        }
        queueFamilyIndex++;
    }

}

DeviceQueueFamily Device::GetQueueFamilyIndices(uint32_t queueBits){
    DeviceQueueFamily deviceQueue;
    
    if((queueBits & GRAPHICS_QUEUE) == GRAPHICS_QUEUE){
        deviceQueue.queueFamilyIndices.push_back(graphicsQueueFamilyIndices[atGraphicsQueueFamilyIndex]);
        atGraphicsQueueFamilyIndex++;
        if(atGraphicsQueueFamilyIndex == graphicsQueueFamilyIndices.size()){
            atGraphicsQueueFamilyIndex = 0;
        }
    }
    if((queueBits & TRANSFER_QUEUE) == TRANSFER_QUEUE){
        deviceQueue.queueFamilyIndices.push_back(transferQueueFamilyIndices[atTransferQueueFamilyIndex]);
        atTransferQueueFamilyIndex++;
        if(atTransferQueueFamilyIndex == transferQueueFamilyIndices.size()){
            atTransferQueueFamilyIndex = 0;
        }
    }

    return deviceQueue;
}

uint32_t Device::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties){
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++){
        if(typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties){//TODO comprehend
            return i;
        }
    }

    throw std::runtime_error("\x1B[31m[ERROR]\033[0m  Failed to find suitable memory type");
}	

VkDevice& Device::GetLogicalDevice(){
    return logicalDevice;
}

VkPhysicalDevice& Device::GetPhysicalDevice(){
    return physicalDevice;
}

void Device::CleanupDevice(){
    vkDestroyDevice(logicalDevice, nullptr);
}

};