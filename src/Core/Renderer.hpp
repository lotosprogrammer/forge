#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include <vulkan/vulkan.h>

#include "Device.hpp"

namespace Forge{

#define GRAPHICS_COMMAND_POOL 0
#define TRANSFER_COMMAND_POOL 1
#define COMPUTE_COMMAND_POOL 2

class Renderer{
public:
    Renderer();
    //Renderer();
    ~Renderer();
    Renderer(const Renderer& other);
    Renderer& operator=(const Renderer& other);

    //CommandBuffer BeginCommandBuffer(uint32_t queueType);
    // void EndCommandBuffer(CommandBuffer commandBuffer);

private:
    VkDevice* pDevice;

    void CreateGraphicsPipeline();
    void CopyFrom(const Renderer& other);

    std::shared_ptr<char> instanceCount;

    std::vector<VkCommandPool> commandPools;
    
    VkPipeline graphicsPipeline;

};

};