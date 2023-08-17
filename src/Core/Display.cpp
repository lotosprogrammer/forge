#include "Display.hpp"

namespace Forge{

Display::Display(){}

Display::Display(VkInstance instance, Window* window){
    instanceCount.reset(new char);
    
    this->pDevice = &Device::GetLogicalDevice();
    this->instance = instance;
    this->pWindow = window;

    CreateSurface();
}

Display::~Display(){
    if(instanceCount.use_count() > 1){
        return;
    }

    for(const VkImageView& imageView : swapchainImageViews){
        vkDestroyImageView(pDevice[0], imageView, nullptr);
    }

    for(const VkFramebuffer& framebuffer : swapchainFramebuffers){
        vkDestroyFramebuffer(pDevice[0], framebuffer, nullptr);
    }

    vkDestroySwapchainKHR(pDevice[0], swapchain, nullptr);

    vkDestroySurfaceKHR(instance, surface, nullptr);

}

Display::Display(const Display& other){
    CopyFrom(other);
}

Display& Display::operator=(const Display& other){
    if(this == &other){
        return *this;
    }
    CopyFrom(other);

    return *this;
}

void Display::CopyFrom(const Display& other){
    swapchain = other.swapchain;
    surface = other.surface;
    swapchainImageViews = other.swapchainImageViews;
    swapchainFramebuffers = other.swapchainFramebuffers;
    instanceCount = other.instanceCount;
}

void Display::CreateSurface(){
    glfwCreateWindowSurface(instance, pWindow->GetWindow(), nullptr, &surface);
}

};