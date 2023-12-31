#include "vulkaninit.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "shapeManager.hpp"
#include "helper.hpp"


void ShapeManager::init(Device *device, SwapChain *swapChain,
                        VkCommandPool commandPool, VkSampler textureSampler) {
  this->device = device;
  this->swapChain = swapChain;
  this->commandPool = commandPool;
  this->textureSampler = textureSampler;

  /*
  For the render pass the formats are as follows:
          initial - undefined; this is due to the fact that image is cleared
          final(color and msaa) - color attachment optimal; this is due to the
  fact that the image going to next renderpass(Text) Also we are clearing the
  framebuffer
  */
  renderPass = createRenderPass(*device, *swapChain, VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, true,
                                true, false);
  commandBuffers = createCommandBuffer(*device, *swapChain, commandPool);
  framebuffers = createFramebuffer(*device, *swapChain, renderPass, true);
}

void ShapeManager::destroy() {
  vkDestroyRenderPass(device->device, renderPass, nullptr);
  for (auto framebuffer : framebuffers)
    vkDestroyFramebuffer(device->device, framebuffer, nullptr);
  for(auto& shapeMap: shapes){
    shapeMap.second.destroy();
  }
}

void ShapeManager::update(uint32_t currentFrame) {
  for (auto &shapeMap : shapes) {
    shapeMap.second.update(currentFrame);
  }
}

void ShapeManager::render(uint32_t currentFrame, uint32_t imageIndex,
                          float bgColor[3]) {
  vkResetCommandBuffer(commandBuffers[currentFrame], 0);
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = renderPass;
  renderPassInfo.framebuffer = framebuffers[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = swapChain->swapChainExtent;
  VkClearValue clearColor = {bgColor[0], bgColor[1], bgColor[2],
                             1.0f}; // bg color
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(swapChain->swapChainExtent.width);
  viewport.height = static_cast<float>(swapChain->swapChainExtent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = swapChain->swapChainExtent;

  vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  // render the shapes
  for(auto& shape: shapes){
    vkCmdBindPipeline(commandBuffers[currentFrame],
                      VK_PIPELINE_BIND_POINT_GRAPHICS, shape.second.graphicsPipeline);
    vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);
    VkBuffer vertexBuffers[] = {shape.second.vertexBuffer.buffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers,
                           offsets);
    vkCmdBindIndexBuffer(commandBuffers[currentFrame], shape.second.indexBuffer.buffer,
                         0, VK_INDEX_TYPE_UINT16);
    vkCmdBindDescriptorSets(commandBuffers[currentFrame],
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            shape.second.pipelineLayout, 0, 1,
                            &shape.second.descriptorSets[currentFrame], 0, nullptr);
    vkCmdDrawIndexed(commandBuffers[currentFrame],
                     static_cast<uint32_t>(shape.second.indices.size()), 1, 0, 0, 0);
  }
  vkCmdEndRenderPass(commandBuffers[currentFrame]);

  if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}
void ShapeManager::changeMsaa() {
  vkDestroyRenderPass(device->device, renderPass, nullptr);
  renderPass = createRenderPass(*device, *swapChain, VK_IMAGE_LAYOUT_UNDEFINED,
                                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, true,
                                true, false);
}

int ShapeManager::addRectangle(ShapeParams shapeParams,
                                std::string texturePath) {
  int shapeNo = shapes.size();
  shapes[shapeNo] = Rectangle();
  shapes[shapeNo].init(device, commandPool, swapChain, textureSampler,
                     texturePath, &renderPass);
  shapes[shapeNo].params = shapeParams;
  return shapeNo;
}

int ShapeManager::addTriangle(ShapeParams shapeParams,
                               std::string texturePath) {
  
  int shapeNo = nextFreeShapeNo;
  nextFreeShapeNo++;
  shapes[shapeNo] = Triangle();
  shapes[shapeNo].init(device, commandPool, swapChain, textureSampler,
                     texturePath, &renderPass);
  shapes[shapeNo].params = shapeParams;
  return shapeNo;
  }
void ShapeManager::removeShape(int shapeNo) {
  shapes[shapeNo].destroy();
  shapes.erase(shapeNo);
}

void ShapeManager::cleanup() {
  for (auto framebuffer : framebuffers)
    vkDestroyFramebuffer(device->device, framebuffer, nullptr);
}

void ShapeManager::recreate() {
  cleanup();
  framebuffers = createFramebuffer(*device, *swapChain, renderPass, true);
}