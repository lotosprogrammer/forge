#include "Image.hpp"

namespace Forge{

Image::Image(){}

Image::Image(VkImageCreateFlags flags, uint32_t mipLevels, VkExtent2D extent,
     VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VkSharingMode sharingMode){


}

Image::~Image(){
    vkDestroyImage(Device::GetLogicalDevice(), image, nullptr);
}

Image::Image(const Image& other){
    CopyFrom(other);
}

Image& Image::operator=(const Image& other){
    CopyFrom(other);
}

void Image::CopyFrom(const Image& other){
    image = other.image;
    memory = other.memory;
}

};