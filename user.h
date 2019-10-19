//
// Created by sun on 2019/10/19.
//

#ifndef PAGGY_C_USER_H
#define PAGGY_C_USER_H

#include "init.h"
#include "key.h"

class USER {
private:
    int size_x, size_y, size_i;
    double sigma;
    int **grayimage, **gaussimage;
    int uM;
    mpz_class uN, uR, ua, ub, uc, uSCAL = 1;  //存放N,M,R
    mpz_class *uF, *uA, *uB, *uC;
    mpz_class uK[4][4], uinvK[4][4], diag[4][4];
    mpz_class ****encryptimage;

private:
    void addGaussNoise(); //对原图像添加高斯噪生
    void encryptpixel(int &pixel, mpz_class **enimg); //对高斯图像的单个像素进行加密
    void gainABC(int &pixel), gainabc(),gainDiag(int &pixel);

public:
    explicit USER(KA &ka, string &input, int &sizex, int &sizey);

    ~USER();

    mpz_class ****encryption(); //对高斯图像进行加密


    //todo 下面是测试函数
    void testaddgauss();
};

#endif //PAGGY_C_USER_H
