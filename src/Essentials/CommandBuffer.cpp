#include "CommandBuffer.hpp"

namespace Forge{

CommandBuffer::CommandBuffer(){}

CommandBuffer::CommandBuffer(VkCommandPool commandPool){
    instanceCount.reset(new char);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    this->commandPool = commandPool;

    if(!vkAllocateCommandBuffers(Device::GetLogicalDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m failed to allocate a command buffer");
    }
}

CommandBuffer::~CommandBuffer(){
    if(instanceCount.use_count() == 1){
        vkFreeCommandBuffers(Device::GetLogicalDevice(), commandPool, 1, &commandBuffer);
    }
}

CommandBuffer::CommandBuffer(const CommandBuffer& other){
    CopyFrom(other);
}

CommandBuffer& CommandBuffer::operator=(const CommandBuffer& other){
    if(this == &other){
        return *this;
    }

    CopyFrom(other);

    return *this;
}

void CommandBuffer::CopyFrom(const CommandBuffer& other){
    commandBuffer = other.commandBuffer;
    commandPool = other.commandPool;
    instanceCount = other.instanceCount;
}

void CommandBuffer::Record(VkCommandBufferUsageFlagBits usage){
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.flags = usage;

    if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m failed to begin recording the command buffer");
    }
}

void CommandBuffer::StopRecording(){
    if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS){
        throw std::runtime_error("Failed to end the command buffer recording");
    }
}

VkCommandBuffer CommandBuffer::GetCommandBuffer(){
    return commandBuffer;
}

};