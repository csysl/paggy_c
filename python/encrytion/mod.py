# -*- coding: utf-8 -*-
'''
    @project:pag2
    @file:matrix.py
    @ide:PyCharm
    @time:2019-10-15 16:33
    @author:Sun
    @todo:
    @ref:
'''

from functools import reduce

# 求解a的模n的逆元素
def mul_inv(a, n):
    b = n
    x0, x1 = 0, 1
    if n == 1: return 1
    while a > 1:
        q = a // n
        a, n = n, a % n
        x0, x1 = x1 - q * x0, x0
    if x1 < 0: x1 += b
    return x1

def chineseremainder(n, a):  # n是模数，a是被模数
    sum = 0
    prod = reduce(lambda a, b: a * b, n)
    for n_i, a_i in zip(n, a):
        p = prod // n_i
        sum += a_i * mul_inv(p, n_i) * p
    return sum % prod

# 定义矩阵a*矩阵b 模 m,n是a,b的维度， 定义a,b为方阵
def mul_matmod(a, b, n, m):
    res = [[0] * n for i in range(n)]
    for i in range(n):
        for j in range(n):
            for k in range(n):
                res[i][j] += a[i][k] * b[k][j]
            res[i][j] %= m
    return res

 #得到去除第i行第j列的矩阵
def gainMatrixMinor(m, i, j):
    return [row[:j] + row[j + 1:] for row in (m[:i] + m[i + 1:])]

# 求矩阵的行列式
def gainMatrixDeternminant(m):
    """
    https://stackoverflow.com/questions/32114054/matrix-inversion-without-numpy
    :param m:
    :return:
    """
    # base case for 2x2 matrix
    if len(m) == 2:
        return m[0][0] * m[1][1] - m[0][1] * m[1][0]

    determinant = 0
    for c in range(len(m)):
        determinant += ((-1) ** c) * m[0][c] * gainMatrixDeternminant(gainMatrixMinor(m, 0, c))
    return determinant

# m是矩阵，求解伴随矩阵
def gainAdjMatrix(m):
    n=len(m)
    res = [[0] * n for i in range(n)]
    for i in range(n):
        for j in range(n):
            # 系数
            if (i + j) & 1:
                coef = -1
            else:
                coef = 1
            # 生成去除第i行第j列的矩阵
            tmp=gainMatrixMinor(m,i,j)
            res[j][i]=coef*gainMatrixDeternminant(tmp)
    return res

if __name__=='__main__':
    import random
    a = [[0] * 4 for i in range(4)]
    print(a)
    a = [a[:] for i in range(4)]
    for i in range(4):
        for j in range(4):
            a[i][j] = random.randint(1,100)
    print(a)

    res=gainAdjMatrix(a)
    print(res)

    pass

