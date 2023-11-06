#include "vulkaninit.hpp"
#include "device.hpp"
#include "text.hpp"
#include "Chrono.hpp"
#include "helper.hpp"

void Text::init(Device* device, VkCommandPool commandPool, SwapChain* swapChain) {
	this->device = device;
	this->swapChain = swapChain;
	this->commandPool = commandPool;
	
	//initalize the font
	const uint32_t fontWidth = STB_FONT_arial_24_usascii_BITMAP_WIDTH;
	const uint32_t fontHeight = STB_FONT_arial_24_usascii_BITMAP_HEIGHT;
	static unsigned char fontpixels[fontHeight][fontWidth];
	stb_font_arial_24_usascii(stbFontData, fontpixels, fontHeight);

	//create the vertex buffer
	VkDeviceSize bufferSize = maxTextLength * sizeof(glm::vec4);
	createBuffer(*device, bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vertexBuffer, &vertexBufferMemory);

	//create the font texture
	createImage(*device, fontWidth, fontWidth, VK_FORMAT_R8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &texture, &textureImageMemory, VK_SAMPLE_COUNT_1_BIT);

	//staging
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(device->device, texture, &memRequirements);
	createBuffer(*device, memRequirements.size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBuffer, &stagingBufferMemory);

	void* data;
	vkMapMemory(device->device, stagingBufferMemory, 0, memRequirements.size, 0, &data);
	memcpy(data, &fontpixels[0][0], fontWidth * fontHeight);
	vkUnmapMemory(device->device, stagingBufferMemory);

	transitionImageLayout(texture, VK_FORMAT_R8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, commandPool, *device);
	copyBufferToImage(stagingBuffer, texture, fontWidth, fontHeight, commandPool, *device);
	transitionImageLayout(texture, VK_FORMAT_R8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, commandPool, *device);
	vkDestroyBuffer(device->device, stagingBuffer, nullptr);
	vkFreeMemory(device->device, stagingBufferMemory, nullptr);

	//create image view
	textureImageView = createImageView(*device, VK_FORMAT_R8_UNORM, texture);

	VkPhysicalDeviceProperties properties{};
	vkGetPhysicalDeviceProperties(device->physicalDevice, &properties);

	//create the sampler
	VkSamplerCreateInfo samplerInfo{};

	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
	samplerInfo.unnormalizedCoordinates = VK_FALSE;
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.maxLod = 0.0f;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.compareOp = VK_COMPARE_OP_NEVER;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 1.0f;
	samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	if (vkCreateSampler(device->device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}

	//create all of the other thigns
	createDescriptorSetLayout();
	createDescriptorPool();
	createGraphicsPipeline();
	createDescriptorSets();
}


void Text::createGraphicsPipeline() {
	//load and create the shader modules
	auto vertShaderCode = readFile(vertexShaderPath);
	auto fragShaderCode = readFile(fragmentShaderPath);

	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, device->device);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, device->device);

	//create a struct to hold information about the shader to send to vulkan
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };


	std::array<VkVertexInputBindingDescription, 2> vertexInputBindings;
	vertexInputBindings[0].binding = 0;
	vertexInputBindings[0].stride = sizeof(glm::vec4);
	vertexInputBindings[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	vertexInputBindings[1].binding = 1;
	vertexInputBindings[1].stride = sizeof(glm::vec4);
	vertexInputBindings[1].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	std::array<VkVertexInputAttributeDescription, 2> vertexInputAttributes;
	vertexInputAttributes[0].binding = 0; //position
	vertexInputAttributes[0].location = 0;
	vertexInputAttributes[0].format = VK_FORMAT_R32G32_SFLOAT;
	vertexInputAttributes[0].offset = 0;

	vertexInputAttributes[1].binding = 1; //UV
	vertexInputAttributes[1].location = 1;
	vertexInputAttributes[1].format = VK_FORMAT_R32G32_SFLOAT;
	vertexInputAttributes[1].offset = sizeof(glm::vec2);

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{}; //modify this
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
	vertexInputInfo.pVertexBindingDescriptions = vertexInputBindings.data();
	vertexInputInfo.pVertexAttributeDescriptions = vertexInputAttributes.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)(*swapChain).swapChainExtent.width;
	viewport.height = (float)(*swapChain).swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = (*swapChain).swapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.rasterizationSamples = device->msaaSamples;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE;
	multisampling.sampleShadingEnable = VK_TRUE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{}; //modfiy this
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1; // Optional
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout; // Optional
	pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

	if (vkCreatePipelineLayout(device->device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = (*swapChain).renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1;

	if (vkCreateGraphicsPipelines(device->device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	//destory the shader modules
	vkDestroyShaderModule(device->device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device->device, vertShaderModule, nullptr);
}


void Text::destroy() {
	vkDestroySampler(device->device, textureSampler, nullptr);
	vkDestroyImageView(device->device, textureImageView, nullptr);
	vkDestroyImage(device->device, texture, nullptr);
	vkFreeMemory(device->device, textureImageMemory, nullptr);
	vkDestroyBuffer(device->device, vertexBuffer, nullptr);
	vkFreeMemory(device->device, vertexBufferMemory, nullptr);
	vkDestroyDescriptorPool(device->device, descriptorPool, nullptr);
	vkDestroyDescriptorSetLayout(device->device, descriptorSetLayout, nullptr);
	vkDestroyPipeline(device->device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device->device, pipelineLayout, nullptr);
}

void Text::createDescriptorPool() {
	std::array<VkDescriptorPoolSize, 1> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	if (vkCreateDescriptorPool(device->device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor pool!");
	}
}

void Text::createDescriptorSetLayout() {
	VkDescriptorSetLayoutBinding samplerLayoutBinding{};
	samplerLayoutBinding.binding = 0;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::array<VkDescriptorSetLayoutBinding, 1> bindings = { samplerLayoutBinding };
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();
	if (vkCreateDescriptorSetLayout(device->device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void Text::createDescriptorSets() {
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();
	descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(device->device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = textureImageView;
		imageInfo.sampler = textureSampler;

		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device->device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void Text::recreateGraphicsPipeline() {
	vkDestroyPipeline(device->device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(device->device, pipelineLayout, nullptr);
	createGraphicsPipeline();
}

void Text::beginUpdate() {
	if (vkMapMemory(device->device, vertexBufferMemory, 0, VK_WHOLE_SIZE, 0, (void**) & mappedMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to map memory!");
	}
	numLetters = 0;
}

void Text::add(std::string text, float x, float y, TextAlignment alignment) {
	const uint32_t firstChar = STB_FONT_arial_24_usascii_FIRST_CHAR;

	assert(mappedMemory != nullptr);

	const float charW = 1.5f * scale / swapChain->swapChainExtent.width;
	const float charH = 1.5f * scale / swapChain->swapChainExtent.height;

	float fbW = (float)swapChain->swapChainExtent.width;
	float fbH = (float)swapChain->swapChainExtent.height;
	x = (x / fbW * 2.0f) - 1.0f;
	y = (y / fbH * 2.0f) - 1.0f;

	// Calculate text width
	float textWidth = 0;
	for (auto letter : text)
	{
		stb_fontchar* charData = &stbFontData[(uint32_t)letter - firstChar];
		textWidth += charData->advance * charW;
	}

	switch (alignment)
	{
	case Right:
		x -= textWidth;
		break;
	case Center:
		x -= textWidth / 2.0f;
		break;
	case Left:
		break;
	}

	// Generate a uv mapped quad per char in the new text
	for (auto letter : text)
	{
		stb_fontchar* charData = &stbFontData[(uint32_t)letter - firstChar];

		mappedMemory->x = (x + (float)charData->x0 * charW);
		mappedMemory->y = (y + (float)charData->y0 * charH);
		mappedMemory->z = charData->s0;
		mappedMemory->w = charData->t0;
		mappedMemory++;

		mappedMemory->x = (x + (float)charData->x1 * charW);
		mappedMemory->y = (y + (float)charData->y0 * charH);
		mappedMemory->z = charData->s1;
		mappedMemory->w = charData->t0;
		mappedMemory++;

		mappedMemory->x = (x + (float)charData->x0 * charW);
		mappedMemory->y = (y + (float)charData->y1 * charH);
		mappedMemory->z = charData->s0;
		mappedMemory->w = charData->t1;
		mappedMemory++;

		mappedMemory->x = (x + (float)charData->x1 * charW);
		mappedMemory->y = (y + (float)charData->y1 * charH);
		mappedMemory->z = charData->s1;
		mappedMemory->w = charData->t1;
		mappedMemory++;

		x += charData->advance * charW;

		numLetters++;
	}
}

void Text::endUpdate() {
	vkUnmapMemory(device->device, vertexBufferMemory);
	mappedMemory = nullptr;
}