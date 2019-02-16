#ifndef FY_FRACTION_HPP
#define FY_FRACTION_HPP

#include <cstdio>
#include ".\\Alice_Lib.hpp"
#include ".\\Integer.hpp"

typedef size_t size_type;


class Fraction{
private:
    Integer p, q;

    void simplify(){
        Integer common = gcd(p, abs(q));
        p /= common;
        q /= common;
    }

    void check() const{
        if(!p) throw("ZeroDivisionError");
    }

public:
    Fraction(): p(1), q(0){}
    Fraction(long long n): p(1), q(n){}
    Fraction(const Integer& a, const Integer& b)
        : p(abs(a)), q(b * sgn(a)) { simplify(); }
    template<size_type N>
        Fraction(const char (&s)[N]){
            sscanf(s, "%d / %d", &q, &p);
            q *= sgn(p);
            p = abs(p);
            check();
            simplify();
        }
    Fraction(const Fraction& frac): p(frac.p), q(frac.q){
        simplify();
        if(p < 0) frac.print();
    }

    Fraction& inverse(){
        Integer t = sgn(q) * p;
        p = abs(q);
        q = t;
        check();
        return *this;
    }

    //void print(char ending = '\n') const{
        //if(p == 1) printf("%d", q);
        //else printf("%d / %d", q, p);
        //if(ending) printf("%c", ending);
    //}

    void print(char ending = '\0') const{
        if(p == 1) q.print();
        else{
            q.print(' '), printf("/ "), p.print();
        }
        if(ending) printf("%c", ending);
    }

    Fraction& operator =(const Fraction& rhs){
        p = rhs.p;
        q = rhs.q;
        return *this;
    }

    Fraction operator -() const{
        Fraction res = *this;
        res.q = -res.q;
        return res;
    }
    bool operator !() const{
        return q == 0;
    }

    explicit operator bool(){
        return q != 0;
    }

    Fraction& operator +=(const Fraction& rhs){
        return (*this) = (*this) + rhs;
    }

    Fraction& operator -=(const Fraction& rhs){
        return (*this) = (*this) - rhs;
    }

    Fraction& operator *=(const Fraction& rhs){
        return (*this) = (*this) * rhs;
    }

    Fraction& operator /=(const Fraction& rhs){
        return (*this) = (*this) / rhs;
    }

    friend Fraction operator +(const Fraction& lhs, const Fraction& rhs);
    friend Fraction operator -(const Fraction& lhs, const Fraction& rhs);
    friend Fraction operator *(const Fraction& lhs, const Fraction& rhs);
    friend Fraction operator /(const Fraction& lhs, const Fraction& rhs);

    friend bool operator <(const Fraction& lhs, const Fraction& rhs);
    friend bool operator >(const Fraction& lhs, const Fraction& rhs);
    friend bool operator <=(const Fraction& lhs, const Fraction& rhs);
    friend bool operator >=(const Fraction& lhs, const Fraction& rhs);
    friend bool operator ==(const Fraction& lhs, const Fraction& rhs);
    friend bool operator !=(const Fraction& lhs, const Fraction& rhs);
};


Fraction operator +(const Fraction& lhs, const Fraction& rhs){
    Fraction res;
    res.p = lhs.p * rhs.p;
    res.q = lhs.q * rhs.p + rhs.q * lhs.p;
    res.simplify();
    return res;
}


Fraction operator -(const Fraction& lhs, const Fraction& rhs){
    return lhs + (-rhs);
}


Fraction operator *(const Fraction& lhs, const Fraction& rhs){
    Fraction res;
    res.p = lhs.p * rhs.p;
    res.q = lhs.q * rhs.q;
    res.simplify();
    return res;
}

Fraction operator /(const Fraction& lhs, const Fraction& rhs){
    Fraction divisor = rhs;
    return lhs * divisor.inverse();
}

bool operator <(const Fraction& lhs, const Fraction& rhs){
    return (lhs.q * rhs.p < rhs.q * lhs.p);
}

bool operator >(const Fraction& lhs, const Fraction& rhs){
    return rhs < lhs;
}

bool operator <=(const Fraction& lhs, const Fraction& rhs){
    return !(rhs > lhs);
}

bool operator >=(const Fraction& lhs, const Fraction& rhs){
    return !(rhs < lhs);
}

bool operator ==(const Fraction& lhs, const Fraction& rhs){
    if(lhs.p != rhs.p || lhs.q != rhs.q) return false;
    return true;
}

bool operator !=(const Fraction& lhs, const Fraction& rhs){
    return !(lhs == rhs);
}




#endif
