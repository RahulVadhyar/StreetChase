#pragma once
#include "vulkaninit.hpp"
#include "Vertex.hpp"
#include "device.hpp"
#include "swapChain.hpp"
#include "buffers.hpp"
#include "texture.hpp"

struct ShapeParams {
	float x;
	float y;
	float rotation;
	float xSize;
	float ySize;
};

class Shape {
public:
	const char* vertexShaderPath = "G:/Chronos/Shaders/vert.spv";
	const char* fragmentShaderPath = "G:/Chronos/Shaders/frag.spv";
	ShapeParams params;
	std::vector<uint16_t> indices;
	std::vector<Vertex> vertices;
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
	void init(Device device, VkCommandPool commandPool, SwapChain* swapChain, VkSampler textureSampler, std::string texturePath);
	void createGraphicsPipeline();
	void createDescriptorPool();
	void createDescriptorSetLayout();
	void createDescriptorSets();
	void update(uint32_t currentFrame);
	void destroy();
};

class Rectangle: public Shape {
public:
	Rectangle() {
		indices = std::vector<uint16_t>{ 0, 1, 2, 2, 3, 0 };
		vertices = std::vector<Vertex>{
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {0.5f, 1.0f, 0.5f}, {1.0f, 1.0f}}
		};
	}
};

class Triangle : public Shape {
public:
	Triangle() {
		indices = std::vector<uint16_t>{ 0, 1, 2 };
		vertices = std::vector<Vertex>{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
		};
	}

};

class Circle : public Shape {
public:
	Circle() {
		indices = std::vector<uint16_t>{ 0, 1, 2, 2, 3, 0 };
		vertices = std::vector<Vertex>{
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {0.5f, 1.0f, 0.5f}, {1.0f, 1.0f}}
		};
		vertexShaderPath = "circle_vert.spv";
		fragmentShaderPath = "circle_frag.spv";

	}
};


struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};