//
// Created by sun on 2019/10/18.
//

#ifndef PAGGY_C_KEY_H
#define PAGGY_C_KEY_H

#include "init.h"

class KA {
public:
    int M, bits;
    int L, S, H;  //去噪的参数，暂存在这里
    double sigma;
    mpz_class N, R;  //存放N,M,R
    mpz_class *F;
    mpz_class K1[4][4], invK1[4][4];
    mpz_class K2[4][4], invK2[4][4], Kcs2[4][4], invKcs2[4][4];
    mpz_class Kcs1[4][4], invKcs1[4][4];


public:
    explicit KA(string &input);

    ~KA();


};

#endif //PAGGY_C_KEY_H
