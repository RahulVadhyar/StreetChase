#include "vulkaninit.hpp"
#include "shape.hpp"
#include "gui.hpp"

std::vector<bool> shapeHeader;

void gui(GUIParams* gui) {
	char texturePath[30] = "texture.jpg";
	shapeHeader.resize(gui->shapeParams.size());
	ImGui::Begin("GUI");
	ImGui::ColorEdit3("Background Color", gui->bgColor);
	ImGui::InputText("Texture Path", texturePath, sizeof(texturePath));
	gui->addRectangle = ImGui::Button("Add Rectangle");
	gui->addTriangle = ImGui::Button("Add Triangle");
	if (gui->addRectangle || gui->addTriangle) {
		const char* intermediate = texturePath;
		gui->texturePath = intermediate;
	}

	for (size_t i = 0; i < gui->shapeParams.size(); i++) {
		shapeHeader[i] = ImGui::CollapsingHeader(("Shape Params " + std::to_string(i + 1)).c_str(), shapeHeader[i]);
		if(shapeHeader[i])
			shapeMover(gui->shapeParams[i]);
	}
	ImGui::End();
}

void shapeMover(ShapeParams* shapeParams) {
	ImGui::DragFloat("X", &shapeParams->x, 0.001f);
	ImGui::DragFloat("Y", &shapeParams->y, 0.001f);
	ImGui::DragFloat("Size X", &shapeParams->xSize, 0.001f);
	ImGui::DragFloat("Size Y", &shapeParams->ySize, 0.001f);
	ImGui::DragFloat("Rotation", &shapeParams->rotation, 1.0f);
};