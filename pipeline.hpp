#pragma once
static std::vector<char> readFile(const std::string& filename);
VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
void createGraphicsPipeline(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout, VkRenderPass renderPass, VkExtent2D swapChainExtent, VkPipelineLayout* pipelineLayout, VkPipeline* graphicsPipeline);