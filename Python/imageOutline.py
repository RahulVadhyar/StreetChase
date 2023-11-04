import PIL
from PIL import Image
import numpy as np
from itertools import product

def loadImage(path):
    image = Image.open(path)
    image = np.array(image)
    return image

def generateMatte(image):
    transformed_image = []
    for row in image:
        new_row = []
        for pixel in row:
            if(pixel[3] == 0):
                new_row.append([0,0,0])         
            else:
                new_row.append([255,255,255]) 
        transformed_image.append(new_row)
    return transformed_image

def generateOutline(image):
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            if((image[i][j] == [255,255,255]).all()):
                for x, y in product([i-1, i, i + 1], [j-1, j, j+1]):
                    if(x < 0 or y < 0 or x >= image.shape[0] or y >= image.shape[1]):
                        image[i][j] = [100, 0, 0]
                        break
                    pixel = image[x][y]
                    if((pixel == [0,0,0]).all()):
                        image[i][j] = [100, 0, 0]
                        break
    return image

def removeMatte(image):
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            if((image[i][j] == [255,255,255]).all()):
                image[i][j] = [0,0,0]
    return image

def generateVertices(image):
    pixelCoordinates = []
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            if((image[i][j] == [100, 0, 0]).all()):
                pixelCoordinates.append([i,j])

    boundingBoxes = []
    currentPixel = pixelCoordinates[0]
    while len(pixelCoordinates) != 0:
        boundingBox = []
        while True:
            i, j = currentPixel
            previousPixel = currentPixel
            for x, y in product([i-1, i, i + 1], [j-1, j, j+1]):
                if(x < 0 or y < 0 or x >= image.shape[0] or y >= image.shape[1]):
                    break
                pixel = image[x][y]
                if((pixel == [100, 0, 0]).all()):
                    currentPixel = [x,y]
                    boundingBox.append(currentPixel)
                    pixelCoordinates.remove(currentPixel)
                    break
            if(currentPixel == previousPixel):
                break
        boundingBoxes.append(boundingBox)
        if(len(pixelCoordinates) != 0):
            currentPixel = pixelCoordinates[0]
    return boundingBoxes



image = loadImage('test.png')
print(f"Image shape: {image.shape}")
print(f"Image type: {image.dtype}")

transformed_image = generateMatte(image)
transformed_image = np.array(transformed_image, dtype=np.uint8)
transformed_image = generateOutline(transformed_image)
transformed_image = removeMatte(transformed_image)
print("Generating vertices")
print(len(generateVertices(transformed_image)))




print(f"Transformed image shape: {transformed_image.shape}")
PIL.Image.fromarray(transformed_image).save('test2.png')
