#pragma once
class DescriptorPool {
public:
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	Device device;
	void createDescriptorSetLayout(Device device);
	void createDescriptorPool();
	void createDescriptorSets(std::vector<UniformBuffer> uniformBuffers, VkImageView textureImageView, VkSampler textureSampler);//
	void destroy();
};