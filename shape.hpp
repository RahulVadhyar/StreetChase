#pragma once
#include "Vertex.hpp"
#include "device.hpp"
#include "buffers.hpp"
class Shape {
public:
	std::vector<uint16_t> indices{ 0, 1, 2, 2, 3, 0 };
	std::vector<Vertex> vertices{
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};
};

class Rectangle: public Shape {
public:
	Device device;
	CommandPool commandPool;
	Buffer vertexBuffer;
	Buffer indexBuffer;
	void init(Device device, CommandPool commandPool);
	void destroy();
};
struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};