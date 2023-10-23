#include "vulkaninit.hpp"
#include "commandpool.hpp"
#include "shape.hpp"

void Rectangle::init(Device device, CommandPool commandPool) {
	this-> device = device;
	this-> commandPool = commandPool;

	vertexBuffer.size = sizeof(vertices[0]) * vertices.size();
	vertexBuffer.create(device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vertexBuffer.copy(vertices.data(), commandPool.commandPool);

	indexBuffer.size = sizeof(indices[0]) * indices.size();
	indexBuffer.create(device, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	indexBuffer.copy(indices.data(), commandPool.commandPool);
}

void Rectangle::destroy() {
	vertexBuffer.destroy();
	indexBuffer.destroy();
}