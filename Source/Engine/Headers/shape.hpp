#pragma once
#include "vulkaninit.hpp"
#include "Vertex.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "buffers.hpp"
#include "texture.hpp"
#include "helper.hpp"
#include "structs.hpp"



class Shape {
public:
  const char *vertexShaderPath = "G:/StreetChase/ThirdParty/Chronos/Shaders/vert.spv";
  const char *fragmentShaderPath = "G:/StreetChase/ThirdParty/Chronos/Shaders/frag.spv";
  ShapeParams params;
  std::vector<uint16_t> indices;
  std::vector<Vertex> vertices;
  Device *device;
  SwapChain *swapChain;
  VkCommandPool commandPool;
  VkSampler textureSampler;
  VkRenderPass *renderPass;

  Buffer vertexBuffer;
  Buffer indexBuffer;
  Texture texture;
  std::vector<VkDescriptorSet> descriptorSets;
  VkDescriptorSetLayout descriptorSetLayout;
  VkDescriptorPool descriptorPool;
  VkPipelineLayout pipelineLayout;
  VkPipeline graphicsPipeline;
  std::vector<UniformBuffer> uniformBuffers;
  void init(Device *device, VkCommandPool commandPool, SwapChain *swapChain,
            VkSampler textureSampler, std::string texturePath,
            VkRenderPass *renderPass);
  void createGraphicsPipeline();
  void createDescriptorPool();
  void createDescriptorSetLayout();
  void createDescriptorSets();
  void update(uint32_t currentFrame);
  void destroy();
  void recreateGraphicsPipeline();
};

class Rectangle : public Shape {
public:
  Rectangle() {
    indices = std::vector<uint16_t>{0, 1, 2, 2, 3, 0};
    vertices =
        std::vector<Vertex>{{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                            {{-0.5f, 0.5f}, {0.5f, 1.0f, 0.5f}, {1.0f, 1.0f}}};
  }
};

class Triangle : public Shape {
public:
  Triangle() {
    indices = std::vector<uint16_t>{0, 1, 2};
    vertices =
        std::vector<Vertex>{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}};
  }
};

class Circle : public Shape {
public:
  Circle() {
    indices = std::vector<uint16_t>{0, 1, 2, 2, 3, 0};
    vertices =
        std::vector<Vertex>{{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                            {{-0.5f, 0.5f}, {0.5f, 1.0f, 0.5f}, {1.0f, 1.0f}}};
    vertexShaderPath = "circle_vert.spv";
    fragmentShaderPath = "circle_frag.spv";
  }
};

