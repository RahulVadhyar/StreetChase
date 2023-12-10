#include "vulkaninit.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "helper.hpp"
#include "texture.hpp"

VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) {
  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {
  // The presentation modes available
  // VK_PRESENT_MODE_IMMEDIATE_KHR: Images submitted by your application are
  // transferred to the screen right away, which may result in tearing.
  // VK_PRESENT_MODE_FIFO_KHR : The swap chain is a queue where the display
  // takes an image from the front of the queue when the display is refreshed
  //							and the program inserts rendered images at the
  //back of the queue.If the queue is full then the program has to wait. 							This is
  //most similar to vertical sync as found in modern games.The moment that the
  //display is refreshed is known as 							"vertical blank".
  // VK_PRESENT_MODE_FIFO_RELAXED_KHR : This mode only differs from the previous
  // one if the application is late and the queue was empty at the last 									vertical
  //blank.Instead of waiting for the next vertical blank, the image is
  //transferred right away when it 									finally arrives.This may result in visible
  //tearing. VK_PRESENT_MODE_MAILBOX_KHR : This is another variation of the
  // second mode.Instead of blocking the application when the queue is full, the
  // images 							that are already queued are simply replaced with the newer ones.This
  //mode can be used to render frames as fast as 							possible while still avoiding
  //tearing, resulting in fewer latency issues than standard vertical sync.This
  //is commonly 							known as "triple buffering", although the existence of three
  //buffers alone does not necessarily mean that the framerate 							is unlocked.
  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                            GLFWwindow *window) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                              VkSurfaceKHR surface) {
  SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}
void SwapChain::init(Device *device, VkSurfaceKHR surface, GLFWwindow *window) {
  this->device = device;
  this->surface = surface;
  this->window = window;
  create();
  createImageViews();
  createColorResources();
}

void SwapChain::create() {
  // create the swapchain
  SwapChainSupportDetails swapChainSupport =
      querySwapChainSupport((*device).physicalDevice, surface);
  VkSurfaceFormatKHR surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

  // set the number of images in the swap chain
  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

  // check if the number of images exceeds the maximum
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = surface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices =
      findQueueFamilies(device->physicalDevice, surface);
  uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                   indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;     // Optional
    createInfo.pQueueFamilyIndices = nullptr; // Optional
  }
  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = nullptr;

  if (vkCreateSwapchainKHR(device->device, &createInfo, nullptr, &swapChain) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create swap chain");
  }
  vkGetSwapchainImagesKHR(device->device, swapChain, &imageCount, nullptr);
  swapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(device->device, swapChain, &imageCount,
                          swapChainImages.data());
  swapChainImageFormat = surfaceFormat.format;
  swapChainExtent = extent;
}

void SwapChain::createImageViews() {
  swapChainImageViews.resize(swapChainImages.size());
  for (size_t i = 0; i < swapChainImages.size(); i++) {
    swapChainImageViews[i] =
        createImageView(*device, swapChainImageFormat, swapChainImages[i]);
  }
}

void SwapChain::createColorResources() {
  VkFormat colorFormat = swapChainImageFormat;
  createImage(*device, swapChainExtent.width, swapChainExtent.height,
              colorFormat, VK_IMAGE_TILING_OPTIMAL,
              VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
                  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
              VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &colorImage,
              &colorImageMemory, device->msaaSamples);

  colorImageView = createImageView(*device, colorFormat, colorImage);
}

void SwapChain::cleanup() {
  vkDestroyImageView(device->device, colorImageView, nullptr);
  vkDestroyImage(device->device, colorImage, nullptr);
  vkFreeMemory(device->device, colorImageMemory, nullptr);
  // destroy the image views
  for (auto imageView : swapChainImageViews) {
    vkDestroyImageView(device->device, imageView, nullptr);
  }
  // destroy the swap chain
  vkDestroySwapchainKHR(device->device, swapChain, nullptr);
}

void SwapChain::recreate() {
  int width = 0, height = 0;
  glfwGetFramebufferSize(window, &width, &height);
  while (width == 0 || height == 0) {
    glfwGetFramebufferSize(window, &width, &height);
    glfwWaitEvents();
  }

  vkDeviceWaitIdle(device->device);

  cleanup();
  create();
  createImageViews();
  createColorResources();
}

void SwapChain::changeMsaa() {
  vkDeviceWaitIdle(device->device);
  cleanup();
  create();
  createImageViews();
  createColorResources();
}