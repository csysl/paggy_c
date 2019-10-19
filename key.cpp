//
// Created by sun on 2019/10/18.
//
#include "key.h"

KA::KA(string &input) {
    ifstream in;
    in.open(input);
    if (!in) {
        cout << "没有输入文件！" << endl;
        return;
    }
    //得到N,M,R
    in >> N >> M >> R >> bits;
    F = new mpz_class[M];
    for (int i = 0; i < M; ++i)in >> F[i];

    string tmp;
    getline(in, tmp);  //这里getline的是因为，前面输入完F之后，in还停留在F那一行，此时该行为空，getline把这行取掉
    //定义给K赋值的lambda表达式
    auto getK = [&](mpz_class K[4][4]) {
        getline(in, tmp);
        istringstream strK(tmp);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                strK >> K[i][j];
            }
        }
    };
    //给K赋值
    getK(K1), getK(invK1);  //用户USER的key
    getK(K2), getK(invK2), getK(Kcs1), getK(invKcs1); //服务器CS1的key
    getK(Kcs2), getK(invKcs2); //服务器CS2的key
    getline(in, tmp);
    istringstream strK(tmp);
    strK >> L >> S >> sigma >> H;
    in.close();

//    for (int j = 0; j < 4; ++j) {
//        for (int l = 0; l < 4; ++l) {
//            cout << invK1[j][l] << " ";
//        }
//        cout << endl;
//    }
//    cout << N << endl;


}

KA::~KA() {
    delete[] F;
    /**
    for (int i = 0; i < 4; ++i) {
        delete[]K1[i];
        delete[]K2[i];
        delete[]Kcs1[i];
        delete[]Kcs2[i];
        delete[]invK1[i];
        delete[]invK2[i];
        delete[]invKcs1[i];
        delete[]invKcs2[i];
    }
    delete[]K1;
    delete[]K2;
    delete[]Kcs1;
    delete[]Kcs2;
    delete[]invK1;
    delete[]invK2;
    delete[]invKcs1;
    delete[]invKcs2;
     **/
}