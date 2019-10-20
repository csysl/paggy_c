//
// Created by sun on 2019/10/19.
//

#include "user.h"

USER::USER(KA &ka, string &input, int &sizex, int &sizey) {
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

    uA = new mpz_class[uM], uB = new mpz_class[uM], uC = new mpz_class[uM];

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

USER::~USER() {
    for (int i = 0; i < size_x; ++i)delete[]grayimage[i], delete[]gaussimage[i], delete[]resultimage[i];
    delete[]grayimage, delete[]gaussimage, delete[]resultimage;
    delete[]uA, delete[]uB, delete[]uC;
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

//对原图像添加高斯噪生
void USER::addGaussNoise() {
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

void USER::gainABC(int &pixel) {
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

void USER::gainabc() {
    ua = func::chineseRemainder(uF, uA, uM) % uN;  //这里应该不会出错，中国同余定理自己测试过
    ub = func::chineseRemainder(uF, uB, uM) % uN;
    uc = func::chineseRemainder(uF, uC, uM) % uN;
}

void USER::gainDiag(int &pixel) {
    diag[0][0] = pixel;
    diag[1][1] = ua, diag[2][2] = ub, diag[3][3] = uc;
}

//对单个高斯图像进行加密，返回4*4矩阵
void USER::encryptpixel(int &pixel, mpz_class **enimg, mpz_class tmp[4][4]) {
    gainABC(pixel);
    gainabc();
    gainDiag(pixel);
    func::mul_matmod(uinvK, diag, uN, tmp);
    func::mul_matmod(tmp, uK, uN, enimg);
}

//对高斯图像进行加密
mpz_class ****USER::encryption() {
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
    mpz_class tmp[4][4];  //TODO 放在外面是为了节省时间，但不直观，实际节省的时间可以忽略不计
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            for (auto & ii : tmp)for (auto & jj : ii)jj = 0;
            encryptpixel(gaussimage[i][j], encryptimage[i][j], tmp);
        }
    }
    return encryptimage;
}

void USER::decryption(mpz_class ****denoseimage) {
    mpz_class tmp[4][4], tmp2[4][4];
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            for (int ii = 0; ii < 4; ++ii)for (int jj = 0; jj < 4; ++jj)tmp[ii][jj] = 0, tmp2[ii][jj] = 0;
            func::mul_matmod(uK, denoseimage[i][j], uN, tmp);
            func::mul_matmod(tmp, uinvK, uN, tmp2);
            resultimage[i][j] = tmp2[0][0].get_si();
        }
    }
}


/*todo 下面的函数用做测试*/

void USER::test(int a) {}

void USER::testaddgauss() {
    addGaussNoise();
    cout << "高斯图像是：" << endl;
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            cout << gaussimage[i][j] << " ";
        }
        cout << endl;
    }
    cout << "标准差是" << sigma << "时噪声图像和源图像的PSNR是：" << func::calPSNR(gaussimage, grayimage, size_x, size_y) << endl;

}

void USER::testendecry() {
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