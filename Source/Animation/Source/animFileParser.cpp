#include "animFileParser.hpp"
/*
We need to load in animation files. It will have the extension ".anim.
"
*/
// Function to parse the shape attributes from a line
AnimationShapeAttributes parseShapeAttributes(const std::string& line) {
    AnimationShapeAttributes attributes;
    std::istringstream ss(line);
    char dummy; // to consume the leading '#'
    ss >> dummy >> attributes.x >> dummy >> attributes.y >> dummy >> attributes.rotation
       >> dummy >> attributes.xSize >> dummy >> attributes.ySize >> dummy >> std::boolalpha >> attributes.show;
    return attributes;
}

// Function to parse the animation data from the file
AnimationData parseAnimationData(std::ifstream& file) {
    AnimationData animationData;
    std::string line;

    // Read the shape name
    std::getline(file, line);
    animationData.name = line.substr(0, line.find('{'));

    while (std::getline(file, line) && line.find('}') == std::string::npos) {
        // Check if the line contains the "Child" field
        if (line.find("Child:") != std::string::npos) {
            // Read child shapes
            while (std::getline(file, line) && line.find(']') == std::string::npos) {
                AnimationData childData = parseAnimationData(file);
                animationData.children.push_back(childData);
            }
        } else if (line.find('#') != std::string::npos) {
            // Read shape attributes
            AnimationShapeAttributes attributes = parseShapeAttributes(line);
            animationData.shapeAttributes.push_back(attributes);
        }
    }

    return animationData;
}

AnimationData loadAnimationData(std::string path){
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open animation file: " + path);
    }

    AnimationData data = parseAnimationData(file);
    file.close();
    return data;
}