#pragma once
#include "vulkaninit.hpp"
#include "device.hpp"
#include "shape.hpp"
#include "Vertex.hpp"
#include "buffers.hpp"
#include "swapchain.hpp"
#include "texture.hpp"
#ifdef DISPLAY_IMGUI
#include "gui.hpp"
#endif
const int MAX_FRAMES_IN_FLIGHT = 2;

struct PostProcessing {
	float brightness = 1.0f;
	float saturation = 1.0f;
	float exposure = 1.0f;
	float brilliance = 1.0f;
	float highlights = 1.0f;
	float shadows = 1.0f;
	float contrast = 1.0f;
	float blackPoint = 0.0f;
	float vibrancy = 1.0f;
	float warmth = 1.0f;
	float tint = 1.0f;
	float sharpness = 1.0f;
	float vignette = 1.0f;
};

struct Settings {
	PostProcessing postProcessing;
	int msaaSamples = 1;
	int maxMsaaSamples = 1;
};

class ChronoApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}
	bool framebufferResized = false;
	Settings settings{};
#ifdef DISPLAY_IMGUI
	GUIParams guiParams{};
#endif
private:
	float prev_time = 0.0f;
	int width = 800;
	int height = 600;
	float bgColor[3] = { 0.0f, 0.0f, 0.0f };
	std::vector<Shape> shapes;
	GLFWwindow* window;
	uint32_t currentFrame = 0;
	Device device = Device();
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
	VkSampler textureSampler;
	SwapChain swapChain = SwapChain();
	VkCommandPool commandPool;
	
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	

	void initWindow();
	void initVulkan();
	void mainLoop();
	void cleanup();
	void createInstance();
	void setupDebugMessenger();
	void createSurface();
	void drawFrame();
	void createSyncObjects();
	void createCommandBuffer();
	void recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex);
	void createCommandPool();

#ifdef DISPLAY_IMGUI
	VkDescriptorPool imguiPool;
	VkCommandPool imguiCommandPool;
	std::vector<VkCommandBuffer> imguiCommandBuffers;
	void initImGui();

#endif

	void showfps() {
		float current_time = glfwGetTime();
		float fps = 1.0f / (current_time - prev_time);
		prev_time = current_time;
		std::cout << "FPS: " << fps << std::endl;
	}
};