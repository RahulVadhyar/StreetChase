//"C:\\Users\\vadhy\\StreetChase\\LevelFileSystem\\test.txt"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// Structure to hold the groundblock coordinates
struct GroundBlock {
    int x;
    int y;
};

// Structure to hold the level information
struct Level {
    std::string name;
    std::vector<GroundBlock> groundBlocks;
};

// Function to parse the file and extract the levels
std::vector<Level> parseFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Level> levels;
    std::string line;

    while (std::getline(file, line)) {
        if (line.find("Level") != std::string::npos) {
            Level level;
            std::istringstream iss(line);
            std::string levelName;
            iss >> levelName >> level.name;
            level.name.pop_back();
            levels.push_back(level);
        }
        else if (line.find("groundblock") != std::string::npos) {
            GroundBlock block;
            std::istringstream iss(line);
            std::string dummy;
            char comma;
            iss >> dummy >> dummy >> block.x >> comma >> block.y >> dummy;
            levels.back().groundBlocks.push_back(block);
        }
    }

    return levels;
}

// Function to display the parsed levels
void displayLevels(const std::vector<Level>& levels) {
    for (const auto& level : levels) {
        std::cout << "Level " << level.name << ":\n";
        for (const auto& block : level.groundBlocks) {
            std::cout << "  groundblock (" << block.x << ", " << block.y << ")\n";
        }
        std::cout << '\n';
    }
}

int main() {
    std::string filename = "C:\\Users\\vadhy\\StreetChase\\LevelFileSystem\\test.txt"; // Replace with the actual filename
    std::vector<Level> levels = parseFile(filename);
    displayLevels(levels);
    return 0;
}