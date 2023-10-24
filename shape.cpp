#include "vulkaninit.hpp"
#include "device.hpp"
#include "commandpool.hpp"
#include "shape.hpp"
#include "chrono.hpp"
void Rectangle::init(Device device, CommandPool commandPool) {
	this-> device = device;
	this-> commandPool = commandPool;

	vertexBuffer.size = sizeof(vertices[0]) * vertices.size();
	vertexBuffer.create(device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vertexBuffer.copy(vertices.data(), commandPool.commandPool);

	indexBuffer.size = sizeof(indices[0]) * indices.size();
	indexBuffer.create(device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	indexBuffer.copy(indices.data(), commandPool.commandPool);

	texture.create(device, commandPool.commandPool);
	uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		uniformBuffers[i].create(device);
	}
}

void Rectangle::destroy() {
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		uniformBuffers[i].destroy();
	}
	texture.destroy();
	vertexBuffer.destroy();
	indexBuffer.destroy();
}