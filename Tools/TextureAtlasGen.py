import PIL
from PIL import Image
import os
import numpy as np
import multiprocessing
import math

MAX_IMAGES_IN_ROW = 10
img_dir = "Assets/Export/Animations/Police/Walking/"
save_dir = "Textures/Animations/Police/walking.png"
current_img_dir = img_dir

def load_image(path, img_dir):
    path = img_dir + path
    img = Image.open(path)
    print(f"Processed image {path}")
    return np.array(img)

def mp_load_image(path):
    return load_image(path, current_img_dir)

def create_texture_atlas(img_dir):
    paths = os.listdir(img_dir)
    
    with multiprocessing.Pool() as p:
        output = p.map(mp_load_image, paths)
    columns = math.ceil(len(paths) / MAX_IMAGES_IN_ROW)
    rows = MAX_IMAGES_IN_ROW
    atlas = np.zeros((output[0].shape[0] * columns, output[0].shape[1] * rows, 4), dtype = np.uint8)
    print(f"The shape of atlas is {atlas.shape}")
    for i, img in enumerate(output):
        atlas[i // MAX_IMAGES_IN_ROW * img.shape[0]:(i // MAX_IMAGES_IN_ROW + 1) * img.shape[0],
               i % MAX_IMAGES_IN_ROW * img.shape[1]:(i % MAX_IMAGES_IN_ROW + 1) * img.shape[1]] = img
        print(f"Added image {i}/{len(output)} to atlas")
    print(f"Saving atlas to texture_atlas.png the shape is {atlas.shape}")
    Image.fromarray(atlas, 'RGBA').save(save_dir)
    print(f"The number of rows is {rows} and the number of columns is {columns}")
    print(f"The number of images is {len(output)}")

if __name__ == "__main__":
    create_texture_atlas(img_dir)