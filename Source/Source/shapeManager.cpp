#include "vulkaninit.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "shapeManager.hpp"
#include "helper.hpp"

void ShapeManager::init(Device* device, SwapChain* swapChain, VkCommandPool commandPool, VkSampler textureSampler){
	this->device = device;
	this->swapChain = swapChain;
	this->commandPool = commandPool;
	this->textureSampler = textureSampler;
#ifdef DISPLAY_IMGUI
	renderPass = createRenderPass(*device, *swapChain, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, true, true, false);
#else 
	renderPass = createRenderPass(*device, *swapChain, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, true, true);
#endif
	commandBuffers = createCommandBuffer(*device, *swapChain, commandPool);
	framebuffers = createFramebuffer(*device, *swapChain, renderPass, true);

	std::cout << "Command Buffers(ShapeManager): " << std::endl;
	for(auto commandBuffer : commandBuffers)
		std::cout << commandBuffer << std::endl;


	shapes.push_back(Rectangle());
	shapes.push_back(Triangle());
	for(Shape& shape : shapes){
		shape.init(device, commandPool, swapChain, textureSampler, "G:/Chronos/Assets/texture.jpg", &renderPass);
	}
	shapes[0].params.x = 0.5;
	shapes[0].params.y = 0.5;
	shapes[0].params.xSize = 0.5;
	shapes[0].params.ySize = 0.5;
	shapes[0].params.rotation = 0;

	shapes[1].params.x = -0.5;
	shapes[1].params.y = -0.5;
	shapes[1].params.xSize = 0.5;
	shapes[1].params.ySize = 0.5;
	shapes[1].params.rotation = 0;
}

void ShapeManager::destroy() {
	vkDestroyRenderPass(device->device, renderPass, nullptr);
	for (Shape& shape : shapes) {
		shape.destroy();
	}
}

void ShapeManager::update(uint32_t currentFrame) {
	for (Shape shape : shapes) {
		shape.update(currentFrame);
	}
}

void ShapeManager::render(uint32_t currentFrame, uint32_t imageIndex, float bgColor[3]) {
	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = framebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain->swapChainExtent;
	VkClearValue clearColor = { bgColor[0], bgColor[1], bgColor[2], 1.0f };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float> (swapChain->swapChainExtent.width);
	viewport.height = static_cast<float> (swapChain->swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChain->swapChainExtent;

	vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	//render the shapes
	for (Shape& shape : shapes) {
		vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, shape.graphicsPipeline);
		vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);
		VkBuffer vertexBuffers[] = { shape.vertexBuffer.buffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffers[currentFrame], shape.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, shape.pipelineLayout, 0, 1, &shape.descriptorSets[currentFrame], 0, nullptr);
		vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(shape.indices.size()), 1, 0, 0, 0);
	}
	vkCmdEndRenderPass(commandBuffers[currentFrame]);

	if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}
void ShapeManager::changeMsaa() {
	vkDestroyRenderPass(device->device, renderPass, nullptr);
#ifdef DISPLAY_IMGUI
	renderPass = createRenderPass(*device, *swapChain, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, true, true, false);
#else 
	renderPass = createRenderPass(*device, *swapChain, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, true, true);
#endif
}

void ShapeManager::addRectangle(ShapeParams shapeParams, std::string texturePath) {
	shapes.push_back(Triangle());
	shapes.back().init(device, commandPool, swapChain, textureSampler, texturePath, &renderPass);
	shapes.back().params = shapeParams;
}

void ShapeManager::addTriangle(ShapeParams shapeParams, std::string texturePath) {
	shapes.push_back(Triangle());
	shapes.back().init(device, commandPool, swapChain, textureSampler, texturePath, &renderPass);
	shapes.back().params = shapeParams;
}