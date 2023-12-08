#pragma once
#include "swapchain.hpp"
#include "vulkaninit.hpp"

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;
  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};
VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool,
                                        VkDevice device);

void endSingleTimeCommands(VkCommandBuffer *commandBuffer, Device device,
                           VkCommandPool commandPool);

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,
                        VkPhysicalDevice physicalDevice);

void createBuffer(Device device, VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer *buffer,
                  VkDeviceMemory *bufferMemory);

void copyBuffer(Device device, VkBuffer srcBuffer, VkBuffer dstBuffer,
                VkDeviceSize size, VkCommandPool commandPool);

std::vector<const char *> getRequiredExtensions();

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device,
                                     VkSurfaceKHR surface);
bool checkDeviceExtensionSupport(VkPhysicalDevice device);
bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

VkCommandPool createCommandPool(Device device, VkSurfaceKHR surface);
VkRenderPass createRenderPass(Device device, SwapChain swapChain,
                              VkImageLayout initalLayout,
                              VkImageLayout finalLayout,
                              VkImageLayout msaaFinalLayout, bool msaa,
                              bool clearFramebuffer, bool dependency);
std::vector<VkFramebuffer> createFramebuffer(Device device, SwapChain swapChain,
                                             VkRenderPass renderPass,
                                             bool msaa);
std::vector<VkCommandBuffer> createCommandBuffer(Device device,
                                                 SwapChain swapChain,
                                                 VkCommandPool commandPool);
std::vector<char> readFile(const std::string &filename);
VkShaderModule createShaderModule(const std::vector<char> &code,
                                  VkDevice device);
VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice physicalDevice);