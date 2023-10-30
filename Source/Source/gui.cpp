#include "vulkaninit.hpp"
#include "shape.hpp"
#include "gui.hpp"
#ifdef DISPLAY_IMGUI
std::vector<bool> shapeHeader;

GUIInternal internal{};
void gui(GUIParams* gui) {
	char texturePath[100] = "G:/Chronos/Assets/texture.jpg";
	shapeHeader.resize(gui->shapeParams.size());

	if (ImGui::BeginMainMenuBar()) {
		ImGui::MenuItem("Add Shape", nullptr, &internal.showAddShapeWindow);
		ImGui::MenuItem("Edit Shape", nullptr, &internal.showShapeWindow);
		ImGui::MenuItem("Post Processing", nullptr, &internal.showPostProcessingWindow);
		ImGui::MenuItem("Settings", nullptr, &internal.showSettingsWindow);
		ImGui::EndMainMenuBar();
	}
	if(internal.showShapeWindow) {
		ImGui::Begin("Shapes");
		{	internal.showAddShapeWindow = ImGui::Button("Add Shape");
			for (size_t i = 0; i < gui->shapeParams.size(); i++) {
				shapeHeader[i] = ImGui::CollapsingHeader(("Shape Params " + std::to_string(i + 1)).c_str(), shapeHeader[i]);
				if (shapeHeader[i])
					shapeMover(gui->shapeParams[i]);
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
						gui->addRectangle = true;
						break;
					case 2:
						gui->addTriangle = true;
						break;	
				}
				const char* intermediate = texturePath;
				gui->texturePath = intermediate;
			}
		}
		ImGui::End();
	}


	if (internal.showSettingsWindow) { 
		ImGui::Begin("Settings");
		{	ImGui::Checkbox("Vsync", &gui->settings->vsync);
			ImGui::ColorEdit3("Background Color", gui->bgColor);
			if (ImGui::BeginCombo("MSAA", ("MSAA " + std::to_string(gui->settings->msaaSamples)).c_str(), 0)) {
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
							gui->settings->msaaSamples = 1;
							break;
						case 2:
							gui->settings->msaaSamples = 2;
							break;
						case 3:
							gui->settings->msaaSamples = 4;
							break;
						case 4:
							gui->settings->msaaSamples = 8;
							break;
						case 5:
							gui->settings->msaaSamples = 16;
							break;
						}
						
			}
			
		}
		ImGui::End();
	}
	if (internal.showPostProcessingWindow) {
		ImGui::Begin("Post Processing");
		{
			ImGui::SliderFloat("Brightness", &gui->settings->postProcessing.brightness, 0.0f, 1.0f);
			ImGui::SliderFloat("Saturation", &gui->settings->postProcessing.saturation, 0.0f, 1.0f);
			ImGui::SliderFloat("Exposure", &gui->settings->postProcessing.exposure, 0.0f, 1.0f);
			ImGui::SliderFloat("Brilliance", &gui->settings->postProcessing.brilliance, 0.0f, 1.0f);
			ImGui::SliderFloat("Highlights", &gui->settings->postProcessing.highlights, 0.0f, 1.0f);
			ImGui::SliderFloat("Shadows", &gui->settings->postProcessing.shadows, 0.0f, 1.0f);
			ImGui::SliderFloat("Contrast", &gui->settings->postProcessing.contrast, 0.0f, 1.0f);
			ImGui::SliderFloat("Black Point", &gui->settings->postProcessing.blackPoint, 0.0f, 1.0f);
			ImGui::SliderFloat("Vibrancy", &gui->settings->postProcessing.vibrancy, 0.0f, 1.0f);
			ImGui::SliderFloat("Warmth", &gui->settings->postProcessing.warmth, 0.0f, 1.0f);
			ImGui::SliderFloat("Tint", &gui->settings->postProcessing.tint, 0.0f, 1.0f);
			ImGui::SliderFloat("Sharpness", &gui->settings->postProcessing.sharpness, 0.0f, 1.0f);
			ImGui::SliderFloat("Vignette", &gui->settings->postProcessing.vignette, 0.0f, 1.0f);

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
#endif