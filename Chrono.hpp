#pragma once
#include "vulkaninit.hpp"
#include "device.hpp"
#include "commandpool.hpp"
#include "shape.hpp"
#include "Vertex.hpp"
#include "buffers.hpp"
#include "swapchain.hpp"

const int MAX_FRAMES_IN_FLIGHT = 2;

class ChronoApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
	bool framebufferResized = false;
private:
	float prev_time = 0.0f;
	int width = 800;
	int height = 600;
	Rectangle rectangle = Rectangle();
	GLFWwindow* window;
	uint32_t currentFrame = 0;
	Device device = Device();
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkCommandPool commandPool;//
	VertexBuffer vertexBuffer = VertexBuffer();
	IndexBuffer indexBuffer = IndexBuffer();
	VkDeviceMemory textureImageMemory;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkImage textureImage;
	VkImageView textureImageView;	
	VkSampler textureSampler;
	SwapChain swapChain = SwapChain();


	/*Classes to create:
	pipeline
	command pool and command buffer
	texture
	*/

	std::vector<VkDescriptorSet> descriptorSets;//
	std::vector<VkCommandBuffer> commandBuffers;//
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<UniformBuffer> uniformBuffers;

	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();
	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	void createCommandPool();//
	void createCommandBuffers();//
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);//
	void drawFrame();
	void createSyncObjects();
	void createDescriptorSetLayout();//
	void createDescriptorPool();//
	void createDescriptorSets();//

	void showfps() {
		float current_time = glfwGetTime();
		float fps = 1.0f / (current_time - prev_time);
		prev_time = current_time;
		std::cout << "FPS: " << fps << std::endl;
	}
};