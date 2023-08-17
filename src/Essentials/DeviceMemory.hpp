#pragma once

#include <iostream>
#include <memory>

#include <vulkan/vulkan.h>

#include "../Core/Device.hpp"

namespace Forge{

class DeviceMemory{
public:
    DeviceMemory();
    ~DeviceMemory();
    DeviceMemory(VkDeviceSize allocationSize, uint32_t memoryTypeIndex);
    DeviceMemory(const DeviceMemory& other);
    DeviceMemory& operator=(const DeviceMemory& other);

    VkDeviceMemory GetMemory();
private:
    VkDeviceMemory memory;
    std::shared_ptr<char> instanceCount;//arbitrary type

};

};