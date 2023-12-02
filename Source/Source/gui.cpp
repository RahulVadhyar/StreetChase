#include "vulkaninit.hpp"
#include "shape.hpp"
#include "gui.hpp"
#include "Chrono.hpp"
#include "helper.hpp"
#ifdef DISPLAY_IMGUI

void GUI::config() {
	char texturePath[100] = "G:/Chronos/Assets/texture.jpg";
	shapeHeader.resize(params->shapeManager->shapes.size());

	if (ImGui::BeginMainMenuBar()) {
		ImGui::MenuItem("Add Shape", nullptr, &internal.showAddShapeWindow);
		ImGui::MenuItem("Edit Shape", nullptr, &internal.showShapeWindow);
		ImGui::MenuItem("Post Processing", nullptr, &internal.showPostProcessingWindow);
		ImGui::MenuItem("Settings", nullptr, &internal.showSettingsWindow);
		ImGui::EndMainMenuBar();
	}
	if(internal.showShapeWindow) {
		ImGui::Begin("Shapes");
		{	for (size_t i = 0; i < params->shapeManager->shapes.size(); i++) {
				shapeHeader[i] = ImGui::CollapsingHeader(("Shape Params " + std::to_string(i + 1)).c_str(), shapeHeader[i]);
				if (shapeHeader[i])
					shapeMover(&params->shapeManager->shapes[i].params);
			}
		}
		ImGui::End();
	}
	if (internal.showAddShapeWindow) {
		ImGui::Begin("Add Shapes");
		{
			ImGui::InputText("Texture Path", texturePath, sizeof(texturePath));
			std::string current;
			switch (internal.selectedShape) {
				case 0:
					current = "None";
					break;
				case 1:
					current = "Rectangle";
					break;
				case 2:
					current = "Triangle";
					break;
			}
			bool selected[3];
			for (int i = 0; i < 3; i++) {
				if (i == internal.selectedShape)
					selected[i] = true;
				else
					selected[i] = false;
			}
			if(ImGui::BeginCombo("Shape", current.c_str())) {
				
				ImGui::Selectable("None", &selected[0], 0);
				ImGui::Selectable("Rectangle", &selected[1], 0);
				ImGui::Selectable("Triangle", &selected[2], 0);
				ImGui::EndCombo();
				for (int i = 0; i < 3; i++)
					if (selected[i])
						internal.selectedShape = i;
			}		
			if (ImGui::Button("Add Shape")) {
				internal.showAddShapeWindow = false;
				switch (internal.selectedShape) {
					case 0:
						break;
					case 1:
						params->addRectangle = true;
						break;
					case 2:
						params->addTriangle = true;
						break;	
				}
				const char* intermediate = texturePath;
				params->texturePath = intermediate;
			}
		}
		ImGui::End();
	}


	if (internal.showSettingsWindow) { 
		ImGui::Begin("Settings");
		{	ImGui::Checkbox("Vsync", &params->settings->vsync);
			ImGui::ColorEdit3("Background Color", params->bgColor);
			if (ImGui::BeginCombo("MSAA", ("MSAA " + std::to_string(params->settings->msaaSamples)).c_str(), 0)) {
				bool selected[5] = { false, false, false, false, false };
				ImGui::Selectable("MSAA 1", &selected[1],0);
				ImGui::Selectable("MSAA 2", &selected[2],0);
				ImGui::Selectable("MSAA 4", &selected[3],0);
				ImGui::Selectable("MSAA 8", &selected[4],0);
				ImGui::Selectable("MSAA 16", &selected[5], 0);
				ImGui::EndCombo();
				for(int i = 0; i < 5; i++)
					if (selected[i])
						switch (i)
						{
						case 1:
							params->settings->msaaSamples = 1;
							break;
						case 2:
							params->settings->msaaSamples = 2;
							break;
						case 3:
							params->settings->msaaSamples = 4;
							break;
						case 4:
							params->settings->msaaSamples = 8;
							break;
						case 5:
							params->settings->msaaSamples = 16;
							break;
						}
						
			}
			
		}
		ImGui::End();
	}
	if (internal.showPostProcessingWindow) {
		ImGui::Begin("Post Processing");
		{
			ImGui::SliderFloat("Brightness", &params->settings->postProcessing.brightness, 0.0f, 1.0f);
			ImGui::SliderFloat("Saturation", &params->settings->postProcessing.saturation, 0.0f, 1.0f);
			ImGui::SliderFloat("Exposure", &params->settings->postProcessing.exposure, 0.0f, 1.0f);
			ImGui::SliderFloat("Brilliance", &params->settings->postProcessing.brilliance, 0.0f, 1.0f);
			ImGui::SliderFloat("Highlights", &params->settings->postProcessing.highlights, 0.0f, 1.0f);
			ImGui::SliderFloat("Shadows", &params->settings->postProcessing.shadows, 0.0f, 1.0f);
			ImGui::SliderFloat("Contrast", &params->settings->postProcessing.contrast, 0.0f, 1.0f);
			ImGui::SliderFloat("Black Point", &params->settings->postProcessing.blackPoint, 0.0f, 1.0f);
			ImGui::SliderFloat("Vibrancy", &params->settings->postProcessing.vibrancy, 0.0f, 1.0f);
			ImGui::SliderFloat("Warmth", &params->settings->postProcessing.warmth, 0.0f, 1.0f);
			ImGui::SliderFloat("Tint", &params->settings->postProcessing.tint, 0.0f, 1.0f);
			ImGui::SliderFloat("Sharpness", &params->settings->postProcessing.sharpness, 0.0f, 1.0f);
			ImGui::SliderFloat("Vignette", &params->settings->postProcessing.vignette, 0.0f, 1.0f);

		}
		ImGui::End();
	}
}

void shapeMover(ShapeParams* shapeParams) {
	ImGui::DragFloat("X", &shapeParams->x, 0.001f);
	ImGui::DragFloat("Y", &shapeParams->y, 0.001f);
	ImGui::DragFloat("Size X", &shapeParams->xSize, 0.001f);
	ImGui::DragFloat("Size Y", &shapeParams->ySize, 0.001f);
	ImGui::DragFloat("Rotation", &shapeParams->rotation, 1.0f);
};


void GUI::init(Device * device, GLFWwindow* window, SwapChain* swapChain, VkInstance instance, VkSurfaceKHR surface, GUIParams* params) {
	this->device = device;
	this->swapChain = swapChain;
	this->params = params;
	this->surface = surface;
	this->window = window;

	renderPass = createRenderPass(*device, *swapChain, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, false, false, true);
	framebuffers = createFramebuffer(*device, *swapChain, renderPass, false);
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	std::array<VkDescriptorPoolSize, 2> poolSizes{};
	poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolSize pool_sizes[] = { { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 } };
	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1;
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;
	vkCreateDescriptorPool(device->device, &pool_info, nullptr, &descriptorPool);

	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance;
	init_info.PhysicalDevice = device->physicalDevice;
	init_info.Device = device->device;
	init_info.QueueFamily = findQueueFamilies(device->physicalDevice, surface).graphicsFamily.value();
	init_info.Queue = device->graphicsQueue;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = descriptorPool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
	init_info.CheckVkResultFn = nullptr; //add a fucntion to this
	ImGui_ImplVulkan_Init(&init_info, renderPass);

	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = findQueueFamilies(device->physicalDevice, surface).graphicsFamily.value();
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool(device->device, &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Could not create graphics command pool");
	}

	VkCommandBuffer command_buffer = beginSingleTimeCommands(commandPool, device->device);
	ImGui_ImplVulkan_CreateFontsTexture();
	endSingleTimeCommands(&command_buffer, *device, commandPool);


	commandBuffers.resize(swapChain->swapChainImageViews.size());
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	if (vkAllocateCommandBuffers(device->device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

	std::cout << "Command Buffers(GUI): " << std::endl;
	for(auto commandBuffer : commandBuffers)
		std::cout << commandBuffer << std::endl;
}

void GUI::destroy() {
	ImGui_ImplVulkan_Shutdown();
	vkDestroyCommandPool(device->device, commandPool, nullptr);
	vkDestroyDescriptorPool(device->device, descriptorPool, nullptr);
}

void GUI::update() {
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	config();
	if (params->changeMSAA) {
		if (params->settings->msaaSamples <= params->settings->maxMsaaSamples) {
			switch (params->settings->msaaSamples) {
			case 1:
				device->msaaSamples = VK_SAMPLE_COUNT_1_BIT;
				break;
			case 2:
				device->msaaSamples = VK_SAMPLE_COUNT_2_BIT;
				break;
			case 4:
				device->msaaSamples = VK_SAMPLE_COUNT_4_BIT;
				break;
			case 8:
				device->msaaSamples = VK_SAMPLE_COUNT_8_BIT;
				break;
			case 16:
				device->msaaSamples = VK_SAMPLE_COUNT_16_BIT;
				break;
			}
			swapChain->cleanup();
			swapChain->init(device, surface, window);
		}
		else {
			params->settings->msaaSamples = device->msaaSamples;
		}
		params->changeMSAA = false;
	}
	if (params->addRectangle) {
		ShapeParams shapeParams;
		shapeParams.x = -0.5;
		shapeParams.y = -0.5;
		shapeParams.xSize = 0.5;
		shapeParams.ySize = 0.5;
		shapeParams.rotation = 0;
		params->shapeManager->addRectangle(shapeParams, params->texturePath);	
		params->addRectangle = false;
	}
	if (params->addTriangle) {
		ShapeParams shapeParams;
		shapeParams.x = -0.5;
		shapeParams.y = -0.5;
		shapeParams.xSize = 0.5;
		shapeParams.ySize = 0.5;
		shapeParams.rotation = 0;
		params->shapeManager->addTriangle(shapeParams, params->texturePath);
		params->addTriangle = false;
	}


	ImGui::Render();
}

void GUI::render(uint32_t currentFrame, uint32_t imageIndex, float bgColor[3]) {
	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	VkClearValue clearColor = { bgColor[0], bgColor[1], bgColor[2], 1.0f };
	vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo);
	VkRenderPassBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.renderPass = renderPass;
	info.framebuffer = framebuffers[imageIndex];
	info.renderArea.extent = swapChain->swapChainExtent;
	info.clearValueCount = 1;
	info.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffers[currentFrame], &info, VK_SUBPASS_CONTENTS_INLINE);
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffers[currentFrame]);
	vkCmdEndRenderPass(commandBuffers[currentFrame]);
	vkEndCommandBuffer(commandBuffers[currentFrame]);
}

#endif