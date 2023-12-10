#pragma once
#include "vulkaninit.hpp"
#include "device.hpp"
#include "shape.hpp"
#include "swapchain.hpp"


class ShapeManager {
public:
  Device *device;
  SwapChain *swapChain;
  VkCommandPool commandPool;

  std::vector<Shape> shapes;
  std::vector<VkCommandBuffer> commandBuffers;
  VkRenderPass renderPass;
  VkSampler textureSampler;
  std::vector<VkFramebuffer> framebuffers;

  void init(Device *device, SwapChain *swapChain, VkCommandPool commandPool,
            VkSampler textureSampler);
  void addTriangle(ShapeParams shapeParams, std::string texturePath);
  void addRectangle(ShapeParams shapeParams, std::string texturePath);
  void destroy();
  void update(uint32_t currentFrame);
  void render(uint32_t currentFrame, uint32_t imageIndex, float bgColor[3]);
  void changeMsaa();
  void recreate();
  void cleanup();
};
