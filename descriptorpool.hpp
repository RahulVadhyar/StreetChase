#pragma once
class DescriptorPool {
public:
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	Device device;
	void createDescriptorSetLayout(Device device);
	void createDescriptorPool();
	void createDescriptorSets(Rectangle rectangle, VkSampler textureSampler);//
	void destroy();
};