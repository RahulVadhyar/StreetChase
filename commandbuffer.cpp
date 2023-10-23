#include "vulkaninit.hpp"
#include "device.hpp"
#include "commandbuffer.hpp"

void CommandBuffer::create(CommandPool commandPool, Rectangle rectangle) {	
	this->commandPool = commandPool;
	this->rectangle = rectangle;
	commandBuffers.resize(commandPool.swapChain.swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool.commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	if (vkAllocateCommandBuffers(commandPool.device.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void CommandBuffer::record(uint32_t currentFrame, uint32_t imageIndex, VkDescriptorSet* descriptorSet) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = commandPool.swapChain.renderPass;
	renderPassInfo.framebuffer = commandPool.swapChain.swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = commandPool.swapChain.swapChainExtent;
	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, commandPool.graphicsPipeline);
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float> (commandPool.swapChain.swapChainExtent.width);
	viewport.height = static_cast<float> (commandPool.swapChain.swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = commandPool.swapChain.swapChainExtent;
	vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);

	VkBuffer vertexBuffers[] = { rectangle.vertexBuffer.buffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffers[currentFrame], rectangle.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
	vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, commandPool.pipelineLayout, 0, 1, descriptorSet, 0, nullptr);
	vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(rectangle.indices.size()), 1, 0, 0, 0);
	vkCmdEndRenderPass(commandBuffers[currentFrame]);

	if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}