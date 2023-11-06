#pragma once
#include "device.hpp"
#include "swapChain.hpp"
#include "stb_font_arial_24_usascii.inl"
class Text{
public:
	uint32_t maxTextLength = 2048;
	stb_fontchar stbFontData[STB_FONT_arial_24_usascii_NUM_CHARS];
	uint32_t numLetters;
	enum TextAlignment {
		Left,
		Center,
		Right
	};
	float scale = 100.0f;

	const char* vertexShaderPath = "G:/Chronos/Shaders/textVert.spv";
	const char* fragmentShaderPath = "G:/Chronos/Shaders/textFrag.spv";
	Device* device;
	SwapChain* swapChain;
	VkCommandPool commandPool;
	VkSampler textureSampler;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkImage texture;
	VkImageView textureImageView;
	VkDeviceMemory textureImageMemory;
	glm::vec4 * mappedMemory;

	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	void init(Device* device, VkCommandPool commandPool, SwapChain* swapChain);
	void createGraphicsPipeline();
	void createDescriptorPool();
	void createDescriptorSetLayout();
	void createDescriptorSets();
	void destroy();
	void recreateGraphicsPipeline();
	void beginUpdate();
	void add(std::string text, float x, float y, TextAlignment alignment);
	void endUpdate();
};