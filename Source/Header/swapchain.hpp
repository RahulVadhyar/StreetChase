#pragma once
#include "vulkaninit.hpp"
#include "device.hpp"
struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector <VkSurfaceFormatKHR> formats;
	std::vector <VkPresentModeKHR> presentModes;
};

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

class SwapChain {
public:
	Device* device;
	
	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkRenderPass renderPass;
	VkSurfaceKHR surface;
	GLFWwindow* window;
	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	void init(Device* device, VkSurfaceKHR surface, GLFWwindow* window);
	void create();
	void recreate();
	void cleanup();
	void destroy();
	void createImageViews();
	void createRenderPass();
	void createFramebuffers();
	void createColorResources();
	void changeMsaa();

#ifdef DISPLAY_IMGUI
	std::vector<VkFramebuffer> imguiFramebuffers;
	VkRenderPass imguiRenderPass;
#endif

};