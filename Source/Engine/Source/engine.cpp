#include "vulkaninit.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "engine.hpp"
#include "validation.hpp"
#include "helper.hpp"
#include "Vertex.hpp"


static void framebuffer_size_callback(GLFWwindow *window, int width,
                                      int height) {
  auto app =
      reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));
  app->framebufferResized = true;
}

void Engine::run() {
#ifdef DISPLAY_IMGUI
		gui = GUI();
		guiParams.settings = &settings;
#endif
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

void Engine::initWindow() {
  // initialize glfw with a resizeable window
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  window = glfwCreateWindow(width, height, "Chrono", nullptr, nullptr);
  if (window == nullptr) {
    throw std::runtime_error("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Engine::initVulkan() {
  createInstance();
  if (enableValidationLayers) {
    setupDebugMessenger();
  }
  createSurface();
  device.init(instance, surface);

  // sync the msaa settings with gui
#ifdef DISPLAY_IMGUI
  settings.maxMsaaSamples = device.msaaSamples;
  settings.msaaSamples = device.msaaSamples;
#endif

  swapChain.init(&device, surface, window);
  createTextureSampler(device, &textureSampler);
  commandPool = createCommandPool(device, swapChain.surface);
  shapeManager.init(&device, &swapChain, commandPool, textureSampler);
  textManager.init(&device, commandPool, &swapChain);
  createSyncObjects();

#ifdef DISPLAY_IMGUI
  gui.init(&device, window, &swapChain, instance, surface, &guiParams);
  guiParams.bgColor = bgColor;
  guiParams.settings = &settings;
  guiParams.shapeManager = &shapeManager;
#endif
}

void Engine::mainLoop() {
  while (!glfwWindowShouldClose(window)) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
    glfwPollEvents();
    drawFrame();
  }
  vkDeviceWaitIdle(device.device);
}

void Engine::cleanup() {
  // after we are done, we need to cleanup all the resources we created
  swapChain.cleanup();
  vkDestroySampler(device.device, textureSampler, nullptr);
  shapeManager.destroy();
  textManager.destroy();
#ifdef DISPLAY_IMGUI
  gui.destroy();
#endif
  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(device.device, renderFinishedSemaphores[i], nullptr);
    vkDestroySemaphore(device.device, imageAvailableSemaphores[i], nullptr);
    vkDestroyFence(device.device, inFlightFences[i], nullptr);
  }
  vkDestroyCommandPool(device.device, commandPool, nullptr);
  if (enableValidationLayers) {
    DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
  }
  device.destroy(); // destroy the logical device
  vkDestroySurfaceKHR(instance, surface, nullptr);
  vkDestroyInstance(instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Engine::drawFrame() {
  // wait for the previous frame to finish
  vkWaitForFences(device.device, 1, &inFlightFences[currentFrame], VK_TRUE,
                  UINT64_MAX);

  // get the index of the next image to render to
  uint32_t imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      device.device, swapChain.swapChain, UINT64_MAX,
      imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

  // if window has been minimized, then recreate the swap chain and other things
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    swapChain.recreate();
    shapeManager.recreate();
    textManager.recreate();
#ifdef DISPLAY_IMGUI
    gui.recreate();
#endif
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Failed to acquire swap chain image");
  }
  // update the shapes and text
  shapeManager.update(currentFrame);
  textManager.beginUpdate();
  textManager.add("Hello world", 0.0f, 0.0f, Text::Center);
  textManager.endUpdate();
  gui.update();

  // reset the fences
  vkResetFences(device.device, 1, &inFlightFences[currentFrame]);

  // record the command buffers
  shapeManager.render(currentFrame, imageIndex, bgColor);
  textManager.render(currentFrame, imageIndex, bgColor);
#ifdef DISPLAY_IMGUI
  gui.render(currentFrame, imageIndex, bgColor);
#endif

  // configure the semaphores
  VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};

  // submit the command buffers
  std::vector<VkCommandBuffer> submitCommandBuffers;
  submitCommandBuffers.push_back(shapeManager.commandBuffers[currentFrame]);
  submitCommandBuffers.push_back(textManager.commandBuffers[currentFrame]);

#ifdef DISPLAY_IMGUI
  submitCommandBuffers.push_back(gui.commandBuffers[currentFrame]);
#endif
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount =
      static_cast<uint32_t>(submitCommandBuffers.size());
  submitInfo.pCommandBuffers = submitCommandBuffers.data();
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(device.graphicsQueue, 1, &submitInfo,
                    inFlightFences[currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("failed to submit draw command buffer!");
  }

  // present the image
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;
  VkSwapchainKHR swapChains[] = {swapChain.swapChain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;
  result = vkQueuePresentKHR(device.presentQueue, &presentInfo);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      framebufferResized) {
    framebufferResized = false;
    swapChain.recreate();
    shapeManager.recreate();
    textManager.recreate();
#ifdef DISPLAY_IMGUI
    gui.recreate();
#endif
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to present swap chain image");
  }

  // update the current frame
  currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Engine::createInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Chrono";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Chrono Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  // using vulkan 1.3 as we need shader printf support
  appInfo.apiVersion = VK_API_VERSION_1_3;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  uint32_t glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  auto extensions = getRequiredExtensions();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  if (enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error("Validation layers requested, but not available");
  }

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

  createInfo.enabledLayerCount = 0;
  createInfo.pNext = nullptr;

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    // uncomment below if u need fine details. It just creates extra verbose
    // generally not needed
    //  populateDebugMessengerCreateInfo(debugCreateInfo);
    //  createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)
    //  &debugCreateInfo;
  }

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create instance");
  }
}

void Engine::setupDebugMessenger() {
  if (!enableValidationLayers)
    return;
  VkDebugUtilsMessengerCreateInfoEXT createInfo;
  populateDebugMessengerCreateInfo(createInfo);
  if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr,
                                   &debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("Failed to set up debug messenger");
  }
}

void Engine::createSurface() {
  if (glfwCreateWindowSurface(instance, window, nullptr, &surface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface");
  }
}

void Engine::createSyncObjects() {
  imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
    if (vkCreateSemaphore(device.device, &semaphoreInfo, nullptr,
                          &imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(device.device, &semaphoreInfo, nullptr,
                          &renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(device.device, &fenceInfo, nullptr, &inFlightFences[i]) !=
            VK_SUCCESS) {

      throw std::runtime_error(
          "failed to create synchronization objects for a frame!");
    }
  }
}