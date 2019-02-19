#ifndef __ALICE_LIB_HPP__
#define __ALICE_LIB_HPP__

#include <cstdio>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "c:\code\project\vector\vector.hpp"

static struct __ALICE_RANDOM_SEED{
    __ALICE_RANDOM_SEED(){
        srand(time(0));
    }
} __ALICE_RANDOM_INIT;

typedef long long LL;
typedef size_t size_type;
typedef unsigned char byte;

#define dbg(x) std::cout << #x " = " << x << std::endl

/*
 *size_type strlen(const char *str){
 *    size_type res = 0;
 *    while(*str++) res++;
 *    return res;
 *}
 */

template<typename T>
void swap(T& A, T& B){
    T tmp(std::move(A));
    A = std::move(B);
    B = std::move(tmp);
}

template<typename T>
void showBits(const T& item, bool withSpace = true, char ending = '\n'){
    size_type length = sizeof(T);
    byte * bit = (byte*) &item;
    for(size_type i = 0; i < length; i++){
        byte mask = 1 << 7;
        while(mask){
            if(*(bit + i) & mask) printf("1");
            else printf("0");
            mask >>= 1;
        }
        if(withSpace) printf(" ");
    }
    if(ending) printf("%c", ending);
}

template<typename T>
void reverse(T begin, T end){
    while(begin < end){
        swap(*(begin++), *(--end));
    }
}

template<typename T>
T gcd(const T& a, const T& b){
    T p = a, q = b, t;
    while(q){
        t = p;
        p = q;
        q = t % q;
    }
    return p;
}

template<typename T>
T lcm(const T& a, const T& b){
    return a * b / gcd(a, b);
}

template<typename T>
T factorial(const T& a){
    const T& res = 1;
    for(int i = 1; i <= a; i++){
        res *= i;
    }
    return res;
}

int sgn(LL a){
    if(a < 0) return -1;
    else if(a > 0) return 1;
    return 0;
}

template<typename Iter_type>
void shuffle(Iter_type begin, Iter_type end){
    size_type cur = 0;
    for(Iter_type it = begin; it != end; it++){
        swap(*it, *(begin + rand() % (++cur)));
    }
}

template<typename T>
T& max(T& A, T& B){
    return A < B ? B : A;
}

template<typename T>
T& max(T& A, T& B, T& C){
    return max(max(A, B), C);
}

template<typename T>
const T& max(const T& A, const T& B){
    return A < B ? B : A;
}

template<typename T>
const T& max(const T& A, const T& B, const T& C){
    return max(max(A, B), C);
}

template<typename T>
T& min(T& A, T& B){
    return A < B ? A : B;
}

template<typename T>
T& min(T& A, T& B, T& C){
    return min(min(A, B), C);
}

template<typename T>
const T& min(const T& A, const T& B){
    return A < B ? A : B;
}

template<typename T>
const T& min(const T& A, const T& B, const T& C){
    return min(min(A, B), C);
}

template<typename T>
T abs(const T& A){
    return A < 0 ? -A : A;
}

template<typename Func, typename Iter_type>
void map(Func fn, Iter_type begin, Iter_type end){
    while(begin != end){
        *begin = fn(*begin);
        begin++;
    }
}

template<typename T, typename Iter_type>
T reduce(T fn(T, T), Iter_type begin, Iter_type end){
    reduce(fn, begin + 1, end, *begin);
}

template<typename T, typename Iter_type>
T reduce(T fn(T, T), Iter_type begin, Iter_type end, T initial){
    T res = initial;
    while(begin != end){
        res = fn(res, *begin++);
    }
    return res;
}


template<typename T>
class Queue : protected vector<T>{
public:
    size_type size(){
        return this->vector<T>::size();
    }
    void push(const T& elem){
        this->push_back(elem);
    }
    void pop(){
        this->pop_back(); // TODO: Queue's pop operation isn't in such a manner. Fix it.
    }
    T& front(){
        return this->vector<T>::front();
    }
    T& back(){
        return this->vector<T>::back();
    }
    bool empty(){
        return this->vector<T>::empty();
    }
};

template<typename T>
class Less{
public:
    bool operator()(const T& lhs, const T& rhs){
        return lhs < rhs;
    }
};

template<typename T>
class Greater{
public:
    bool operator()(const T& lhs, const T& rhs){
        return rhs < lhs;
    }
};

template<typename T, typename Compare = Greater<T> >
class Heap : protected vector<T>{
protected:
    static Compare cmp;
    void heapify_up(size_type index){
        if(index == 1) return;
        vector<T> & data = (*this);
        size_type pre_index = index >> 1;
        T& cur = data[index - 1];
        T& pre = data[pre_index - 1];
        if(cmp(cur, pre)){
            swap(cur, pre);
            heapify_up(pre_index);
        }
    }

    void heapify_down(size_type index){
        vector<T> & data = (*this);
        size_type lch_index, rch_index, max_index;
        max_index = index;
        lch_index = index << 1;
        rch_index = lch_index + 1;
        if(lch_index <= size()){
            if(cmp(data[lch_index - 1], data[max_index - 1])) max_index = lch_index;
        }
        if(rch_index <= size()){
            if(cmp(data[rch_index - 1], data[max_index - 1])) max_index = rch_index;
        }
        if(max_index != index){
            swap(data[index - 1], data[max_index - 1]);
            heapify_down(max_index);
        }
    }

    void buildHeap(){
        size_type sz = size() >> 1;
        while(sz) heapify_down(sz--);
    }

public:
    Heap(){
        buildHeap();
    }

    Heap(const vector<T> & other): vector<T>::vector(other){
        buildHeap();
    }

    ~Heap(){}

    size_type size() const{
        return this->vector<T>::size();
    }

    bool empty() const{
        return this->vector<T>::empty();
    }

    void push(const T& elem){
        this->push_back(elem);
        heapify_up(size());
    }

    T top() const{
        return (*this)[0];
    }

    T extract(){
        T ret = top();
        (*this)[0] = (*this)[size() - 1];
        this->pop_back();
        heapify_down(1);
        return ret;
    }

    void show() const{
        vector<T> & data = (*this);
        for(auto &item : data){
            printf("%d ", item);
        }
        puts("");
    }

};

template<typename T, typename Compare>
Compare Heap<T, Compare>::cmp;

template<typename T, typename Compare = Less<T> >
class MinHeap : public Heap<T, Compare>{
public:
    MinHeap(const vector<T> & other): Heap<T, Compare>(other){
        Heap<T, Compare>::buildHeap();
    }

    void decrease(size_type index, const T& value){
        (*this)[index] -= value;
        Heap<T, Compare>::heapify_up(index + 1);
    }
};

template<typename T, typename Compare = Greater<T> >
class MaxHeap : public Heap<T, Compare>{
public:
    MaxHeap(const vector<T> & other): Heap<T, Compare>(other){
        Heap<T, Compare>::buildHeap();
    }
    void increase(size_type index, const T& value){
        (*this)[index] += value;
        Heap<T, Compare>::heapify_up(index + 1);
    }
};

template<typename T>
class Stack: protected vector<T>{
public:
    Stack(){}

    ~Stack(){}

    void push(const T& elem){
        this->vector<T>::push_back(elem);
    }

    T& back(){
        return this->vector<T>::back();
    }

    const T& back() const{
        return this->vector<T>::back();
    }

    void pop(){
        this->vector<T>::pop_back();
    }

    T extract(){
        T ret = this->top();
        pop();
        return ret;
    }

    bool empty() const{
        return this->vector<T>::empty();
    }

    size_type size() const{
        return this->vector<T>::size();
    }

};

#endif

