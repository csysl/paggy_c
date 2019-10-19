# -*- coding: utf-8 -*-
'''
    @project:paggy
    @file:init.py
    @ide:PyCharm
    @time:2019-10-16 10:54
    @author:Sun
    @todo:
    @ref:
'''

import numpy as np

# todo image
# imagepath = 'testimage/1.jpg'
imagepath = '../img/lena.png'

# todo 局部均值滤波
L = 7  # 滤波窗口的大小   21                                7
S = 3  # 像素矩阵的宽度  必须是奇数  5                              3

sigma = 100  # 图片添加高斯噪声的**方差**，产生整数的噪声
sigma = np.sqrt(sigma)#.astype(int)
# step = 8  # 计算eij的线程数


# todo encryption
M = 4  # 加密算法的M值  #
bits = 32  # 加密算法产生的密钥长度为2*M*bits

# todo 去噪
H = 20  # 应该是k的10倍 180
SCAL = M * pow(2, bits)
