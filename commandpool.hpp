#pragma once
#include "vulkaninit.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "shape.hpp"
#include "buffers.hpp"


class CommandPool {
public:
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	VkPipeline graphicsPipeline;
	Device device;
	SwapChain swapChain;
	IndexBuffer indexBuffer;
	VertexBuffer vertexBuffer;
	VkPipelineLayout pipelineLayout;
	Rectangle rectangle;
	void createCommandPool(Device device, VkSurfaceKHR surface, SwapChain swapChain, VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout);
	void createCommandBuffers(VertexBuffer vertexBuffer, IndexBuffer indexBuffer, Rectangle rectangle);
	void recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex, VkDescriptorSet* descriptorSet);
	void destroy();

};