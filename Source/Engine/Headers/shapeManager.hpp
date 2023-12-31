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
  int nextFreeShapeNo = 0;
  std::map<int, Shape> shapes;
  std::vector<VkCommandBuffer> commandBuffers;
  VkRenderPass renderPass;
  VkSampler textureSampler;
  std::vector<VkFramebuffer> framebuffers;

  void init(Device *device, SwapChain *swapChain, VkCommandPool commandPool,
            VkSampler textureSampler);
  int addTriangle(ShapeParams shapeParams, std::string texturePath);
  int addRectangle(ShapeParams shapeParams, std::string texturePath);
  void removeShape(int shapeNo);
  void destroy();
  void update(uint32_t currentFrame);
  void render(uint32_t currentFrame, uint32_t imageIndex, float bgColor[3]);
  void changeMsaa();
  void recreate();
  void cleanup();
};
