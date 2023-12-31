#pragma once
struct ShapeParams {
  float x = 0;
  float y = 0;
  float rotation = 0;
  float xSize = 1;
  float ySize = 1;
  bool show = true;
};

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};