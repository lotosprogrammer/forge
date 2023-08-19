#include "ImageView.hpp"

namespace Forge{

ImageView::ImageView(){}

ImageView::ImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkComponentMapping components, VkImageSubresourceRange subresourceRange){
    instanceCount.reset(new char);

    VkImageViewCreateInfo createInfo;
    
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.format = format;
    createInfo.image = image;
    createInfo.viewType = viewType;
    createInfo.components = components;
    createInfo.subresourceRange = subresourceRange;

    if(vkCreateImageView(Device::GetLogicalDevice(), &createInfo, nullptr, &imageView) != VK_SUCCESS){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m  failed to create an image view");
    }
}

ImageView::ImageView(const ImageView& other){
    CopyFrom(other);
}

ImageView& ImageView::operator=(const ImageView& other){
    if(this == &other){
        return *this;
    }

    CopyFrom(other);

    return *this;
}

void ImageView::CopyFrom(const ImageView& other){
    imageView = other.imageView;
}

VkImageView ImageView::GetImageView(){
    return imageView;
}

ImageView::~ImageView(){
    if(instanceCount.use_count() == 1){
        vkDestroyImageView(Device::GetLogicalDevice(), imageView, nullptr);
    }
}

};