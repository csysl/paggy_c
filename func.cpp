//
// Created by sun on 2019/10/19.
//
#include "func.h"

double func::calPSNR(int **sourceImage, int **targetImage, int &size_x, int &size_y) {
    double MSE = 0;
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            MSE += pow((sourceImage[i][j] - targetImage[i][j]), 2);
        }
    }
    MSE /= double(size_x * size_y);
    auto PSNR = 20 * log10(255.0 / sqrt(MSE));
    return PSNR;
};

mpz_class func::mulInv(mpz_class a, mpz_class b) {
    mpz_class b0 = b, x0 = 0, x1 = 1, q, amb, xqx;

    if (b == 1) {
        return 1;
    }

    while (a > 1) {
        q = a / b;
        amb = a % b;
        a = b;
        b = amb;

        xqx = x1 - (q * x0);
        x1 = x0;
        x0 = xqx;
    }

    if (x1 < 0) {
        x1 += b0;
    }

    return x1;
}

mpz_class func::chineseRemainder(mpz_class *n, mpz_class *a, int len) {
    mpz_class prod = 1, p;
    for (int i = 0; i < len; i++) prod *= n[i];
    mpz_class sm = 0;
    for (int i = 0; i < len; i++) {
        p = prod / n[i];
        sm += a[i] * mulInv(p, n[i]) * p;
    }
    return sm % prod;
}

void func::mul_matmod(mpz_class a[4][4], mpz_class b[4][4], mpz_class &N, mpz_class res[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int l = 0; l < 4; ++l)
                res[i][j] += a[i][l] * b[l][j];
            res[i][j] %= N;
        }
    }
}

void func::mul_matmod(mpz_class a[4][4], mpz_class b[4][4], mpz_class &N, mpz_class **res) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int l = 0; l < 4; ++l)
                res[i][j] += a[i][l] * b[l][j];
            res[i][j] %= N;
        }
    }
}

void func::mul_matmod(mpz_class a[4][4], mpz_class **b, mpz_class &N, mpz_class res[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int l = 0; l < 4; ++l)
                res[i][j] += a[i][l] * b[l][j];
            res[i][j] %= N;
        }
    }
}