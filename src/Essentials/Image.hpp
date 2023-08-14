#pragma once

#include <iostream>

#include <vulkan/vulkan.h>

namespace Forge{

class Image{
public:
    Image();
    Image(VkImageCreateFlags flags, uint32_t mipLevels, VkExtent2D extent,
     VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharingMode);
    ~Image();
    Image(const Image& other);
    Image& operator=(const Image& other);

    VkImage GetImage();
private:

    VkImage image;
    VkDeviceMemory memory;

};
//queue families handled by scheduler
}