#include <fstream>
#include <stdexcept>
#include <vector>
#ifdef INCLUDE_CHRONO
#include "Chrono.hpp"
#endif

static std::vector<char> readFile(const std::string& filename);
VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
void createGraphicsPipeline(VkDevice device, VkDescriptorSetLayout* descriptorSetLayout, VkRenderPass renderPass, VkExtent2D swapChainExtent, VkPipelineLayout* pipelineLayout, VkPipeline* graphicsPipeline);