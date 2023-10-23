#pragma once

void createTextureSampler(Device device, VkSampler* textureSampler);
void createImage(Device device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage* image, VkDeviceMemory* imageMemory);
void createTextureImage(Device device, VkImage* textureImage, VkDeviceMemory* textureImageMemory, VkCommandPool commandPool);
void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandPool commandPool, Device device);
void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, VkCommandPool commandPool, Device device);
VkImageView createImageView(VkImage image, VkFormat format, VkDevice device);
