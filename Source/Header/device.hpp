#pragma once
class Device {
public:
  VkDevice device;
  VkPhysicalDevice physicalDevice;
  VkQueue graphicsQueue;
  VkQueue presentQueue;
  VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
  void init(VkInstance instance, VkSurfaceKHR surface);
  void destroy();

private:
  void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
  void createLogicalDevice(VkSurfaceKHR surface);
};