#include "Display.hpp"

namespace Forge{

Display::Display(){}

Display::Display(Device& device, VkInstance instance){
    this->device = &device;
    this->instance = instance;
}

Display::~Display(){
    if(swapchain.use_count() > 1){
        return;
    }

    for(const VkImageView& imageView : swapchainImageViews.get()[0]){
        vkDestroyImageView(device->GetLogicalDevice(), imageView, nullptr);
    }

    for(const VkFramebuffer& framebuffer : swapchainFramebuffers.get()[0]){
        vkDestroyFramebuffer(device->GetLogicalDevice(), framebuffer, nullptr);
    }

    vkDestroySwapchainKHR(device->GetLogicalDevice(), swapchain.get()[0], nullptr);

    vkDestroySurfaceKHR(instance, surface.get()[0], nullptr);

    swapchain.reset();
    surface.reset();
    swapchainFramebuffers.reset();
    swapchainImageViews.reset();
}

Display::Display(const Display& other){
    CopyFrom(other);
}

Display& Display::operator=(const Display& other){
    CopyFrom(other);
}

void Display::CopyFrom(const Display& other){
    swapchain = other.swapchain;
    surface = other.surface;
    swapchainImageViews = other.swapchainImageViews;
    swapchainFramebuffers = other.swapchainFramebuffers;
}

};