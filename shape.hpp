#pragma once
#include "vulkaninit.hpp"
#include "Vertex.hpp"
#include "device.hpp"
#include "swapChain.hpp"
#include "buffers.hpp"
#include "texture.hpp"
class Shape {
public:
	std::vector<uint16_t> indices{ 0, 1, 2, 2, 3, 0 };
	std::vector<Vertex> vertices{
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {0.5f, 1.0f, 0.5f}, {1.0f, 1.0f}}
	};
};

class Rectangle: public Shape {
public:
	Device device;
	SwapChain* swapChain;
	VkCommandPool commandPool;
	VkSampler textureSampler;

	Buffer vertexBuffer;
	Buffer indexBuffer;
	Texture texture;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	


	std::vector<UniformBuffer> uniformBuffers;
	void init(Device device, VkCommandPool commandPool, SwapChain* swapChain, VkSampler textureSampler);
	void createGraphicsPipeline();
	void createDescriptorPool();
	void createDescriptorSetLayout();
	void createDescriptorSets();
	void update(uint32_t currentFrame, uint32_t pos);
	void destroy();
};

class Triangle : public Shape {};

class Circle : public Shape {};


struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};