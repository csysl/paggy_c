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

    if (b == 1) return 1;
    while (a > 1) {
        q = a / b;
        amb = a % b;
        a = b;
        b = amb;

        xqx = x1 - (q * x0);
        x1 = x0;
        x0 = xqx;
    }
    if (x1 < 0) x1 += b0;
    return x1;
}

mpz_class func::chineseRemainder(mpz_class *n, mpz_class *a, int len) {
    mpz_class prod = 1, p, sm = 0;
    for (int i = 0; i < len; i++) prod *= n[i];
    for (int i = 0; i < len; i++) {
        p = prod / n[i];
        sm += a[i] * p * mulInv(p, n[i]);
    }
    return sm % prod;
}

/*快速矩阵乘法，并无意义 https://www.geeksforgeeks.org/strassens-matrix-multiplication-algorithm-implementation/*/

inline mpz_class **func::MatrixMultiply(mpz_class **a, mpz_class **b, mpz_class &N, int n,
                                        int l, int m) {
    mpz_class **c = new mpz_class *[n];
    for (int i = 0; i < n; i++)
        c[i] = new mpz_class[m];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            c[i][j] = 0;
            for (int k = 0; k < l; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
            c[i][j] %= N;
        }
    }
    return c;
}

inline mpz_class **func::Strassen(mpz_class **a, mpz_class **b, mpz_class &N, int n, int l, int m) {
    if (n == 1 || l == 1 || m == 1)
        return MatrixMultiply(a, b,N, n, l, m);

    mpz_class **c = new mpz_class *[n];
    for (int i = 0; i < n; i++)
        c[i] = new mpz_class[m];

    int adjN = (n >> 1) + (n & 1);
    int adjL = (l >> 1) + (l & 1);
    int adjM = (m >> 1) + (m & 1);

    mpz_class ****As = new mpz_class ***[2];
    for (int x = 0; x < 2; x++) {
        As[x] = new mpz_class **[2];
        for (int y = 0; y < 2; y++) {
            As[x][y] = new mpz_class *[adjN];
            for (int i = 0; i < adjN; i++) {
                As[x][y][i] = new mpz_class[adjL];
                for (int j = 0; j < adjL; j++) {
                    int I = i + (x & 1) * adjN;
                    int J = j + (y & 1) * adjL;
                    As[x][y][i][j] = (I < n && J < l) ? a[I][J] : 0;
                }
            }
        }
    }

    mpz_class ****Bs = new mpz_class ***[2];
    for (int x = 0; x < 2; x++) {
        Bs[x] = new mpz_class **[2];
        for (int y = 0; y < 2; y++) {
            Bs[x][y] = new mpz_class *[adjN];
            for (int i = 0; i < adjL; i++) {
                Bs[x][y][i] = new mpz_class[adjM];
                for (int j = 0; j < adjM; j++) {
                    int I = i + (x & 1) * adjL;
                    int J = j + (y & 1) * adjM;
                    Bs[x][y][i][j] = (I < l && J < m) ? b[I][J] : 0;
                }
            }
        }
    }

    mpz_class ***s = new mpz_class **[10];
    for (int i = 0; i < 10; i++) {
        switch (i) {
            case 0:
                s[i] = new mpz_class *[adjL];
                for (int j = 0; j < adjL; j++) {
                    s[i][j] = new mpz_class[adjM];
                    for (int k = 0; k < adjM; k++) {
                        s[i][j][k] = Bs[0][1][j][k] - Bs[1][1][j][k];
                    }
                }
                break;
            case 1:
                s[i] = new mpz_class *[adjN];
                for (int j = 0; j < adjN; j++) {
                    s[i][j] = new mpz_class[adjL];
                    for (int k = 0; k < adjL; k++) {
                        s[i][j][k] = As[0][0][j][k] + As[0][1][j][k];
                    }
                }
                break;
            case 2:
                s[i] = new mpz_class *[adjN];
                for (int j = 0; j < adjN; j++) {
                    s[i][j] = new mpz_class[adjL];
                    for (int k = 0; k < adjL; k++) {
                        s[i][j][k] = As[1][0][j][k] + As[1][1][j][k];
                    }
                }
                break;
            case 3:
                s[i] = new mpz_class *[adjL];
                for (int j = 0; j < adjL; j++) {
                    s[i][j] = new mpz_class[adjM];
                    for (int k = 0; k < adjM; k++) {
                        s[i][j][k] = Bs[1][0][j][k] - Bs[0][0][j][k];
                    }
                }
                break;
            case 4:
                s[i] = new mpz_class *[adjN];
                for (int j = 0; j < adjN; j++) {
                    s[i][j] = new mpz_class[adjL];
                    for (int k = 0; k < adjL; k++) {
                        s[i][j][k] = As[0][0][j][k] + As[1][1][j][k];
                    }
                }
                break;
            case 5:
                s[i] = new mpz_class *[adjL];
                for (int j = 0; j < adjL; j++) {
                    s[i][j] = new mpz_class[adjM];
                    for (int k = 0; k < adjM; k++) {
                        s[i][j][k] = Bs[0][0][j][k] + Bs[1][1][j][k];
                    }
                }
                break;
            case 6:
                s[i] = new mpz_class *[adjN];
                for (int j = 0; j < adjN; j++) {
                    s[i][j] = new mpz_class[adjL];
                    for (int k = 0; k < adjL; k++) {
                        s[i][j][k] = As[0][1][j][k] - As[1][1][j][k];
                    }
                }
                break;
            case 7:
                s[i] = new mpz_class *[adjL];
                for (int j = 0; j < adjL; j++) {
                    s[i][j] = new mpz_class[adjM];
                    for (int k = 0; k < adjM; k++) {
                        s[i][j][k] = Bs[1][0][j][k] + Bs[1][1][j][k];
                    }
                }
                break;
            case 8:
                s[i] = new mpz_class *[adjN];
                for (int j = 0; j < adjN; j++) {
                    s[i][j] = new mpz_class[adjL];
                    for (int k = 0; k < adjL; k++) {
                        s[i][j][k] = As[0][0][j][k] - As[1][0][j][k];
                    }
                }
                break;
            case 9:
                s[i] = new mpz_class *[adjL];
                for (int j = 0; j < adjL; j++) {
                    s[i][j] = new mpz_class[adjM];
                    for (int k = 0; k < adjM; k++) {
                        s[i][j][k] = Bs[0][0][j][k] + Bs[0][1][j][k];
                    }
                }
                break;
        }
    }

    mpz_class ***p = new mpz_class **[7];
    p[0] = Strassen(As[0][0], s[0], N, adjN, adjL, adjM);
    p[1] = Strassen(s[1], Bs[1][1], N, adjN, adjL, adjM);
    p[2] = Strassen(s[2], Bs[0][0], N, adjN, adjL, adjM);
    p[3] = Strassen(As[1][1], s[3], N, adjN, adjL, adjM);
    p[4] = Strassen(s[4], s[5], N, adjN, adjL, adjM);
    p[5] = Strassen(s[6], s[7], N, adjN, adjL, adjM);
    p[6] = Strassen(s[8], s[9], N, adjN, adjL, adjM);

    for (int i = 0; i < adjN; i++) {
        for (int j = 0; j < adjM; j++) {
            c[i][j] = p[4][i][j] + p[3][i][j] - p[1][i][j] + p[5][i][j];
            if (j + adjM < m)
                c[i][j + adjM] = p[0][i][j] + p[1][i][j];
            if (i + adjN < n)
                c[i + adjN][j] = p[2][i][j] + p[3][i][j];
            if (i + adjN < n && j + adjM < m)
                c[i + adjN][j + adjM] = p[4][i][j] + p[0][i][j] - p[2][i][j] - p[6][i][j];
        }
    }

    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            for (int i = 0; i < adjN; i++) {
                delete[] As[x][y][i];
            }
            delete[] As[x][y];
        }
        delete[] As[x];
    }
    delete[] As;

    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            for (int i = 0; i < adjL; i++) {
                delete[] Bs[x][y][i];
            }
            delete[] Bs[x][y];
        }
        delete[] Bs[x];
    }
    delete[] Bs;

    for (int i = 0; i < 10; i++) {
        switch (i) {
            case 0:
            case 3:
            case 5:
            case 7:
            case 9:
                for (int j = 0; j < adjL; j++) {
                    delete[] s[i][j];
                }
                break;
            case 1:
            case 2:
            case 4:
            case 6:
            case 8:
                for (int j = 0; j < adjN; j++) {
                    delete[] s[i][j];
                }
                break;
        }
        delete[] s[i];
    }
    delete[] s;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < (n >> 1); j++) {
            delete[] p[i][j];
        }
        delete[] p[i];
    }
    delete[] p;
    for (int i = 0; i < adjN; i++) for (int j = 0; j < adjM; j++)c[i][j] %= N;


    return c;
}

//void func::mul_matmod(mpz_class a[4][4], mpz_class b[4][4], mpz_class &N, mpz_class res[4][4]) {
//    for (int i = 0; i < 4; ++i) {
//        for (int j = 0; j < 4; ++j) {
//            for (int l = 0; l < 4; ++l)
//                res[i][j] += a[i][l] * b[l][j];   //64次+，64次* 16次%
//            res[i][j] %= N;
//        }
//    }
//}

//void func::mul_matmod(mpz_class a[4][4], mpz_class b[4][4], mpz_class &N, mpz_class **res) {
//    for (int i = 0; i < 4; ++i) {
//        for (int j = 0; j < 4; ++j) {
//            for (int l = 0; l < 4; ++l)
//                res[i][j] += a[i][l] * b[l][j];
//            res[i][j] %= N;
//        }
//    }
//}
//
//void func::mul_matmod(mpz_class a[4][4], mpz_class **b, mpz_class &N, mpz_class res[4][4]) {
//    for (int i = 0; i < 4; ++i) {
//        for (int j = 0; j < 4; ++j) {
//            for (int l = 0; l < 4; ++l)
//                res[i][j] += a[i][l] * b[l][j];
//            res[i][j] %= N;
//        }
//    }
//}