#pragma once
#include "vulkaninit.hpp"
#include "shape.hpp"
#include "Vertex.hpp"
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

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkCommandPool commandPool;
	VkBuffer indexBuffer;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkDeviceMemory indexBufferMemory;
	VkDeviceMemory textureImageMemory;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkImage textureImage;
	VkImageView textureImageView;	
	VkSampler textureSampler;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkDescriptorSet> descriptorSets;
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;


	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();
	void createInstance();
	void setupDebugMessenger();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSurface();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	void drawFrame();
	void createSyncObjects();
	void recreateSwapChain();
	void cleanupSwapChain();
	void createVertexBuffer();
	void createIndexBuffer();
	void createDescriptorSetLayout();
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage);
	void createDescriptorPool();
	void createDescriptorSets();

	void showfps() {
		float current_time = glfwGetTime();
		float fps = 1.0f / (current_time - prev_time);
		prev_time = current_time;
		std::cout << "FPS: " << fps << std::endl;
	}
};