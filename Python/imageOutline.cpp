#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "G:\Chronos\Include\stb_image.h"
using namespace std;

#define IMAGE_PATH "G:\\Chronos\\Python\\test.png"

int getIndex(int i, int j, int width, int height, int channels){
    return (j*width*channels) + (i*channels);
}

vector<int> getNeightbours(int i, int j, int width, int height, int channels){
    vector<int> neighbours = {getIndex(i - 1, j - 1, width, height, channels),
                                              getIndex(i,     j - 1, width, height, channels),
                                              getIndex(i + 1, j - 1, width, height, channels),
                                              getIndex(i - 1, j, width, height, channels),
                                              getIndex(i,     j, width, height, channels),
                                              getIndex(i + 1, j, width, height, channels),
                                              getIndex(i - 1, j + 1, width, height, channels),
                                              getIndex(i,     j + 1, width, height, channels),
                                              getIndex(i + 1, j + 1, width, height, channels),
                                              };
    return neighbours;
}

unsigned char* loadImage(int* width, int* height, int* channels){
    unsigned char *img = stbi_load(IMAGE_PATH, width, height, channels, 4);
    if(img == NULL){
        cout << "Error in loading the image" << endl;
        exit(1);
    }
    cout << "Image loaded successfully" << endl;
    cout << "Width: " << *width << " Height: " << *height << " Channels: " << *channels << endl;
    return img;
}

void generateMatte(unsigned char* image, int width, int height, int channels){
    for (int i = 0; i < width; i ++) {
        for(int j = 0; j < height; j++){
            int index = getIndex(i, j, width, height, channels);
            if(image[index+3] == 0){
                // cout << "R: " << (int)image[index] << " G: " << (int)image[index+1] << " B: " << (int)image[index+2] << " A: " << (int)image[index+3] << endl;
                image[index]   = 0;
                image[index+1] = 0;
                image[index+2] = 0;
                image[index+3] = 255;
            } else {
                image[index]   = 255;
                image[index+1] = 255;
                image[index+2] = 255;
                image[index+3] = 255;
            }
        }
    }
}

void generateOutline(unsigned char* image, int width, int height, int channels){
    for(int i =  0; i < width; i++){
        for(int j =0; j < height; j++){
            int index = getIndex(i, j, width, height, channels);
            if(image[index] == 255 && image[index+1] == 255 && image[index+2] == 255){
                if(i == 0 || j == 0 || j == height - 1 || i == width - 1){
                    image[index] = 100;
                    image[index+1] = 0;
                    image[index+2] = 0;
                } else {
                    vector<int> neighbours = getNeightbours(i, j, width, height, channels);
                    for(auto neighbour: neighbours){
                        if(image[neighbour] == 0 && image[neighbour+1] == 0 && image[neighbour+2] == 0){
                            image[index] = 100;
                            image[index+1] = 0;
                            image[index+2] = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void removeMatte(unsigned char* image, int width, int height, int channels){
    for (int i = 0; i < width * height * channels; i += channels) {
        if(image[i] == 255 && image[i+1] == 255 && image[i+2] == 255){
            image[i] = 0;
            image[i+1] = 0;
            image[i+2] = 0;
        }
    }
}

void generateVertices(unsigned char* image, int width, int height, int channels){
    vector<vector<int>> pixelCoordinates;
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            int index = getIndex(i, j, width, height, channels);
            if(image[index] == 100 && image[index+1] == 0 && image[index+2] == 0){
                pixelCoordinates.push_back({i, j});
            }
        }
    }
    vector<vector<int>> boundingBox;
    vector<int> pixel = pixelCoordinates[0];
    vector<int> previousPixel = pixel;
    do { 
        vector<vector<int>> neighbours = {{pixel[0] + 1, pixel[1]},
                                            {pixel[0],     pixel[1] + 1},
                                            {pixel[0] + 1, pixel[1] + 1},
                                            };
        if(pixel[0] != 0){
            neighbours.push_back({pixel[0] - 1, pixel[1] + 1});
            neighbours.push_back({pixel[0] - 1, pixel[1]    });
        }
        if(pixel[1] != 0){
            neighbours.push_back({pixel[0],     pixel[1] - 1});
            neighbours.push_back({pixel[0] + 1, pixel[1] - 1});
        }
        if(pixel[0] != 0 && pixel[1] != 0){
            neighbours.push_back({pixel[0] - 1, pixel[1] - 1});
        }

        for(auto neighbour: neighbours){
            int index = getIndex(neighbour[0], neighbour[1], width, height, channels);
            if(image[index] == 100 && image[index+1] == 0 && image[index+2] == 0){
                boundingBox.push_back(pixel);
                pixelCoordinates.erase(pixelCoordinates.begin());
                pixel = neighbour;
                break;
            }
        }
    } while(pixel != previousPixel && pixelCoordinates.size() != 0);
    cout << "Number of vertices intially: "<< boundingBox.size() << endl;
    for(int i = 0; i < boundingBox.size(); i += 2){
        boundingBox.erase(boundingBox.begin() + i);
    }
    cout << "number of vertices now: "<< boundingBox.size() << endl;

    ofstream file("vertices.txt");
    for(auto pixel: boundingBox){
        file << static_cast<float>(pixel[0])/width << " " << static_cast<float>(pixel[1])/height << endl;
    }
    file.close();
    cout << "Vertices saved successfully" << endl;
    
}


int main(){
    int width, height, channels;
    unsigned char* img = loadImage(&width, &height, &channels);
    cout << "Image loaded successfully" << endl;
    generateMatte(img, width, height, channels);
    cout << "Matte generated successfully" << endl;
    generateOutline(img, width, height, channels);
    cout << "Outline generated successfully" << endl;
    removeMatte(img, width, height, channels);
    cout << "Matte removed successfully" << endl;
    generateVertices(img, width, height, channels);
    stbi_image_free(img);

    return 0;
}