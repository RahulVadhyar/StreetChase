#pragma once

struct QueueFamilyIndices {
	std::optional <uint32_t> graphicsFamily;
	std::optional <uint32_t> presentFamily;
	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);

void endSingleTimeCommands(VkCommandBuffer* commandBuffer, VkQueue graphicsQueue, VkDevice device, VkCommandPool commandPool);

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice physicalDevice);

void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);

void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkQueue graphicsQueue);

std::vector <const char*> getRequiredExtensions();

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
bool checkDeviceExtensionSupport(VkPhysicalDevice device);
bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);