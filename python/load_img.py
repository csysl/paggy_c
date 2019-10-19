import cv2
import os
import numpy as np

images_dir = '../img'

for root, dirs, files in os.walk(images_dir):
    for image_name in files:
        img = cv2.imread('../img/' + image_name, cv2.IMREAD_GRAYSCALE)
        # f=open('../img_text/' + image_name[:-4] + '.txt','w')
        # f.write(len(img)),f.write(len(img[0]))
        np.savetxt('../img_text/' + image_name[:-4] + '.txt', img, fmt='%d')
        # print(img)
