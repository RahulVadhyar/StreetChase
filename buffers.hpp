class Buffer {
public:
	Buffer(VkPhysicalDevice physicalDevice, VkDevice device);
	VkBuffer buffer;
	VkDeviceMemory memory;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkDeviceSize size;
	void create(VkBufferUsageFlags flags, VkMemoryPropertyFlags properties);
	void copy(void* data, VkCommandPool commandPool, VkQueue graphicsQueue);
	void destroy();
};

class VertexBuffer : public Buffer {
public:
	VertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, Shape shape);
};

class IndexBuffer : public Buffer {
public:
	IndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, Shape shape);
};

class UniformBuffer : public Buffer {
public:
	void* data;
	UniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device);
	void update(VkExtent2D swapChainExtent);
};
