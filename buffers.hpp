void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer* buffer, VkDeviceMemory* bufferMemory);

void copyBuffer(VkDevice device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool, VkQueue graphicsQueue);


class Buffer {
public:
	VkBuffer buffer;
	VkDeviceMemory memory;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkDeviceSize size;
	void create(VkPhysicalDevice physicalDevice, VkDevice device, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties);
	void copy(void* data, VkCommandPool commandPool, VkQueue graphicsQueue);
	void destroy();
};

class VertexBuffer : public Buffer {
public:
	void create(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, Shape shape);
};

class IndexBuffer : public Buffer {
public:
	void create(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, Shape shape);
};

class UniformBuffer : public Buffer {
public:
	void* data;
	void create(VkPhysicalDevice physicalDevice, VkDevice device);
	void update(VkExtent2D swapChainExtent);
};
