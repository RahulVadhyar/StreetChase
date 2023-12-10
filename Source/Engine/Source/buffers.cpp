#include "vulkaninit.hpp"
#include "device.hpp"
#include "shape.hpp"
#include "buffers.hpp"
#include "helper.hpp"


void Buffer::create(Device device, VkBufferUsageFlags flags,
                    VkMemoryPropertyFlags properties) {
  this->device = device;
  createBuffer(this->device, size, flags, properties, &buffer, &memory);
}

void Buffer::copy(void *inputData, VkCommandPool commandPool) {
  // copies the data to a staging buffer in order to copy to device buffer
  void *data;
  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  createBuffer(device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
               &stagingBuffer, &stagingBufferMemory);
  vkMapMemory(device.device, stagingBufferMemory, 0, size, 0, &data);
  memcpy(data, inputData, (size_t)size);
  vkUnmapMemory(device.device, stagingBufferMemory);
  copyBuffer(device, stagingBuffer, buffer, size, commandPool);
  vkDestroyBuffer(device.device, stagingBuffer, nullptr);
  vkFreeMemory(device.device, stagingBufferMemory, nullptr);
}

void Buffer::destroy() {
  vkDestroyBuffer(device.device, buffer, nullptr);
  vkFreeMemory(device.device, memory, nullptr);
}

void UniformBuffer::create(Device device) {
  size = sizeof(UniformBufferObject);
  Buffer::create(device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  vkMapMemory(device.device, memory, 0, size, 0, &data);
}

void UniformBuffer::update(VkExtent2D swapChainExtent, float x, float y,
                           float rotation, float x_size, float y_size) {
  // This is where the shape parameters are updated
  UniformBufferObject ubo{};
  ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
  ubo.model = glm::rotate(ubo.model, glm::radians(rotation),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.model = glm::scale(ubo.model, glm::vec3(x_size, y_size, 1.0f));
  ubo.view =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 0.1f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(1.0f, 0.0f, 0.0f));
  ubo.proj =
      glm::ortho(-1.0f, 1.0f,
                 -(float)swapChainExtent.height / (float)swapChainExtent.width,
                 (float)swapChainExtent.height / (float)swapChainExtent.width,
                 -100.0f, 100.0f);
  ubo.proj[1][1] *= -1;
  memcpy(data, &ubo, sizeof(ubo));
}