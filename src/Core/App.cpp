#include "App.hpp"

#include "../Essentials/Image.hpp"

namespace Forge{


bool App::glfwInitialized = false;

App::App(){
    if(!glfwInitialized){
        if(!glfwInit()){
            throw std::runtime_error("\x1B[31m[ERROR]\033[0m\t\t failed to init GLFW3");
        }

        glfwInitialized = true;
    }

    CreateInstance();
    Device::SetupDevice(instance, deviceExtensions);
}    

App::App(const App& other){
    CopyFrom(other);
}

App& App::operator=(const App& other){
    if(this == &other){
        return *this;
    }

    CopyFrom(other);

    return *this;
}

App::~App(){
    Device::CleanupDevice();
    vkDestroyInstance(instance, nullptr); 
}

void App::CopyFrom(const App& other){
    instance = other.instance;
    window = other.window;
}

void App::CreateInstance(){
    uint32_t requiredGLFWExtensionCount;
    const char** glfwRequiredExtensions = glfwGetRequiredInstanceExtensions(&requiredGLFWExtensionCount);
    for(size_t i = 0; i < requiredGLFWExtensionCount; i++){
        extensions.push_back(glfwRequiredExtensions[i]);
    }


    CheckExtensionSupport();

    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledExtensionCount = extensions.size();
    
#ifdef _DEBUG 
    createInfo.enabledLayerCount = validationLayers.size();
    createInfo.ppEnabledLayerNames = validationLayers.data();
#else
    createInfo.enabledLayerCount = 0;
#endif

    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    
    appInfo.apiVersion = VK_MAKE_API_VERSION(1, 2, 0, 0);
    appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    appInfo.engineVersion = VK_MAKE_API_VERSION(1, 0, 0, 0);
    appInfo.pApplicationName = "UNNAMED";
    appInfo.pEngineName = "FORGE";

    createInfo.pApplicationInfo = &appInfo;

    createInfo.flags = 0;
    createInfo.pNext = nullptr;

    if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m\t\t failed to create a vulkan instance");
    }
}

void App::CheckExtensionSupport(){
    uint32_t availableExtensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

    for(const char* required : extensions){
        bool match = false;  
        for(const VkExtensionProperties& available : availableExtensions){
            if(strcmp(available.extensionName, required)){
                match = true;
            }
        }
        if(!match){
            throw std::runtime_error("\x1B[31m[ERROR]\033[0m\t\t not all required extensions are supported, check your GPU drivers");
        }
    }

}

void App::CreateWindow(glm::vec2 dimensions, std::string title, bool fullscreen){
    window = Window(dimensions, title, fullscreen);

    Display display = Display(instance, &window);
}

};
