#pragma once
#include "vulkaninit.hpp"
#include "device.hpp"
#include "commandpool.hpp"
#include "shape.hpp"
#include "Vertex.hpp"
#include "buffers.hpp"
#include "swapchain.hpp"
#include "descriptorpool.hpp"
#include "texture.hpp"
#include "commandbuffer.hpp"

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
	Texture texture = Texture();
	VkSampler textureSampler;
	SwapChain swapChain = SwapChain();
	CommandPool commandPool = CommandPool();
	DescriptorPool descriptorPool = DescriptorPool();
	CommandBuffer commandBuffer = CommandBuffer();

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
	void drawFrame();
	void createSyncObjects();

	void showfps() {
		float current_time = glfwGetTime();
		float fps = 1.0f / (current_time - prev_time);
		prev_time = current_time;
		std::cout << "FPS: " << fps << std::endl;
	}
};