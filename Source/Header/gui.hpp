struct GUIParams {
	std::vector<ShapeParams*> shapeParams;
	float* bgColor;
	bool addRectangle;
	bool addTriangle;
	std::string texturePath;
};

void gui(GUIParams* gui);
void shapeMover(ShapeParams* shapeParams);
//void addShapes();
//void changeTexture();