#pragma once
void createBuffer(Device device, VkDeviceSize size, VkBufferUsageFlags usage,
                  VkMemoryPropertyFlags properties, VkBuffer *buffer,
                  VkDeviceMemory *bufferMemory);

void copyBuffer(Device device, VkBuffer srcBuffer, VkBuffer dstBuffer,
                VkDeviceSize size, VkCommandPool commandPool);

class Buffer {
public:
  VkBuffer buffer;
  VkDeviceMemory memory;
  Device device;
  VkDeviceSize size;
  void create(Device device, VkBufferUsageFlags flags,
              VkMemoryPropertyFlags properties);
  void copy(void *data, VkCommandPool commandPool);
  void destroy();
};

class UniformBuffer : public Buffer {
public:
  void *data;
  void create(Device device);
  void update(VkExtent2D swapChainExtent, float x, float y, float rotation,
              float x_size, float y_size);
};
