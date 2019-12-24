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

#include "cs.h"

CS::CS(const KA &ka, int sizex, int sizey, int num) :
        size_x(sizex), size_y(sizey),
        th_num(num) {
    //key
    for (std::size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            //cs1
            K2[i][j] = ka.K2[i][j], invK2[i][j] = ka.invK2[i][j];
            Kcs2[i][j] = ka.Kcs2[i][j], invKcs2[i][j] = ka.invKcs2[i][j];
            //cs2
            Kcs1[i][j] = ka.Kcs1[i][j], invKcs1[i][j] = ka.invKcs1[i][j];
        }
    }

}

CS::~CS() {

}

/**
 * 对图像进行加密
 * @param encryptimage  user传来的图像
 */
void CS::CS1encryptImage(mpz_class ****encryptimage) {
    REencryptimage = new mpz_class ***[size_x];
    for (int i = 0; i < size_x; ++i) {
        REencryptimage[i] = new mpz_class **[size_y];
        for (int j = 0; j < size_y; ++j) {
            REencryptimage[i][j] = new mpz_class *[4];
            for (int l = 0; l < 4; ++l) {
                REencryptimage[i][j][l] = new mpz_class[4];
            }
        }
    }
}
