#pragma once
static std::vector<char> readFile(const std::string& filename);
VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
void createGraphicsPipeline(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout, SwapChain swapChain, VkPipelineLayout* pipelineLayout, VkPipeline* graphicsPipeline);