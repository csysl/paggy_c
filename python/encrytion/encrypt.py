# -*- coding: utf-8 -*-
'''
    @project:pag2
    @file:encrytion.py
    @ide:PyCharm
    @time:2019-10-15 10:58
    @author:Sun
    @todo:
    @ref:
'''
import operator
from functools import reduce
from Cryptodome.Util import number as prime
from encrytion.mod import *
import random
import time
import numpy as np


# 产生一个大小为length*length的Zn内的矩阵
def gainMatinN(length, N):
    res = [[0] * length for i in range(length)]
    for i in range(length):
        for j in range(length):
            res[i][j] = prime.getRandomRange(0, N)  # a <= n < b
    return res


# 产生矩阵k的模N矩阵
def gainInvk(k, N):
    res = [[0] * 4 for i in range(4)]
    # 行列式
    det = gainMatrixDeternminant(k)
    if det % N != 0:
        # det 的模n逆元素
        invdet = prime.inverse(det % N, N)
        # 得到伴随矩阵
        adjmatrix = gainAdjMatrix(k)
        # 得到模逆矩阵
        for i in range(4):
            for j in range(4):
                res[i][j] = (adjmatrix[i][j] * invdet) % N
        return res

    print('当前矩阵不可逆')
    return True


# 检查两个矩阵是不是模n的逆矩阵
def check_KandInvK(K, invK, N):
    def checkone(m):
        for i in range(len(m)):
            for j in range(len(m)):
                if i == j and m[i][j] != 1:
                    return False
                elif i != j and m[i][j] != 0:
                    return False
        return True

    res = mul_matmod(K, invK, 4, N)
    if checkone(res):
        # print('生成的矩阵和模逆矩阵正确')
        return True
    else:
        # print('生成的矩阵和模逆矩阵不正确，重新生成矩阵和模逆矩阵')
        return False


# 检查两个矩阵相乘等不等于第三个矩阵
def check_K1andK2(K1, K2, K, N):
    n = len(K)
    tmp = mul_matmod(K1, K2, n, N)
    for i in range(n):
        for j in range(n):
            if tmp[i][j] != K[i][j]:
                return False
    return True


class Encryption:
    """实际只需要知道N,R,K,K'四个值即可"""

    def __init__(self, M, bits=512):
        self.__M = M
        self.__P, self.__Q, self.__F = [], [], []
        self.__N = -1
        self.__R = -1
        self.__A, self.__B, self.__C = [], [], []
        self.__a, self.__b, self.__c = -1, -1, -1
        self.__bits = bits

        # matrix
        self.__K, self.__invK = None, None  # 矩阵以及模逆矩阵
        self.__K1, self.__K2, self.__invK1, self.__invK2 = None, None, None, None
        self.__Kcs1, self.__Kcs2, self.__invKcs1, self.__invKsc2 = None, None, None, None
        self.__diag = None

    # todo 生成矩阵k和模逆矩阵k'  参数R
    def gainParam(self):
        stime = time.time()
        self.__gainPandQ()
        self.__gainF()
        self.__gainN()
        # self.__gainK()
        self.__gaininvK()  # 在此处执行gainK
        self.__gainKey1()  # 生成用户和服务器的密钥
        self.__gainKey2()  # 生成服务器之间的密钥
        self.__gainR()
        etime = time.time()
        print('生成key的时间是：%fs' % (etime - stime))

    # 随机产生一个明文
    def gainPlain(self):
        return prime.getRandomRange(0, self.__N)

    def test(self, plain):
        stime = time.time()
        self.__gainABC(plain)
        etime = time.time()
        print('产生ABC的时间是：%fs' % (etime - stime))
        stime = time.time()
        self.__gainabc()
        etime = time.time()
        print('产生abc的时间是：%fs' % (etime - stime))
        self.__A.clear(), self.__B.clear(), self.__C.clear()  # 多像素加密时清空，因为产生ABC使用的是append方式
        stime = time.time()
        self.__gainDiag(plain)  # 产生对角矩阵
        etime = time.time()
        print('产生对角矩阵的时间是：%fs' % (etime - stime))
        stime = time.time()
        res = mul_matmod(self.__invK1, self.__diag, 4, self.__N)
        res = mul_matmod(res, self.__K1, 4, self.__N)
        etime = time.time()
        print('加密的时间是：%fs' % (etime - stime))

        stime = time.time()
        res = mul_matmod(self.__invK2, res, 4, self.__N)
        res = mul_matmod(res, self.__K2, 4, self.__N)
        etime = time.time()
        print('加密的时间是：%fs' % (etime - stime))

        stime = time.time()
        res = mul_matmod(self.__Kcs2, res, 4, self.__N)
        res = mul_matmod(res, self.__invKcs2, 4, self.__N)
        etime = time.time()
        print('解密的时间是：%fs' % (etime - stime))

        stime = time.time()
        res = mul_matmod(self.__Kcs1, res, 4, self.__N)
        res = mul_matmod(res, self.__invKcs1, 4, self.__N)
        etime = time.time()
        print('解密的时间是：%fs' % (etime - stime))
        print(res[0][0])

        stime = time.time()
        res = mul_matmod(self.__invKcs1, res, 4, self.__N)
        res = mul_matmod(res, self.__Kcs1, 4, self.__N)
        etime = time.time()
        print('加密的时间是：%fs' % (etime - stime))

        stime = time.time()
        res = mul_matmod(self.__invKcs2, res, 4, self.__N)
        res = mul_matmod(res, self.__Kcs2, 4, self.__N)
        etime = time.time()
        print('加密的时间是：%fs' % (etime - stime))

        # stime = time.time()
        # res = np.dot(self.__invK, self.__diag) % self.__N
        # res = np.dot(res, self.__K) % self.__N
        # etime = time.time()
        # print('加密的时间是：%fs' % (etime - stime))
        return res

    # todo 加密密文
    def encryption(self, plain):
        stime = time.time()
        self.__gainABC(plain)
        etime = time.time()
        print('产生ABC的时间是：%fs' % (etime - stime))
        stime = time.time()
        self.__gainabc()
        etime = time.time()
        print('产生abc的时间是：%fs' % (etime - stime))
        self.__A.clear(), self.__B.clear(), self.__C.clear()  # 多像素加密时清空，因为产生ABC使用的是append方式
        stime = time.time()
        self.__gainDiag(plain)  # 产生对角矩阵
        etime = time.time()
        print('产生对角矩阵的时间是：%fs' % (etime - stime))
        stime = time.time()
        res = mul_matmod(self.__invK, self.__diag, 4, self.__N)
        res = mul_matmod(res, self.__K, 4, self.__N)
        etime = time.time()
        print('加密的时间是：%fs' % (etime - stime))
        stime = time.time()
        res = np.dot(self.__invK, self.__diag) % self.__N
        res = np.dot(res, self.__K) % self.__N
        etime = time.time()
        print('加密的时间是：%fs' % (etime - stime))
        return res

    # todo 解密
    def decryption(self, cipher):
        stime = time.time()
        for i in range(len(cipher)):
            for j in range(len(cipher[0])):
                cipher[i][j] %= self.__N

        res = mul_matmod(self.__K, cipher, 4, self.__N)
        res = mul_matmod(res, self.__invK, 4, self.__N)
        etime = time.time()
        print('解密的时间是：%fs' % (etime - stime))
        return res[0][0]

    def __gainPandQ(self):
        for i in range(self.__M):
            p, q = prime.getPrime(self.__bits), prime.getPrime(self.__bits)
            self.__P.append(p), self.__Q.append(q)
            continue

    def __gainF(self):
        for i in range(self.__M):
            self.__F.append(self.__P[i] * self.__Q[i])
        # print(self.__F)

    def __gainN(self):
        self.__N = reduce(operator.mul, self.__F)
        # print('%x'%(self.__F[0]*self.__F[1]))
        # print('%x'%self.__N)

    def __gainK(self):
        self.__K = gainMatinN(4, self.__N)

    def __gaininvK(self):
        while True:
            """while True:
                self.__gainK()
                det = gainMatrixDeternminant(self.__K)
                if det % self.__N != 0:
                    break
            # det 的模n逆元素
            invdet = prime.inverse(det % self.__N, self.__N)
            # 得到伴随矩阵
            adjmatrix = gainAdjMatrix(self.__K)
            # 得到模逆矩阵
            self.__invK = [[0] * 4 for i in range(4)]
            for i in range(4):
                for j in range(4):
                    self.__invK[i][j] = (adjmatrix[i][j] * invdet) % self.__N"""
            while True:
                self.__gainK()
                self.__invK = gainInvk(self.__K, self.__N)
                if self.__invK == True:
                    continue
                else:
                    break
            # 检查模逆矩阵的正确性
            if check_KandInvK(self.__K, self.__invK, self.__N):
                print('生成的密钥K和invK正确')
                break
            else:
                print('生成的密钥K和invK不正确！！！！')

    # TODO 生成用户和服务器的密钥
    def __gainKey1(self):
        # 产生密钥k1和k1'
        while True:
            while True:
                self.__K1 = gainMatinN(4, self.__N)
                self.__invK1 = gainInvk(self.__K1, self.__N)
                if self.__invK1 == True:
                    continue
                else:
                    break
            # 检查模逆矩阵的正确性
            if check_KandInvK(self.__K1, self.__invK1, self.__N):
                print('生成的密钥K1和invK1正确')
                break
            else:
                print('生成的密钥K1和invK1不正确！！！！')
        # 产生密钥k2和k2'
        self.__K2 = mul_matmod(self.__invK1, self.__K, 4, self.__N)
        self.__invK2 = gainInvk(self.__K2, self.__N)
        if check_KandInvK(self.__K2, self.__invK2, self.__N):
            print('生成的密钥K2和invK2正确')
        else:
            print('生成的密钥K2和invK2不正确！！！！')

        # check
        if check_K1andK2(self.__K1, self.__K2, self.__K, self.__N):
            print('生成的K1和K2是正确的')
        else:
            print('生成的K1和K2不正确！！！')
        if check_K1andK2(self.__invK2, self.__invK1, self.__invK, self.__N):
            print('生成的invK1和invK2是正确的')
        else:
            print('生成的invK1和invK2不正确！！！')

    # TODO 生成服务器之间的密钥
    def __gainKey2(self):
        # 产生密钥Kcs1和Kcs1'
        while True:
            while True:
                self.__Kcs1 = gainMatinN(4, self.__N)
                self.__invKcs1 = gainInvk(self.__Kcs1, self.__N)
                if self.__invKcs1 == True:
                    continue
                else:
                    break
            # 检查模逆矩阵的正确性
            if check_KandInvK(self.__Kcs1, self.__invKcs1, self.__N):
                print('生成的密钥Kcs1和invKcs1正确')
                break
            else:
                print('生成的密钥Kcs1和invKcs1不正确！！！！')

        # 产生密钥Kcs2和Kcs2'
        self.__Kcs2 = mul_matmod(self.__invKcs1, self.__K, 4, self.__N)
        self.__invKcs2 = gainInvk(self.__Kcs2, self.__N)
        if check_KandInvK(self.__Kcs2, self.__invKcs2, self.__N):
            print('生成的密钥Kcs2和invKcs2正确')
        else:
            print('生成的密钥Kcs2和invKcs2不正确！！！！')
        # check
        if check_K1andK2(self.__Kcs1, self.__Kcs2, self.__K, self.__N):
            print('生成的Kcs1和Kcs2是正确的')
        else:
            print('生成的Kcs1和Kcs2不正确！！！')
        if check_K1andK2(self.__invKcs2, self.__invKcs1, self.__invK, self.__N):
            print('生成的invKcs1和invKcs2是正确的')
        else:
            print('生成的invKcs1和invKcs2不正确！！！')

    def __gainR(self):
        self.__R = prime.getRandomRange(0, self.__N)

    def __gainABC(self, plain):
        for i in range(self.__M):
            tmp = random.random()
            # tmp=0.5
            if tmp < self.__M / (self.__M + 1):
                self.__A.append(plain)
                self.__B.append(self.__R)
                self.__C.append(self.__R)
            elif tmp >= self.__M / (self.__M + 1) and tmp < 1 - 1 / (2 * (self.__M + 1)):
                self.__A.append(self.__R)
                self.__B.append(plain)
                self.__C.append(self.__R)
            else:
                self.__A.append(self.__R)
                self.__B.append(self.__R)
                self.__C.append(plain)

    def __gainabc(self):
        from modint import chinese_remainder
        self.__a = chinese_remainder(self.__F, self.__A) % self.__N
        self.__b = chinese_remainder(self.__F, self.__B) % self.__N
        self.__c = chinese_remainder(self.__F, self.__C) % self.__N

    def __gainDiag(self, plain):
        self.__diag = [[0] * 4 for i in range(4)]
        self.__diag[0][0] = plain
        self.__diag[1][1], self.__diag[2][2], self.__diag[3][3] = self.__a, self.__b, self.__c

    def writeFile(self, path='./'):  # 将算法得到的信息记录到文本
        def writeK(f, K):
            for i in range(4):
                for j in range(4):
                    print(K[i][j], file=f, end=' ')
                # f.write('\n')
            f.write('\n')

        with open(path+'param'+str(self.__bits)+'.txt', 'w') as f:
            for it in [self.__N, self.__M, self.__R,self.__bits]:
                print(it, file=f, end=' ')
            f.write('\n')
            for it in self.__F:
                print(it, file=f, end=' ')
            f.write('\n')
            writeK(f, self.__K1)
            writeK(f, self.__invK1)
            writeK(f, self.__K2)
            writeK(f, self.__invK2)
            writeK(f, self.__Kcs2)
            writeK(f, self.__invKcs2)
            writeK(f, self.__Kcs1)
            writeK(f, self.__invKcs1)
            f.close()



    @property
    def N(self):
        return self.__N

    @property
    def M(self):
        return self.__M

    @property
    def R(self):
        return self.__R

    @property
    def F(self):
        return self.__F[:]

    @property
    def userK(self):
        return self.__K1[:], self.__invK1[:]

    @property
    def cs1K(self):
        return self.__K2[:], self.__invK2[:], self.__Kcs2[:], self.__invKcs2[:]

    @property
    def cs2K(self):
        return self.__Kcs1[:], self.__invKcs1[:]

    @property
    def K(self):
        return self.__K[:], self.__invK[:]


if __name__ == '__main__':
    encry = Encryption(10, 512)
    encry.gainParam()
    a = encry.gainPlain()
    print(a)
    a = encry.test(a)
    a = encry.decryption(a)
    print(a)
    # for i in range(3):
    #     a = encry.gainPlain()
    #     print('plaintext is %x' % a)
    #     res = encry.encryption(a)
    #     a = encry.decryption(res)
    #     print('decrypt result is %x' % a)
