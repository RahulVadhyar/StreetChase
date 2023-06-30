import PIL
from PIL import Image
import os
import numpy as np
import multiprocessing
IMG_DIR = "/home/starship/Documents/StreetChase/Art/Main Character/Animations/Running/"

final_img = np.array([[[]]])

paths = os.listdir(IMG_DIR)

def load_image(path):
    path = IMG_DIR + path
    img = Image.open(path)
    return np.array(img)

with multiprocessing.Pool() as p:
    output = p.map(load_image, paths)

final_img = np.concatenate(output)
print(final_img.shape)
    