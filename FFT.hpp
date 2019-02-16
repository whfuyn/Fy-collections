#ifndef __ALICE_FFT_HPP__
#define __ALICE_FFT_HPP__

#include <complex>
#include <cmath>
#include "c:/code/project/vector/vector.hpp"
#include <iostream>
//#include "./Alice_Lib.hpp"

using std::swap;

using Numeric = std::complex<long double>;
using Coef = vector<Numeric>;

template<typename T>
void print(T coef)
{
    for(auto i : coef)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void BitReverseCopy(Coef& A)
{
    size_t n = A.size();
    static auto rev = [n](size_t k)
    {
        size_t res = 0, mask1 = n >> 1, mask2 = 1;
        while(mask1)
        {
            if(k & mask1) res |= mask2;
            mask1 >>= 1;
            mask2 <<= 1;
        }
        return res;
    };
    for(size_t k = 0; k < n; k++)
    {
        if(k < rev(k)) swap(A[k], A[rev(k)]);
    }
}


void DFT(Coef& A)
{
    size_t n = 1, lgn = 0;
    while(n < A.size()) n <<= 1, lgn++;
    BitReverseCopy(A);
    for(size_t s = 1; s <= lgn; s++)
    {
        size_t m = 1ULL << s;
        Numeric wm = exp(Numeric(0, 2 * acos(-1) / m));
        for(size_t k = 0; k < n; k += m)
        {
            Numeric w = 1;
            for(size_t j = 0; j < m / 2; j++)
            {
                Numeric t = w * A[k + j + m / 2];
                Numeric u = A[k + j];
                A[k + j] = u + t;
                A[k + j + m / 2] = u - t;
                w *= wm;
            }
        }
    }
}


void IDFT(Coef& A)
{
    size_t n = 1, lgn = 0;
    while(n < A.size()) n <<= 1, lgn++;
    BitReverseCopy(A);
    for(size_t s = 1; s <= lgn; s++)
    {
        size_t m = 1ULL << s;
        Numeric wm = exp(Numeric(0, -2 * acos(-1) / m));
        for(size_t k = 0; k < n; k += m)
        {
            Numeric w = 1;
            for(size_t j = 0; j < m / 2; j++)
            {
                Numeric t = w * A[k + j + m / 2];
                Numeric u = A[k + j];
                A[k + j] = u + t;
                A[k + j + m / 2] = u - t;
                w *= wm;
            }
        }
    }
    for(size_t k = 0; k < n; k++) A[k] /= n;
}

template<typename T>
T FFT(const T& a, const T& b)
{
    size_t n = 1, N = a.size() + b.size(); // a.size() + b.size() - 1, actually
    while(n < N) n <<= 1;
    Coef ya(n), yb(n), yc(n);
    for(size_t i = 0; i < a.size() || i < b.size(); i++)
    {
        if(i < a.size()) ya[i] = a[i];
        if(i < b.size()) yb[i] = b[i];
    }
    DFT(ya), DFT(yb);
    for(size_t i = 0; i < n; i++)
    {
        yc[i] = ya[i] * yb[i];
    }
    IDFT(yc);
    T res(n);
    for(size_t i = 0; i < n; i++)
    {
        res[i] = round(yc[i].real());
    }
    return res;
}

#endif
