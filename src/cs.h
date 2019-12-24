/* -*- coding: utf-8 -*-*/
/*
    @project:paggy_c
    @file:cs.h
    @ide:CLion
    @time:2019-10-21 02:18
    @author:sun
    @todo:
    @ref:
*/

#ifndef PAGGY_C_CS_H
#define PAGGY_C_CS_H

#include "init.h"
#include "key/key.h"


class CS{
private:
    int size_x, size_y, size_i, th_num = 1;

    mpz_class K2[4][4], invK2[4][4], Kcs2[4][4], invKcs2[4][4];  //cs1
    mpz_class Kcs1[4][4], invKcs1[4][4];  //cs2

    mpz_class ****REencryptimage= nullptr;  //user加密图像二次加密后图像

public:
    explicit CS(const KA &ka,int sizex, int sizey, int num);
    ~CS();

    void CS1encryptImage(mpz_class ****encryptimage= nullptr);
};

#endif //PAGGY_C_CS_H
