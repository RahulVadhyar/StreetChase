#include "vulkaninit.hpp"
#include "device.hpp"
#include "swapchain.hpp"
#include "chrono.hpp"
#include "validation.hpp"
#include "helper.hpp"
#include "Vertex.hpp"


static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	auto app = reinterpret_cast<ChronoApplication*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void ChronoApplication::initWindow() {
	//initialize glfw with a resizeable window
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, "Chrono", nullptr, nullptr);
	if (window == nullptr) {
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void ChronoApplication::initVulkan() {
	createInstance();
	if (enableValidationLayers) {
		setupDebugMessenger();
	}
	createSurface();
	device.init(instance, surface);
	swapChain.init(device, surface, window);
	createTextureSampler(device, &textureSampler);
	createCommandPool();
	shapes.push_back(Rectangle());
	shapes.push_back(Triangle());
	for(Shape& shape : shapes) {
		shape.init(device, commandPool, &swapChain, textureSampler, "texture.jpg");
	}
	shapes[0].params.x = 0.5;
	shapes[0].params.y = 0.5;
	shapes[0].params.xSize = 0.5;
	shapes[0].params.ySize = 0.5;
	shapes[0].params.rotation = 0;

	shapes[1].params.x = -0.5;
	shapes[1].params.y = -0.5;
	shapes[1].params.xSize = 0.5;
	shapes[1].params.ySize = 0.5;
	shapes[1].params.rotation = 0;

	createCommandBuffer();
	createSyncObjects();
#ifdef DISPLAY_IMGUI
	initImGui();
	guiParams.bgColor = bgColor;
	guiParams.shapeParams.push_back(&shapes[0].params);
	guiParams.shapeParams.push_back(&shapes[1].params);
#endif
}

void ChronoApplication::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
#ifndef NDEBUG
		//showfps();
#endif
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
		glfwPollEvents();
#ifdef DISPLAY_IMGUI
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		gui(&guiParams);
		if (guiParams.addRectangle) {
			shapes.push_back(Rectangle());
			shapes.back().init(device, commandPool, &swapChain, textureSampler, guiParams.texturePath);
			guiParams.shapeParams.push_back(&shapes.back().params);
			guiParams.addRectangle = false;
		}
		if (guiParams.addTriangle) {
			shapes.push_back(Triangle());
			shapes.back().init(device, commandPool, &swapChain, textureSampler, guiParams.texturePath);
			shapes.back().params.x = -0.5;
			shapes.back().params.y = -0.5;
			shapes.back().params.xSize = 0.5;
			shapes.back().params.ySize = 0.5;
			shapes.back().params.rotation = 0;
			guiParams.shapeParams.push_back(&shapes.back().params);
			guiParams.addTriangle = false;
		}
		ImGui::Render();
#endif
		drawFrame();
	}
	vkDeviceWaitIdle(device.device);
}

void ChronoApplication::cleanup() {

	swapChain.cleanup();
	vkDestroySampler(device.device, textureSampler, nullptr);
	for(Shape& shape : shapes) {
		shape.destroy();
	}
	vkDestroyCommandPool(device.device, commandPool, nullptr);
#ifdef DISPLAY_IMGUI
	ImGui_ImplVulkan_Shutdown();
	vkDestroyCommandPool(device.device, imguiCommandPool, nullptr);
#endif
	//destroy the semaphores
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device.device, renderFinishedSemaphores[i], nullptr);
		vkDestroySemaphore(device.device, imageAvailableSemaphores[i], nullptr);
		vkDestroyFence(device.device, inFlightFences[i], nullptr);
	}
	swapChain.destroy();
#ifdef DISPLAY_IMGUI
	vkDestroyDescriptorPool(device.device, imguiPool, nullptr);
#endif
	
	
	//destroy the debug messenger
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	//destroy the logical device
	device.destroy();

	//destroy the surface
	vkDestroySurfaceKHR(instance, surface, nullptr);

	//destroy the vulkan instance
	vkDestroyInstance(instance, nullptr);

	//destroy window and terminate glfw
	glfwDestroyWindow(window);
	glfwTerminate();
}

void ChronoApplication::drawFrame(){
	vkWaitForFences(device.device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device.device, swapChain.swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		swapChain.recreate();
		return;
	}
	else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
		throw std::runtime_error("Failed to acquire swap chain image");
	}
	for(Shape shape: shapes) {
		shape.update(currentFrame);
	}

	vkResetFences(device.device, 1, &inFlightFences[currentFrame]);

	vkResetCommandBuffer(commandBuffers[currentFrame], 0);
#ifdef DISPLAY_IMGUI
	vkResetCommandBuffer(imguiCommandBuffers[currentFrame], 0);
#endif
	recordCommandBuffer(currentFrame, imageIndex);
	
	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };

	std::vector<VkCommandBuffer> submitCommandBuffers = { commandBuffers[currentFrame] };
#ifdef DISPLAY_IMGUI
	submitCommandBuffers.push_back(imguiCommandBuffers[currentFrame]);
#endif
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = submitCommandBuffers.size();
	submitInfo.pCommandBuffers = submitCommandBuffers.data();
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(device.graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { swapChain.swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;
	result = vkQueuePresentKHR(device.presentQueue, &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		swapChain.recreate();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swap chain image");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void ChronoApplication::createInstance() {
	//create a struct to hold information about the application to send to vulkan
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Chrono";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Chrono Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	//create a struct to hold information about the instance to send to vulkan
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	//get the extensions required by glfw
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	//set the extensions to be used by vulkan
	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	//check for validation layer support
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("Validation layers requested, but not available");
	}

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

	//set the validation layers to be used by vulkan
	createInfo.enabledLayerCount = 0;
	createInfo.pNext = nullptr;

	//set the debug messenger
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t> (validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		/*populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;*/
	}

	//create the instance
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create instance");
	}
}

void ChronoApplication::setupDebugMessenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	//create the debug messenger
	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("Failed to set up debug messenger");
	}
}

void ChronoApplication::createSurface() {
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create window surface");
	}
}

void ChronoApplication::createSyncObjects() {
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device.device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device.device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device.device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

void ChronoApplication::createCommandBuffer() {
	commandBuffers.resize(swapChain.swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
	if (vkAllocateCommandBuffers(device.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void ChronoApplication::recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain.renderPass;
	renderPassInfo.framebuffer = swapChain.swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain.swapChainExtent;
	VkClearValue clearColor = { bgColor[0], bgColor[1], bgColor[2], 1.0f};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float> (swapChain.swapChainExtent.width);
	viewport.height = static_cast<float> (swapChain.swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChain.swapChainExtent;

	vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	for(Shape& shape: shapes) {
		vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, shape.graphicsPipeline);
		vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);
		VkBuffer vertexBuffers[] = { shape.vertexBuffer.buffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffers[currentFrame], shape.indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, shape.pipelineLayout, 0, 1, &shape.descriptorSets[currentFrame], 0, nullptr);
		vkCmdDrawIndexed(commandBuffers[currentFrame], static_cast<uint32_t>(shape.indices.size()), 1, 0, 0, 0);
	}
	vkCmdEndRenderPass(commandBuffers[currentFrame]);

	if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

#ifdef DISPLAY_IMGUI
	vkBeginCommandBuffer(imguiCommandBuffers[currentFrame], &beginInfo);
	VkRenderPassBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	info.renderPass = swapChain.imguiRenderPass;
	info.framebuffer = swapChain.imguiFramebuffers[imageIndex];
	info.renderArea.extent = swapChain.swapChainExtent;
	info.clearValueCount = 1;
	info.pClearValues = &clearColor;
	vkCmdBeginRenderPass(imguiCommandBuffers[currentFrame], &info, VK_SUBPASS_CONTENTS_INLINE);
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), imguiCommandBuffers[currentFrame]);
	vkCmdEndRenderPass(imguiCommandBuffers[currentFrame]);
	vkEndCommandBuffer(imguiCommandBuffers[currentFrame]);


#endif

}

void ChronoApplication::createCommandPool() {
	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(device.physicalDevice, surface);
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT; // Optional
	if (vkCreateCommandPool(device.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create command pool!");
	}
}

#ifdef DISPLAY_IMGUI
void ChronoApplication::initImGui() {
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

	VkDescriptorPoolSize pool_sizes[] ={{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 }};
	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1;
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;
	vkCreateDescriptorPool(device.device, &pool_info, nullptr, &imguiPool);

	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = instance;
	init_info.PhysicalDevice = device.physicalDevice;
	init_info.Device = device.device;
	init_info.QueueFamily = findQueueFamilies(device.physicalDevice, surface).graphicsFamily.value(); //IF LITERALLY ANYTHING HAPPENS WE NEED TO COME BACK TO THIS
	init_info.Queue = device.graphicsQueue;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = imguiPool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = MAX_FRAMES_IN_FLIGHT;
	init_info.CheckVkResultFn = nullptr; //add a fucntion to this
	init_info.MSAASamples = device.msaaSamples;
	ImGui_ImplVulkan_Init(&init_info, swapChain.imguiRenderPass);

	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = findQueueFamilies(device.physicalDevice, surface).graphicsFamily.value();
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool(device.device, &commandPoolCreateInfo, nullptr, &imguiCommandPool) != VK_SUCCESS) {
		throw std::runtime_error("Could not create graphics command pool");
	}

	VkCommandBuffer command_buffer = beginSingleTimeCommands(imguiCommandPool, device.device);
	ImGui_ImplVulkan_CreateFontsTexture(command_buffer);
	endSingleTimeCommands(&command_buffer, device, imguiCommandPool);

	
	imguiCommandBuffers.resize(swapChain.swapChainFramebuffers.size());
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(imguiCommandBuffers.size());
	if (vkAllocateCommandBuffers(device.device, &allocInfo, imguiCommandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}

}


#endif