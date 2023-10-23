#pragma once


void createTextureSampler(VkPhysicalDevice physicalDevice, VkDevice device, VkSampler* textureSampler);
void createImage(VkPhysicalDevice physicalDevice, VkDevice device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* imageMemory);
void createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice, VkImage* textureImage, VkDeviceMemory* textureImageMemory, VkCommandPool commandPool, VkQueue graphicsQueue);
void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue);
void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkCommandPool commandPool, VkDevice device, VkQueue graphicsQueue);
VkImageView createImageView(VkImage image, VkFormat format, VkDevice device);
