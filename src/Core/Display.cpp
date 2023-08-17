#include "Display.hpp"

namespace Forge{

Display::Display(){}

Display::Display(VkInstance instance, Window* window){
    this->pDevice = &Device::GetLogicalDevice();
    this->instance = instance;
    this->pWindow = window;

    CreateSurface();
}

Display::~Display(){
    if(swapchain.use_count() > 1){
        return;
    }

    for(const VkImageView& imageView : swapchainImageViews.get()[0]){
        vkDestroyImageView(pDevice[0], imageView, nullptr);
    }

    for(const VkFramebuffer& framebuffer : swapchainFramebuffers.get()[0]){
        vkDestroyFramebuffer(pDevice[0], framebuffer, nullptr);
    }

    vkDestroySwapchainKHR(pDevice[0], swapchain.get()[0], nullptr);

    vkDestroySurfaceKHR(instance, surface.get()[0], nullptr);

    swapchain.reset();
    surface.reset();
    swapchainFramebuffers.reset();
    swapchainImageViews.reset(); // IMAGEVIEW AND BUFFER CLASSES PROS: easier resource managament, simpler, faster, easeier to debug,
    //CONS:
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
}

void Display::CreateSurface(){
    glfwCreateWindowSurface(instance, pWindow->GetWindow(), nullptr, surface.get());
}

};