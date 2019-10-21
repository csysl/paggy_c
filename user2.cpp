/*  coding: utf-8
    @project:paggy_c
    @file:user2.h
    @ide:CLion
    @time:2019-10-21 10:53
    @author:sun
    @todo:
    @ref:
*/

#include "init.h"
#include "key.h"

int size_x, size_y, size_i, th_num = 1;
double sigma;
int **grayimage, **gaussimage, **resultimage;
int uM;
mpz_class uSCAL = 1, *uF, uN, uR;
mpz_class uK[4][4], uinvK[4][4];
mpz_class ****encryptimage;

void userInit(KA &ka, string &input, int &sizex, int &sizey, int &num) {
    th_num = num;
    size_x = sizex, size_y = sizey;
    size_i = size_x * size_y;
    sigma = ka.sigma;   //cout<<sigma<<endl;
    uM = ka.M, uN = ka.N, uR = ka.R, uF = ka.F;
    uSCAL <<= ka.bits, uSCAL *= uM;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            uK[i][j] = ka.K1[i][j], uinvK[i][j] = ka.invK1[i][j];
        }
    }
    //cout << uSCAL << endl;

    //读取图片
    ifstream in;
    in.open(input);
    if (!in) {
        cout << "图片不存在！" << endl;
        return;
    }
    grayimage = new int *[size_x], gaussimage = new int *[size_x], resultimage = new int *[size_x];
    for (int i = 0; i < size_x; ++i)
        grayimage[i] = new int[size_y](), gaussimage[i] = new int[size_y](), resultimage[i] = new int[size_y]();
    string tmp;
    for (int i = 0; i < size_x; ++i) {
        getline(in, tmp);
        istringstream tpixel(tmp);
        for (int j = 0; j < size_y; ++j) {
            tpixel >> grayimage[i][j];
        }
    }
    in.close();
    addGaussNoise();//添加高斯噪声
}

void userErasememory() {
    /*uF的内存由key agent释放*/
    for (int i = 0; i < size_x; ++i)delete[]grayimage[i], delete[]gaussimage[i], delete[]resultimage[i];
    delete[]grayimage, delete[]gaussimage, delete[]resultimage;
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            for (int l = 0; l < 4; ++l)
                delete[]encryptimage[i][j][l];
            delete[]encryptimage[i][j];
        }
        delete[]encryptimage[i];
    }
    delete[]encryptimage;
}

void addGaussNoise() {
    random_device r;
    default_random_engine e{r()};
    normal_distribution<double> gauss{0, sigma};
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            gaussimage[i][j] = grayimage[i][j] + int(gauss(e));
            if (gaussimage[i][j] > 255)gaussimage[i][j] = 255;
            else if (gaussimage[i][j] < 0)gaussimage[i][j] = 0;
        }
    }
}

void gainABC(int &pixel, mpz_class *uA, mpz_class *uB, mpz_class *uC) {
    random_device r;
    default_random_engine e(r());
    uniform_real_distribution<double> u(0.0, 1.0);
    for (int i = 0; i < uM; ++i) {
        auto tmp = u(e);
        if (tmp < double(uM) / double(uM + 1)) {
            uA[i] = pixel, uB[i] = uR, uC[i] = uR;
            continue;
        } else if (tmp >= double(uM) / double(uM + 1) && tmp < (1.0 - (1.0 / (2.0 * double(uM + 1))))) {
            uA[i] = uR, uB[i] = pixel, uC[i] = uR;
            continue;
        } else {
            uA[i] = uR, uB[i] = uR, uC[i] = pixel;
        }
    }
}

void gainabc(mpz_class &ua, mpz_class &ub, mpz_class &uc, mpz_class *uA, mpz_class *uB, mpz_class *uC) {
    ua = func::chineseRemainder(uF, uA, uM) % uN;  //这里应该不会出错，中国同余定理自己测试过
    ub = func::chineseRemainder(uF, uB, uM) % uN;
    uc = func::chineseRemainder(uF, uC, uM) % uN;
}

void gainDiag(int &pixel, mpz_class &ua, mpz_class &ub, mpz_class &uc, mpz_class diag[4][4]) {
    diag[0][0] = pixel;
    diag[1][1] = ua, diag[2][2] = ub, diag[3][3] = uc;
}

//对单个高斯图像进行加密，返回4*4矩阵
void encryptpixel(int start, int end, int **scrimg, mpz_class ****enimg) {
    mpz_class tmp[4][4], diag[4][4];
    auto *uA = new mpz_class[uM], *uB = new mpz_class[uM], *uC = new mpz_class[uM];
    mpz_class ua, ub, uc;
    for (int i = start; i < end; ++i) {
        for (int j = 0; j < size_y; ++j) {
            for (auto &ii : tmp)for (auto &jj : ii)jj = 0;
            gainABC(scrimg[i][j], uA, uB, uC);
            gainabc(ua, ub, uc, uA, uB, uC);
            gainDiag(scrimg[i][j], ua, ub, uc, diag);
            func::mul_matmod(uinvK, diag, uN, tmp);
            func::mul_matmod(tmp, uK, uN, enimg[i][j]);
        }
    }
    delete[]uA, delete[]uB, delete[]uC;
}

//对高斯图像进行加密
mpz_class ****userEncryption() {
    //初始化加密后的图像
    encryptimage = new mpz_class ***[size_x];
    for (int i = 0; i < size_x; ++i) {
        encryptimage[i] = new mpz_class **[size_y];
        for (int j = 0; j < size_y; ++j) {
            encryptimage[i][j] = new mpz_class *[4];
            for (int l = 0; l < 4; ++l) {
                encryptimage[i][j][l] = new mpz_class[4];
            }
        }
    }
    cout << "标准差是" << sigma << "时噪声图像和源图像的PSNR是：" << func::calPSNR(grayimage, gaussimage, size_x, size_y) << endl;

    //图像进行加密
    std::thread th[size_x / th_num];
    for (int i = 0; i < size_x / th_num; ++i) {
        th[i] = thread(
                [=]() {
                    encryptpixel(i * th_num, (i + 1) * th_num, gaussimage, encryptimage);
                }
        );
    }
    for (int i = 0; i < size_x / th_num; ++i)th[i].join();
    //uF回空
    uF = nullptr;
    return encryptimage;
}

void userDecryption(mpz_class ****denoseimage) {
    mpz_class tmp[4][4], tmp2[4][4];
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            for (int ii = 0; ii < 4; ++ii)for (int jj = 0; jj < 4; ++jj)tmp[ii][jj] = 0, tmp2[ii][jj] = 0;
            func::mul_matmod(uK, denoseimage[i][j], uN, tmp);
            func::mul_matmod(tmp, uinvK, uN, tmp2);
            resultimage[i][j] = tmp2[0][0].get_si();  //todo 这里没有除SCAL ！！！！！！！！！！！！！！！！！！！11
        }
    }
}

/*以下函数用做测试*/
void usertestendecry() {
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            cout << gaussimage[i][j] << " ";
        }
        cout << endl;
        for (int j = 0; j < size_y; ++j) {
            cout << resultimage[i][j] << " ";
        }
        cout << endl;
    }
}
