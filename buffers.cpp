#include "vulkaninit.hpp"
#include "shape.hpp"
#include "buffers.hpp"
#include "helper.hpp"

void Buffer::create(VkPhysicalDevice physicalDevice, VkDevice device, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties) {
	this->physicalDevice = physicalDevice;
	this->device = device;
	createBuffer(this->physicalDevice, this->device, size, flags, properties, &buffer, &memory);
}

void Buffer::copy(void* inputData, VkCommandPool commandPool, VkQueue graphicsQueue) {
	void* data;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(physicalDevice, device, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);
	vkMapMemory(device, stagingBufferMemory, 0, size, 0, &data);
	memcpy(data, inputData, (size_t)size);
	vkUnmapMemory(device, stagingBufferMemory);
	copyBuffer(device, stagingBuffer, buffer, size, commandPool, graphicsQueue);
	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Buffer::destroy() {
	vkDestroyBuffer(device, buffer, nullptr);
	vkFreeMemory(device, memory, nullptr);
}

void VertexBuffer::create(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, Shape shape){
	size = sizeof(shape.vertices[0]) * shape.vertices.size();
	Buffer::create(physicalDevice, device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	this->copy(shape.vertices.data(), commandPool, graphicsQueue);
}
void IndexBuffer::create(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, Shape shape){
	size = sizeof(shape.indices[0]) * shape.indices.size();
	Buffer::create(physicalDevice, device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	this->copy(shape.indices.data(), commandPool, graphicsQueue);
}

void UniformBuffer::create(VkPhysicalDevice physicalDevice, VkDevice device){
	size = sizeof(UniformBufferObject);
	Buffer::create(physicalDevice, device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	vkMapMemory(device, memory, 0, size, 0, &data);
}

void UniformBuffer::update(VkExtent2D swapChainExtent) {
	static auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
	UniformBufferObject ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), (float)swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;
	memcpy(data, &ubo, sizeof(ubo));
}