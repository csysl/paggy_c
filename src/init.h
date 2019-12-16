//
// Created by sun on 2019/10/18.
//

#ifndef PAGGY_C_INIT_H
#define PAGGY_C_INIT_H

#include <bits/stdc++.h>
#include <gmpxx.h>
#include <gmp.h>
#include "func/func.h"
#include "key/key.h"

using namespace std;
using namespace func;

/*USER*/
void userInit(KA &ka, string &input, int &sizex, int &sizey, int &num);
void addGaussNoise();
mpz_class ****userEncryption();
void userDecryption(mpz_class ****denoseimage);
void userErasememory();
//test
void usertestendecry();

/*CS*/

#endif //PAGGY_C_INIT_H
