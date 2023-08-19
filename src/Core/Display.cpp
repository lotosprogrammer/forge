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
    CreateSwapchainImageViews();
    CreateRenderPass();
    CreateFramebuffers();
}

Display::~Display(){
    if(instanceCount.use_count() > 1 || instanceCount.use_count() < 1){
        return;
    }



    for(const VkFramebuffer& framebuffer : swapchainFramebuffers){
        vkDestroyFramebuffer(pDevice[0], framebuffer, nullptr);
    }

    vkDestroyRenderPass(pDevice[0], renderPass, nullptr);

    for(const ImageView* imageView : swapchainImageViews){
        delete imageView;
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
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m  failed to create a window surface");
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
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m Failed to create a swapchain");
    }

    swapchainImageFormat = bestFormat.format;
}

void Display::CreateSwapchainImageViews(){
    uint32_t swapChainImageCount;

    vkGetSwapchainImagesKHR(pDevice[0], swapchain, &swapchainImageCount, nullptr);

    std::vector<VkImage> swapchainImages(swapchainImageCount);

    vkGetSwapchainImagesKHR(pDevice[0], swapchain, &swapChainImageCount, swapchainImages.data());

    VkImageSubresourceRange subresourceRange;
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel = 0;
    subresourceRange.levelCount = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount = 1;

    VkComponentMapping components;
    components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    components.a = VK_COMPONENT_SWIZZLE_IDENTITY;


    for(const VkImage& swapchainImage : swapchainImages){
        swapchainImageViews.push_back(new ImageView(swapchainImage, VK_IMAGE_VIEW_TYPE_2D, swapchainImageFormat, components, subresourceRange));
    }
}

void Display::CreateFramebuffers(){
    swapchainFramebuffers.resize(swapchainImageCount);

    VkFramebufferCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.renderPass = renderPass;
    createInfo.width = surfaceExtent.width;
    createInfo.height = surfaceExtent.height;
    createInfo.layers = 1;
    uint32_t i = 0;
    for(ImageView* imageView : swapchainImageViews){
        const std::vector<VkImageView> attachments = {
            imageView->GetImageView()
        };

        createInfo.attachmentCount = attachments.size();
        createInfo.pAttachments = attachments.data();


        if(vkCreateFramebuffer(pDevice[0], &createInfo, nullptr, &swapchainFramebuffers[i]) != VK_SUCCESS){
            throw std::runtime_error("\x1B[31m[ERROR]\033[0m failed to create a framebuffer");
        }

        i++;
    }
}

void Display::CreateRenderPass(){
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapchainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;


    VkRenderPassCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.attachmentCount = 1;
    createInfo.pAttachments = &colorAttachment;
    createInfo.subpassCount = 1;
    createInfo.pSubpasses = &subpass;

    if(vkCreateRenderPass(pDevice[0], &createInfo, nullptr, &renderPass) != VK_SUCCESS){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m failed to create a render pass");
    }
}

}; 