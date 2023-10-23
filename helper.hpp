#pragma once
VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);

void endSingleTimeCommands(VkCommandBuffer* commandBuffer, VkQueue graphicsQueue, VkDevice device, VkCommandPool commandPool);

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);

void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkQueue graphicsQueue);

std::vector <const char*> getRequiredExtensions();