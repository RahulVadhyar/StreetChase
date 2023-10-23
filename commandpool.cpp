#include "vulkaninit.hpp"
#include "device.hpp"
#include "commandpool.hpp"
#include "helper.hpp"
#include "swapchain.hpp"

void CommandPool::createCommandPool(Device device, VkSurfaceKHR surface, SwapChain swapChain, VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout) {
	this->device = device;
	this->swapChain = swapChain;
	this->graphicsPipeline = graphicsPipeline;
	this->pipelineLayout = pipelineLayout;

	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(device.physicalDevice, surface);
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional
	if (vkCreateCommandPool(device.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

void CommandPool::destroy() {
	vkDestroyCommandPool(device.device, commandPool, nullptr);

}