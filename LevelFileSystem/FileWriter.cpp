#include <iostream>
#include <fstream>
#include <vector>

struct GroundBlock {
    int x;
    int y;
};

struct Level {
    std::string name;
    std::vector<GroundBlock> groundBlocks;
};

void writeLevelToFile(const Level& level, std::ofstream& file) {
    file << "Level " << level.name << "{\n";
    for (const GroundBlock& block : level.groundBlocks) {
        file << "    groundblock (" << block.x << ", " << block.y << ");\n";
    }
    file << "};\n\n";
}

int main() {
    Level level1;
    level1.name = "LevelNae1";
    level1.groundBlocks = {
        {1, 4},
        {3, 5},
        {6, 7},
        {2, 3},
        {4, 1},
    };

    Level level2;
    level2.name = "LeelName2";
    level2.groundBlocks = {
        {2, 3},
        {4, 1}
    };

    std::ofstream file("C:\\Users\\vadhy\\StreetChase\\LevelFileSystem\\test.txt");
    if (file.is_open()) {
        writeLevelToFile(level1, file);
        writeLevelToFile(level2, file);
        file.close();
        std::cout << "File created successfully.\n";
    } else {
        std::cout << "Unable to create file.\n";
    }

    return 0;
}
