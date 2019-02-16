#ifndef __ALICE_INTEGER_HPP__
#define __ALICE_INTEGER_HPP__

#include <cstdio>
#include <iostream>
#include <string>
#include "c:\code\project\vector\vector.hpp"
#include "c:\code\Lib\Alice_Lib.hpp"

typedef long long LL;
typedef size_t size_type;

class Integer;

int sgn(const Integer& N);
void swap(Integer& A, Integer& B);

class Integer{
private:
    static const int base = 1000000000; // 9 zeros
    int sign;
    vector<int> data;

    void push_back(int num){
        data.push_back(num);
    }

    void pop_back(){
        data.pop_back();
    }

    size_type size() const{
        return data.size();
    }

    int MSB() const{
        return data.back();
    }

    void clear(){
        data.clear();
    }

    void extend(size_type n){
        if(size() < n) data.resize(n, 0);
    }

    void rectify(){
        while(MSB() == 0) pop_back();
    }

    void shiftLeft(size_type n){
        data.insert(data.begin(), n, 0);
    }

    Integer getFullBit() const{
        return Integer(1) << size() * 31;
    }

    std::string toString() const{
        size_type sz = size() * 9 + 2;
        char buffer[sz];
        char * pBuffer = buffer;
        if(sign == 0){
            snprintf(pBuffer, sz, "0\n");
            return std::string(buffer);
        }
        if(sign == -1) snprintf(pBuffer++, sz--, "-");
        snprintf(pBuffer, sz, "%d", (*this)[size() - 1]);
        for(size_type index = size() - 1; index > 0; index--){
            while(*pBuffer) ++pBuffer, --sz;
            snprintf(pBuffer, sz, "%09d", (*this)[index - 1]);
        }
        return std::string(buffer);
    }

    int& operator [](size_type index){
        return data[index];
    }

    const int& operator [](size_type index) const{
        return data[index];
    }

    static int _absCmp(const Integer& lhs, const Integer& rhs);
    static void _divide(Integer& p, const Integer& q, Integer& _r);
    static Integer _bitOP(const Integer& lhs, const Integer& rhs, LL op(LL, LL));

public:
    Integer(): sign(0){}
    template<typename Num>
    Integer(Num n): sign(sgn(n)){
        while(n){
            push_back(abs(n % base));
            n /= base;
        }
    }
    Integer(const Integer& other): sign(other.sign), data(other.data){}
    Integer(Integer&& other) noexcept: sign(other.sign), data(std::move(other.data)){}
    Integer(const std::string& other): sign(0){
        size_type len = other.size();
        if(len == 1 && other[0] == 0) return;
        size_type cnt = 0;
        Integer res = 0, radix = 10;
        if(other[cnt] == '-') sign = -1, cnt++;
        else sign = 1;
        if(other[cnt] == '0'){
            if(other[++cnt] == 'x') cnt++, radix = 16;
            else radix = 8;
        }
        while(cnt < len){
            int digit = other[cnt] - '0';
            if(radix == 16 && digit >= 'a' -'0'){
                if(digit <= 'Z' - '0') digit = other[cnt] - 'A';
                else digit = other[cnt] - 'a';
            }
            res *= radix;
            res += digit;
            ++cnt;
        }
        data = std::move(res.data);
    }
    template<size_type N>
    Integer(const char (&other)[N]): sign(0){
        size_type len = N - 1;
        if(len == 1 && other[0] == 0) return;
        size_type cnt = 0;
        Integer res = 0, radix = 10;
        if(other[cnt] == '-') sign = -1, cnt++;
        else sign = 1;
        if(other[cnt] == '0'){
            if(other[++cnt] == 'x') cnt++, radix = 16;
            else radix = 8;
        }
        while(cnt < len){
            int digit = other[cnt] - '0';
            if(radix == 16 && digit >= 'a' -'0'){
                if(digit <= 'Z' - '0') digit = other[cnt] - 'A';
                else digit = other[cnt] - 'a';
            }
            res *= radix;
            res += digit;
            ++cnt;
        }
        data = std::move(res.data);
    }
    ~Integer(){}

    int getSign() const{
        return sign;
    }

    Integer pow(const Integer& times) const{
        Integer res = 1, mul = *this, n = times;
        while(n){
            if(n % 2) res *= mul;
            mul *= mul;
            n /= 2;
        }
        return res;
    }

    void print(char ending = '\0') const{
        if(sign == 0) printf("0");
        else{
            if(sign == -1) printf("-");
            printf("%d", (*this)[size() - 1]);
            for(size_type index = size() - 1; index > 0; index--){
                printf("%09d", (*this)[index - 1]);
            }
        }
        if(ending) printf("%c", ending);
    }

    void println() const{
        print('\n');
    }

    Integer operator -() const{
        Integer ret(*this);
        ret.sign = -sign;
        return ret;
    }

    Integer operator ~() const{
        return -(*this) - 1;
    }

    Integer& operator ++(){
        return (*this) += 1;
    }

    Integer operator ++(int){
        Integer ret = (*this);
        ++(*this);
        return ret;
    }

    Integer& operator --(){
        return (*this) -= 1;
    }

    Integer operator --(int){
        Integer ret = (*this);
        --(*this);
        return ret;
    }

    explicit operator bool() const{
        return (sign != 0);
    }

    Integer& operator =(const Integer& rhs){
        sign = rhs.sign;
        data = rhs.data;
        return (*this);
    }

    Integer& operator =(Integer&& rhs) noexcept{
        if(this != &rhs){
            sign = rhs.sign;
            data = std::move(rhs.data);
        }
        return *this;
    }

    Integer& operator +=(const Integer& rhs){
        return (*this) = (*this) + rhs;
    }

    Integer& operator -=(const Integer& rhs){
        return (*this) = (*this) - rhs;
    }

    Integer& operator *=(const Integer& rhs){
        return (*this) = (*this) * rhs;
    }

    Integer& operator /=(const Integer& rhs){
        return (*this) = (*this) / rhs;
    }

    Integer& operator %=(const Integer& rhs){
        return (*this) = (*this) % rhs;
    }

    Integer& operator |=(const Integer& rhs){
        return (*this) = (*this) | rhs;
    }

    Integer& operator &=(const Integer& rhs){
        return (*this) = (*this) & rhs;
    }

    Integer& operator ^=(const Integer& rhs){
        return (*this) = (*this) ^ rhs;
    }

    Integer& operator <<=(const Integer& rhs){
        return (*this) = (*this) << rhs;
    }

    Integer& operator >>=(const Integer& rhs){
        return (*this) = (*this) >> rhs;
    }

    friend Integer operator +(const Integer& lhs, const Integer& rhs);
    friend Integer operator -(const Integer& lhs, const Integer& rhs);
    friend Integer operator *(const Integer& lhs, const Integer& rhs);
    friend Integer operator /(const Integer& lhs, const Integer& rhs);
    friend Integer operator %(const Integer& lhs, const Integer& rhs);
    friend Integer operator |(const Integer& lhs, const Integer& rhs);
    friend Integer operator &(const Integer& lhs, const Integer& rhs);
    friend Integer operator ^(const Integer& lhs, const Integer& rhs);
    friend Integer operator <<(const Integer& lhs, const Integer& rhs);
    friend Integer operator >>(const Integer& lhs, const Integer& rhs);

    friend bool operator <(const Integer& lhs, const Integer& rhs);
    friend bool operator >(const Integer& lhs, const Integer& rhs);
    friend bool operator <=(const Integer& lhs, const Integer& rhs);
    friend bool operator >=(const Integer& lhs, const Integer& rhs);
    friend bool operator ==(const Integer& lhs, const Integer& rhs);
    friend bool operator !=(const Integer& lhs, const Integer& rhs);

    friend std::istream& operator >>(std::istream& is, Integer& rhs);
    friend std::ostream& operator <<(std::ostream& os, const Integer& rhs);

    friend void swap(Integer& A, Integer& B);

};

Integer operator +(const Integer& _lhs, const Integer& _rhs){
    if(_lhs.sign == 0) return _rhs;
    if(_rhs.sign == 0) return _lhs;

    Integer res;
    const int base = Integer::base;
    int op = (_lhs.sign == _rhs.sign ? 1 : -1);

    const Integer *p1 = &_lhs, *p2 = &_rhs;
    int cmp = Integer::_absCmp(*p1, *p2);
    if(cmp == 0 && op == -1) return res;
    if(cmp < 0) swap(p1, p2);

    const Integer& lhs = *p1;
    const Integer& rhs = *p2;

    res.sign = lhs.sign;

    int carry = 0;
    size_type i;
    for(i = 0; i < lhs.size(); i++){
        int acc = lhs[i] + carry;
        if(i < rhs.size()) acc += op * rhs[i];
        if(acc < 0){
            carry = -1;
            acc += base;
            res.push_back(acc);
        }
        else{
            carry = acc / base;
            res.push_back(acc % base);
        }
    }
    if(carry) res.push_back(carry);
    res.rectify();
    return res;
}

Integer operator -(const Integer& lhs, const Integer& rhs){
    return lhs + (-rhs);
}


Integer operator *(const Integer& lhs, const Integer& rhs){
    const int base = Integer::base;
    Integer res;
    res.sign = lhs.sign * rhs.sign;
    if(res.sign == 0) return res;
    res.extend(lhs.size() + rhs.size());
    for(size_type i = 0, j = 0; j < rhs.size(); j++){
        LL acc = 0, carry = 0;
        for(i = 0; i < lhs.size(); i++){
            acc = (LL)lhs[i] * rhs[j] + carry;
            res[i + j] += acc % base;
            carry = acc / base + res[i + j] / base;
            res[i + j] %= base;
        }
        res[i + j] += carry;
    }
    res.rectify();
    return res;
}

Integer operator /(const Integer& lhs, const Integer& rhs){
    if(!rhs) puts("Integer division by zero"), throw("ZeroDivisionError");
    if(!lhs) return 0;
    if(lhs.sign * rhs.sign == -1 && Integer::_absCmp(lhs, rhs) == -1) return -1;
    int sign = rhs.sign;
    Integer p = sign * lhs, q = sign * rhs, r = 0;
    Integer::_divide(p, q, r);
    return r;
}

Integer operator %(const Integer& lhs, const Integer& rhs){
    if(!rhs) puts("Integer modulo by zero"), throw("ZeroDivisionError");
    if(!lhs) return 0;
    if(lhs.sign * rhs.sign == -1 && Integer::_absCmp(lhs, rhs) == -1) return lhs + rhs;
    int sign = rhs.sign;
    Integer p = sign * lhs, q = sign * rhs, r = 0;
    Integer::_divide(p, q, r);
    return sign * p;
}

Integer operator |(const Integer& lhs, const Integer& rhs){
    auto op = [] (LL x1, LL x2) -> LL{
        return x1 | x2;
    };
    return Integer::_bitOP(lhs, rhs, op);
}

Integer operator &(const Integer& lhs, const Integer& rhs){
    auto op = [] (LL x1, LL x2) -> LL{
        return x1 & x2;
    };
    return Integer::_bitOP(lhs, rhs, op);
}

Integer operator ^(const Integer& lhs, const Integer& rhs){
    auto op = [] (LL x1, LL x2) -> LL{
        return x1 ^ x2;
    };
    return Integer::_bitOP(lhs, rhs, op);
}

bool operator <(const Integer& lhs, const Integer& rhs){
    if(lhs.sign < rhs.sign) return true;
    if(rhs.sign < lhs.sign) return false;
    int res = Integer::_absCmp(lhs, rhs);
    if(res == 0) return false;
    return (res < 0) == (lhs.sign > 0);
}

bool operator >(const Integer& lhs, const Integer& rhs){
    return rhs < lhs;
}

bool operator <=(const Integer& lhs, const Integer& rhs){
    return !(lhs > rhs);
}

bool operator >=(const Integer& lhs, const Integer& rhs){
    return !(lhs < rhs);
}

bool operator ==(const Integer& lhs, const Integer& rhs){
    if(lhs.sign != rhs.sign) return false;
    return Integer::_absCmp(lhs, rhs) == 0;
}

bool operator !=(const Integer& lhs, const Integer& rhs){
    return !(lhs == rhs);
}

Integer operator <<(const Integer& lhs, const Integer& rhs){
    return lhs * Integer(2).pow(rhs);
}

Integer operator >>(const Integer& lhs, const Integer& rhs){
    return lhs / Integer(2).pow(rhs);
}

std::istream& operator >>(std::istream& is, Integer& rhs){
    std::string ip;
    is >> ip;
    rhs = Integer(ip);
    return is;
}

std::ostream& operator <<(std::ostream& os, const Integer& rhs){
    return os << rhs.toString();
}

int sgn(const Integer& N){
    return N.getSign();
}

void swap(Integer& A, Integer& B){
    swap(A.sign, B.sign);
    swap(A.data, B.data);
}

int Integer::_absCmp(const Integer& lhs, const Integer& rhs){
    if(lhs.size() < rhs.size()) return -1;
    if(rhs.size() < lhs.size()) return 1;
    for(size_type i = lhs.size(); i > 0; i--){
        if(lhs[i - 1] < rhs[i - 1]) return -1;
        if(rhs[i - 1] < lhs[i - 1]) return 1;
    }
    return 0;
}

void Integer::_divide(Integer& p, const Integer& q, Integer& _r){
    if(Integer::_absCmp(p, q) == -1) return;
    Integer r = 0;
    if(p.size() > q.size()){
        LL acc = (LL)p[p.size() - 1] * Integer::base + p[p.size() - 2];
        r = acc / q.MSB();
        r.shiftLeft(p.size() - q.size() - 1);
        p -= r * q;
    }
    else{
        r = p.MSB() / q.MSB();
        r.shiftLeft(p.size() - q.size());
        p -= r * q;
    }
    int factor = 1;
    while(p < 0){
        Integer recover = 1;
        if(Integer::_absCmp(p, q) >= 0){
            if(p.size() > q.size()){
                LL acc = (LL)p[p.size() - 1] * Integer::base + p[p.size() - 2];
                recover = acc / (q.MSB() + factor);
                recover.shiftLeft(p.size() - q.size() - 1);
            }
            else{
                recover = p.MSB() / (q.MSB() + factor);
                recover.shiftLeft(p.size() - q.size());
            }
        }
        r -= recover;
        p += recover * q;
        factor = 0;
    }
    _r += r;
    _divide(p, q, _r);
}

Integer Integer::_bitOP (const Integer& lhs, const Integer& rhs, LL op(LL, LL))
{
    Integer fullBit;
    if(lhs.getSign() == -1 || rhs.getSign() == -1)
        fullBit = Integer::_absCmp(lhs, rhs) >= 0 ? lhs.getFullBit() : rhs.getFullBit();
    Integer a, b, res = 0, bit = 1, radix = Integer(1) << 58;
    a = lhs >= 0 ? lhs : fullBit + lhs;
    b = rhs >= 0 ? rhs : fullBit + rhs;
    while(a || b){
        LL x = 0, y = 0;
        Integer r1 = 0, r2 = 0;
        Integer::_divide(a, radix, r1);
        Integer::_divide(b, radix, r2);
        swap(a, r1);
        swap(b, r2);
        if(r1.size() >= 1) x = r1[r1.size() - 1];
        if(r1.size() >= 2) x *= base, x += r1[r1.size() - 2];
        if(r2.size() >= 1) y = r2[r2.size() - 1];
        if(r2.size() >= 2) y *= base, y += r2[r2.size() - 2];
        res += op(x, y) * bit;
        bit <<= 58;
    }
    if(fullBit && res >= fullBit / 2){
        res = fullBit - res;
        res.sign = -1;
    }
    return res;
}




#endif

