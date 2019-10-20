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
    void mul_matmod(mpz_class a[4][4], mpz_class b[4][4], mpz_class &N, mpz_class res[4][4]);
    void mul_matmod(mpz_class a[4][4], mpz_class b[4][4], mpz_class &N, mpz_class **res);
    void mul_matmod(mpz_class a[4][4], mpz_class **b, mpz_class &N, mpz_class res[4][4]);
}

#endif //PAGGY_C_FUNC_H
