#include "Display.hpp"


namespace Forge{

Display::Display(){}

Display::Display(VkInstance instance, Window* window){
    instanceCount.reset(new char);
    
    this->pDevice = &Device::GetLogicalDevice();
    this->instance = instance;
    this->pWindow = window;

    CreateSurface();
    CreateSwapchain();
}

Display::~Display(){
    if(instanceCount.use_count() > 1 || instanceCount.use_count() < 1){
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
    if(glfwCreateWindowSurface(instance, pWindow->GetWindow(), nullptr, &surface) != VK_SUCCESS){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m\t\t failed to create a window surface");
    }

    VkSurfaceCapabilitiesKHR capabilites;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Device::GetPhysicalDevice(), surface, &capabilites);

    swapchainImageCount = std::clamp(desiredSwapchainImageCount, capabilites.minImageCount, capabilites.maxImageCount);
    surfaceExtent = capabilites.currentExtent;
}

void Display::CreateSwapchain(){

    uint32_t surfaceFormatCount;

    vkGetPhysicalDeviceSurfaceFormatsKHR(Device::GetPhysicalDevice(), surface, &surfaceFormatCount, nullptr);

    std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);

    vkGetPhysicalDeviceSurfaceFormatsKHR(Device::GetPhysicalDevice(), surface, &surfaceFormatCount, surfaceFormats.data());

    VkSurfaceFormatKHR bestFormat{};

    for(const VkSurfaceFormatKHR& format : surfaceFormats){
        if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR){
            bestFormat = format;
        }
    }
    if(bestFormat.format == VK_FORMAT_UNDEFINED){
        bestFormat = surfaceFormats[0];
    }



    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.surface = surface;
    createInfo.minImageCount = swapchainImageCount;
    createInfo.imageColorSpace = bestFormat.colorSpace;//todo find the best one
    createInfo.imageExtent = surfaceExtent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageFormat = bestFormat.format;
    createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; 
    createInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
    createInfo.clipped = true;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.oldSwapchain = VK_NULL_HANDLE;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    DeviceQueueFamily deviceQueueFamily = Device::GetQueueFamilyIndices(GRAPHICS_QUEUE | TRANSFER_QUEUE);

    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.pQueueFamilyIndices = deviceQueueFamily.queueFamilyIndices.data();
    createInfo.queueFamilyIndexCount = deviceQueueFamily.queueFamilyIndices.size();

    if(vkCreateSwapchainKHR(pDevice[0], &createInfo, nullptr, &swapchain) != VK_SUCCESS){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m\t\t Failed to create a swapchain");
    }
}

};