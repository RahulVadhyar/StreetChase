struct PostProcessing {
	float brightness = 1.0f;
	float saturation = 1.0f;
	float exposure = 1.0f;
	float brilliance = 1.0f;
	float highlights = 1.0f;
	float shadows = 1.0f;
	float contrast = 1.0f;
	float blackPoint = 0.0f;
	float vibrancy = 1.0f;
	float warmth = 1.0f;
	float tint = 1.0f;
	float sharpness = 1.0f;
	float vignette = 1.0f;
};

struct Settings {
	PostProcessing postProcessing;
	int msaaSamples = 1;
	int maxMsaaSamples = 1;
	bool vsync = false;
};


struct GUIParams {
	std::vector<ShapeParams*> shapeParams;
	float* bgColor;
	bool addRectangle;
	bool addTriangle;
	std::string texturePath;
	Settings* settings{};
	bool changeMSAA = false;
};

struct GUIInternal {
	bool showSettingsWindow = false;
	bool showShapeWindow = false;	
	bool showAddShapeWindow = false;
	bool showPostProcessingWindow = false;
	int selectedShape = 0;
};

void gui(GUIParams* gui);
void shapeMover(ShapeParams* shapeParams);