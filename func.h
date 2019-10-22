//
// Created by sun on 2019/10/19.
//

#ifndef PAGGY_C_FUNC_H
#define PAGGY_C_FUNC_H

//#include "init.h"
#include <bits/stdc++.h>
#include <gmpxx.h>
#include <gmp.h>
#include <numeric>

using namespace std;

namespace func {
    double calPSNR(int **sourceImage, int **targetImage, int &size_x, int &size_y); //计算图相间的PSNR值
    mpz_class mulInv(mpz_class a, mpz_class b); //扩展欧几里得算法求解乘法逆元，求解中国同余定理时使用
    mpz_class chineseRemainder(mpz_class *n, mpz_class *a, int len); //中国同余定理，n是除数，a是被除数
    inline void mul_matmod(mpz_class a[4][4], mpz_class b[4][4], mpz_class &N, mpz_class res[4][4]){
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int l = 0; l < 4; ++l)
                    res[i][j] += a[i][l] * b[l][j];   //64次+，64次* 16次%
                res[i][j] %= N;
            }
        }
    };
    inline void mul_matmod(mpz_class a[4][4], mpz_class b[4][4], mpz_class &N, mpz_class **res){
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int l = 0; l < 4; ++l)
                    res[i][j] += a[i][l] * b[l][j];   //64次+，64次* 16次%
                res[i][j] %= N;
            }
        }
    };
    inline void mul_matmod(mpz_class a[4][4], mpz_class **b, mpz_class &N, mpz_class res[4][4]){
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int l = 0; l < 4; ++l)
                    res[i][j] += a[i][l] * b[l][j];   //64次+，64次* 16次%
                res[i][j] %= N;
            }
        }
    };
    inline mpz_class** MatrixMultiply(mpz_class** a, mpz_class** b, mpz_class &N,int n,int l, int m);
    inline mpz_class** Strassen(mpz_class** a, mpz_class** b, mpz_class &N,int n,int l, int m);
}

#endif //PAGGY_C_FUNC_H
