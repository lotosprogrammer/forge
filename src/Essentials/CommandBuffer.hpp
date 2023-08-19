#pragma once

#include <iostream>
#include <memory>

#include <vulkan/vulkan.h>

#include "../Core/Device.hpp"

namespace Forge{

class CommandBuffer{
public:
    CommandBuffer();
    CommandBuffer(VkCommandPool commandPool);
    ~CommandBuffer();
    CommandBuffer(const CommandBuffer& other);
    CommandBuffer& operator=(const CommandBuffer& other);

    VkCommandBuffer GetCommandBuffer();

    void Record(VkCommandBufferUsageFlagBits usage);
    void StopRecording();
    void Submit();//uwu
private:
    void CopyFrom(const CommandBuffer& other);

    VkCommandPool commandPool;

    std::shared_ptr<char> instanceCount;
    VkCommandBuffer commandBuffer;

};

};