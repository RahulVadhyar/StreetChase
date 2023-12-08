#pragma once
#include "device.hpp"
#include "vulkaninit.hpp"

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                            GLFWwindow *window);
VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats);
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                              VkSurfaceKHR surface);

class SwapChain {
public:
  Device *device;

  VkSwapchainKHR swapChain;
  VkFormat swapChainImageFormat;
  VkExtent2D swapChainExtent;
  VkSurfaceKHR surface;
  GLFWwindow *window;
  VkImage colorImage;
  VkDeviceMemory colorImageMemory;
  VkImageView colorImageView;
  std::vector<VkImage> swapChainImages;
  std::vector<VkImageView> swapChainImageViews;

  void init(Device *device, VkSurfaceKHR surface, GLFWwindow *window);
  void create();
  void recreate();
  void cleanup();
  void createImageViews();
  void createColorResources();
  void changeMsaa();
};