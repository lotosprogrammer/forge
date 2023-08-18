#pragma once

#include <iostream>
#include <memory>

#include <vulkan/vulkan.h>

#include "Device.hpp"

namespace Forge{


class ImageView{
public:
    ImageView();
    ImageView(VkImage image, VkImageViewType viewType, VkFormat format, VkComponentMapping components, VkImageSubresourceRange subresourceRange);
    ~ImageView();
    ImageView(const ImageView& other);
    ImageView& operator=(const ImageView& other);

    VkImageView GetImageView();
private:
    void CopyFrom(const ImageView& other);

    std::shared_ptr<char> instanceCount;

    VkImageView imageView;
};

};