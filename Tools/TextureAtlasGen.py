import PIL
from PIL import Image
import os
import numpy as np
import multiprocessing
import math

IMG_DIR = "/home/starsystem/Documents/StreetChase/Art/Main Character/Animations/Running/"
MAX_IMAGES_IN_ROW = 4

def load_image(path):
    path = IMG_DIR + path
    img = Image.open(path)
    print(f"Processed image {path}")
    return np.array(img)

def create_texture_atlas():
    paths = os.listdir(IMG_DIR)
    with multiprocessing.Pool() as p:
        output = p.map(load_image, paths)
    columns = math.ceil(len(paths) / MAX_IMAGES_IN_ROW)
    rows = MAX_IMAGES_IN_ROW
    atlas = np.zeros((output[0].shape[0] * columns, output[0].shape[1] * rows, 4), dtype = np.uint8)
    print(f"The shape of atlas is {atlas.shape}")
    for i, img in enumerate(output):
        atlas[i // MAX_IMAGES_IN_ROW * img.shape[0]:(i // MAX_IMAGES_IN_ROW + 1) * img.shape[0],
               i % MAX_IMAGES_IN_ROW * img.shape[1]:(i % MAX_IMAGES_IN_ROW + 1) * img.shape[1]] = img
        print(f"Added image {i}/{len(output)} to atlas")
    print(f"Saving atlas to texture_atlas.png the shape is {atlas.shape}")
    Image.fromarray(atlas, 'RGBA').save('texture_atlas.png')
    print(f"The number of rows is {rows} and the number of columns is {columns}")
    print(f"The number of images is {len(output)}")

create_texture_atlas()