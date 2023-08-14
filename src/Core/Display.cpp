#include "Display.hpp"

namespace Forge{

Display::Display(){}

Display::Display(VkInstance instance, Window* window){
    this->device = &Device::GetLogicalDevice();
    this->instance = instance;
    this->window = window;

    CreateSurface();
}

Display::~Display(){
    if(swapchain.use_count() > 1){
        return;
    }

    for(const VkImageView& imageView : swapchainImageViews.get()[0]){
        vkDestroyImageView(device[0], imageView, nullptr);
    }

    for(const VkFramebuffer& framebuffer : swapchainFramebuffers.get()[0]){
        vkDestroyFramebuffer(device[0], framebuffer, nullptr);
    }

    vkDestroySwapchainKHR(device[0], swapchain.get()[0], nullptr);

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
    CopyFrom(other);
}

void Display::CopyFrom(const Display& other){
    swapchain = other.swapchain;
    surface = other.surface;
    swapchainImageViews = other.swapchainImageViews;
    swapchainFramebuffers = other.swapchainFramebuffers;
}

void Display::CreateSurface(){
    glfwCreateWindowSurface(instance, window->GetWindow(), nullptr, surface.get());
}

};