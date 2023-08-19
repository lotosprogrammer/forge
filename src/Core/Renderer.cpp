#include "Renderer.hpp"

namespace Forge{

Renderer::Renderer(){}

Renderer::~Renderer(){

    if(instanceCount.use_count() > 1 || instanceCount.use_count() < 1){
        return;
    }

    for(const VkCommandPool& commandPool : commandPools){
        vkDestroyCommandPool(pDevice[0], commandPool, nullptr);
    }

    vkDestroyPipeline(pDevice[0], graphicsPipeline, nullptr);
}

};