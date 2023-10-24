#pragma once
#include "vulkaninit.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "buffers.hpp"

class CommandPool {
public:
	VkCommandPool commandPool;
	VkPipeline graphicsPipeline;
	Device device;
	SwapChain* swapChain;
	VkPipelineLayout pipelineLayout;
	void createCommandPool(Device device, VkSurfaceKHR surface, SwapChain* swapChain, VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout);
	void destroy();

};