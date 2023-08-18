#include "Image.hpp"

namespace Forge{

Image::Image(){}

Image::Image(VkImageCreateFlags flags, uint32_t mipLevels, VkExtent2D extent,
    VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharingMode, VkImageLayout layout, uint32_t usageBits, VkFormat format){

    instanceCount.reset(new char);

    VkImageCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = flags;
    createInfo.mipLevels = mipLevels;
    createInfo.extent = {extent.width, extent.height, 1};
    createInfo.samples = samples;
    createInfo.tiling = tiling;
    createInfo.usage = usage;
    createInfo.sharingMode = sharingMode;
    createInfo.initialLayout = layout;
    createInfo.arrayLayers = 1;
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.format = VK_FORMAT_R8G8B8A8_SRGB;

    DeviceQueueFamily deviceQueue = Device::GetQueueFamilyIndices(usageBits);

    createInfo.pQueueFamilyIndices = deviceQueue.queueFamilyIndices.data();
    createInfo.queueFamilyIndexCount = deviceQueue.queueFamilyIndices.size();


    if(vkCreateImage(Device::GetLogicalDevice(), &createInfo, nullptr, &image) != VK_SUCCESS){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m\t\t failed to create an image");
    }


    VkMemoryRequirements requirements{};

    vkGetImageMemoryRequirements(Device::GetLogicalDevice(), image, &requirements);
    
    memory = DeviceMemory(requirements.size, Device::FindMemoryType(requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));

    vkBindImageMemory(Device::GetLogicalDevice(), image, memory.GetMemory(), 0);
}

Image::~Image(){
    if(instanceCount.use_count() == 1){
        vkDestroyImage(Device::GetLogicalDevice(), image, nullptr);
    
    }
}

Image::Image(const Image& other){
    CopyFrom(other);
}

Image& Image::operator=(const Image& other){
    if(this == &other){
        return *this;
    }
    CopyFrom(other);

    return *this;
}

void Image::CopyFrom(const Image& other){
    image = other.image;
    memory = other.memory;
    instanceCount = other.instanceCount;
}

};