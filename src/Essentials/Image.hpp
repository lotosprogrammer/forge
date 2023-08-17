#pragma once

#include <iostream>

#include <vulkan/vulkan.h>

#include "../Core/Device.hpp"
#include "DeviceMemory.hpp"

namespace Forge{

class Image{
public:
    Image();
    Image(VkImageCreateFlags flags, uint32_t mipLevels, VkExtent2D extent,
     VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharingMode, VkImageLayout layout, uint32_t usageBits);
    ~Image();
    Image(const Image& other);
    Image& operator=(const Image& other);

    VkImage GetImage();
private:
    void CopyFrom(const Image& other);

    std::shared_ptr<char> instanceCount; //arbitraryType

    VkImage image;
    DeviceMemory memory;//todo, make it a class

};
//queue families handled by scheduler
}