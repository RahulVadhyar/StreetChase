#pragma once
#include "device.hpp"
#include "commandpool.hpp"
#include "shape.hpp"
#include "buffers.hpp"
#include "swapchain.hpp"


class CommandBuffer {
public:
	std::vector<VkCommandBuffer> commandBuffers;
	CommandPool commandPool;
	Rectangle rectangle;

	void create(CommandPool commandPool, Rectangle rectangle);
	void record(uint32_t currentFrame, uint32_t imageIndex, VkDescriptorSet* descriptorSet);
};