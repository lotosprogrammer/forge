#include "DeviceMemory.hpp"

namespace Forge{

DeviceMemory::DeviceMemory(){}

DeviceMemory::~DeviceMemory(){
    if(instanceCount.use_count() == 1){
        vkFreeMemory(Device::GetLogicalDevice(), memory, nullptr);
    }
}

DeviceMemory::DeviceMemory(VkDeviceSize allocationSize, uint32_t memoryTypeIndex){
    instanceCount.reset(new char);

    VkMemoryAllocateInfo allocInfo;
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.allocationSize = allocationSize;
    allocInfo.memoryTypeIndex = memoryTypeIndex;

    if(vkAllocateMemory(Device::GetLogicalDevice(), &allocInfo, nullptr, &memory) != VK_SUCCESS){
        throw std::runtime_error("\x1B[31m[ERROR]\033[0m\t\t failed to allocate memory");
    }
}

DeviceMemory::DeviceMemory(const DeviceMemory& other){
     memory = other.memory;
    instanceCount = other.instanceCount;
}

DeviceMemory& DeviceMemory::operator=(const DeviceMemory& other){
    if(this == &other){
        return *this;
    }
    memory = other.memory;
    instanceCount = other.instanceCount;

    return *this;
}

VkDeviceMemory DeviceMemory::GetMemory(){
    return memory;
}

};