# -*- coding: utf-8 -*-
'''
    @project:paggy
    @file:main.py
    @ide:PyCharm
    @time:2019-10-16 10:54
    @author:Sun
    @todo:
    @ref:
'''
import cv2
import init
from encrytion.encrypt import Encryption

# todo 读取图片
# img = io.imread(init.imagepath)  # length*width*(r,g,b)
img = cv2.imread(init.imagepath, cv2.IMREAD_GRAYSCALE)

# TP为密钥生成器并生成密钥
TP = Encryption(init.M, init.bits)
TP.gainParam()  # 生成密钥和参数
TP.writeFile(path='../param/')  # 将密钥写入到txt，目的是为了c++程序读取
print(len(img),len(img[0]))

# 将其他参数写入txt
with open('../param/param' + str(init.bits) + '.txt', 'a') as f:
    for it in [init.L, init.S, init.sigma, init.H]:
        print(it, file=f, end=' ')
