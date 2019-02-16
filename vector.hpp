#ifndef __FY_VECTOR_HPP__
#define __FY_VECTOR_HPP__

#include <cstddef>
#include <memory>
#include "c:\code\project\iterator\iterator.hpp"

template<typename T> class vector;

template<typename T>
bool operator <(const vector<T>& lhs, const vector<T>& rhs);
template<typename T>
bool operator >(const vector<T>& lhs, const vector<T>& rhs);
template<typename T>
bool operator <=(const vector<T>& lhs, const vector<T>& rhs);
template<typename T>
bool operator >=(const vector<T>& lhs, const vector<T>& rhs);
template<typename T>
bool operator ==(const vector<T>& lhs, const vector<T>& rhs);
template<typename T>
bool operator !=(const vector<T>& lhs, const vector<T>& rhs);
template<typename T>
void swap(vector<T>& lhs, vector<T>& rhs);


template<typename T>
class vector{
private:
    void release();
    static void moveElem(T* from, T* to, size_t n);

protected:
    size_t volume;
    size_t count;
    T* array;

public:
    vector();
    vector(const vector& other);
    vector(vector&& other) noexcept;
    vector(size_t n);
    vector(size_t n, const T& value);
    vector(std::initializer_list<T> lst);

    ~vector();

    size_t size() const;
    size_t capacity() const;
    bool empty() const;

    void assign(size_t n, const T& value);
    void push_back(const T& elem);
    void pop_back();
    template<typename... Args>
    iterator<T> emplace(const_iterator<T> pos, Args... args);
    template<typename... Args>
    void emplace_back(Args... args);

    iterator<T> insert(const_iterator<T> pos, const T& value);
    iterator<T> insert(const_iterator<T> pos, size_t count, const T& value);
    iterator<T> erase(const_iterator<T> pos);
    iterator<T> erase(const_iterator<T> first, const_iterator<T> last);

    void clear();
    void reserve(size_t n);
    void shrink_to_fit();

    iterator<T> begin();
    iterator<T> end();
    const_iterator<T> cbegin() const;
    const_iterator<T> cend() const;

    reverse_iterator<T> rbegin();
    reverse_iterator<T> rend();
    const_reverse_iterator<T> crbegin() const;
    const_reverse_iterator<T> crend() const;

    void resize(size_t sz);
    void resize(size_t sz, const T& value);

    T& operator [](size_t index);
    const T& operator [](size_t index) const;

    T& at(size_t index);
    const T& at(size_t index) const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    T* data();
    const T* data() const;

    vector<T>& operator =(const vector& rhs);
    vector<T>& operator =(vector&& rhs) noexcept;

    friend void swap <T>(vector<T>& lhs, vector<T>& rhs);
    friend bool operator < <T>(const vector<T>& lhs, const vector<T>& rhs);
    friend bool operator > <T>(const vector<T>& lhs, const vector<T>& rhs);
    friend bool operator <= <T>(const vector<T>& lhs, const vector<T>& rhs);
    friend bool operator >= <T>(const vector<T>& lhs, const vector<T>& rhs);
    friend bool operator == <T>(const vector<T>& lhs, const vector<T>& rhs);
    friend bool operator != <T>(const vector<T>& lhs, const vector<T>& rhs);

};



template<typename T>
vector<T>::vector(): volume(0), count(0), array(nullptr){}


template<typename T>
vector<T>::vector(const vector& other):
    volume(other.volume), count(other.count), array( (T*) new char[volume * sizeof(T)])
{
    for(size_t i = 0; i < count; i++){
        new (&array[i]) T(other[i]);
    }
}

template<typename T>
vector<T>::vector(vector&& other) noexcept:
    volume(other.volume), count(other.count), array(other.array)
{
    other.array = nullptr;
}

template<typename T>
vector<T>::vector(size_t n): vector(n, T()){}

template<typename T>
vector<T>::vector(size_t n, const T& value):
    volume(n), count(n), array( (T*) new char[n * sizeof(T)])
{
    for(size_t i = 0; i < count; i++){
        new (&array[i]) T(value);
    }
}

template<typename T>
vector<T>::vector(std::initializer_list<T> lst)
    :volume(lst.size()), count(0), array((T*) new char[lst.size() * sizeof(T)])
{
    for(auto& item : lst){
        new (array + count++) T(item);
    }
}

template<typename T>
vector<T>::~vector(){
    release();
    delete [] (char*) array;
}

template<typename T>
void vector<T>::release(){
    for(size_t i = count; i; array[--i].T::~T());
}

template<typename T>
void vector<T>::moveElem(T* to, T* from, size_t n){
    for(size_t i = 0; i < n; i++){
        new (to + i) T(std::move(*(from + i)));
    }
}

template<typename T>
void vector<T>::clear(){
    release();
    count = 0;
}

template<typename T>
size_t vector<T>::size() const{
    return count;
}

template<typename T>
size_t vector<T>::capacity() const{
    return volume;
}

template<typename T>
bool vector<T>::empty() const{
    return size() == 0;
}

template<typename T>
void vector<T>::assign(size_t n, const T& value){
    clear();
    if(volume < n){
        volume = n;
        delete [] (char*) array;
        array = (T*) new char[volume * sizeof(T)];
    }
    while(count < n) new (&array[count++]) T(value);
}

template<typename T>
iterator<T> vector<T>::insert(const_iterator<T> pos, size_t n, const T& value){
    size_t len = pos - cbegin();
    if(count + n > volume){
        volume = volume ? 2 * volume : 1;
        while(volume < count + n) volume *= 2;
        T* tmp( (T*) new char[volume * sizeof(T)] );
        //memcpy(tmp, array, len * sizeof(T));
        moveElem(tmp, array, len);
        for(size_t i = 0; i < n; i++) new (&tmp[len + i]) T(value);
        //memcpy(tmp + len + n, array + len, (count - len) * sizeof(T));
        moveElem(tmp + len + n, array + len, (count - len));
        delete [] (char*) array;
        array = tmp;
    }
    else{
        //memmove(array + len + n, array + len, (count - len) * sizeof(T));
        moveElem(array + len + n, array + len, (count - len));
        for(size_t i = 0; i < n; i++) new (&array[len + i]) T(value);
    }
    count += n;
    return begin() + len;
}

template<typename T>
iterator<T> vector<T>::insert(const_iterator<T> pos, const T& value){
    return insert(pos, 1, value);
}

template<typename T>
iterator<T> vector<T>::erase(const_iterator<T> pos){
    size_t index = pos - cbegin();
    (array + index)->T::~T();
    //memmove(array + index, array + index + 1, (count - index - 1) * sizeof(T));
    moveElem(array + index, array + index + 1, (count - index - 1));
    count--;
    return begin() + index;
}

template<typename T>
iterator<T> vector<T>::erase(const_iterator<T> first, const_iterator<T> last){
    size_t len = last - first;
    size_t index = first - cbegin();
    for(T* it = begin() + index; it != last; it++) it->T::~T();
    //memmove(array + index, array + index + len, (count - index - len) * sizeof(T));
    moveElem(array + index, array + index + len, (count - index - len));
    count -= len;
    return begin() + index;
}


template<typename T>
void vector<T>::resize(size_t sz){
    if(sz > volume){
        T* tmp( (T*) new char[sz * sizeof(T)] );
        //memcpy(tmp, array, count * sizeof(T));
        moveElem(tmp, array, count);
        for(size_t i = count; i < sz; i++) new (&tmp[i]) T();
        delete [] (char*) array;
        volume = sz;
        count = sz;
        array = tmp;
    }
    else{
        size_t i = count;
        for(; i < sz; i++) new (&array[i]) T();
        while(i - sz) array[--i].T::~T();
        count = sz;
    }
}

template<typename T>
void vector<T>::resize(size_t sz, const T& value){
    if(sz > volume){
        T* tmp( (T*) new char[sz * sizeof(T)] );
        //memcpy(tmp, array, count * sizeof(T));
        moveElem(tmp, array, count);
        for(size_t i = count; i < sz; i++) new (&tmp[i]) T(value);
        delete [] (char*) array;
        volume = sz;
        count = sz;
        array = tmp;
    }
    else{
        size_t i = count;
        for(; i < sz; i++) new (&array[i]) T(value);
        while(i - sz) array[--i].T::~T();
        count = sz;
    }
}

template<typename T>
void vector<T>::reserve(size_t n){
    if(volume < n){
        T* tmp( (T*) new char[n * sizeof(T)] );
        //memcpy(tmp, array, count * sizeof(T));
        moveElem(tmp, array, count);
        delete [] (char*) array;
        array = tmp;
        volume = n;
    }
}

template<typename T>
void vector<T>::shrink_to_fit(){
    if(volume > count){
        T* tmp( (T*) new char[count * sizeof(T)] );
        //memcpy(tmp, array, count * sizeof(T));
        moveElem(tmp, array, count);
        delete [] (char*) array;
        array = tmp;
        volume = count;
    }
}

template<typename T>
void vector<T>::push_back(const T& elem){
    if(count == volume){
        volume = volume ? 2 * volume : 1;
        T* tmp( (T*) new char[volume * sizeof(T)] );
        //memcpy(tmp, array, count * sizeof(T));
        moveElem(tmp, array, count);
        delete [] (char*) array;
        array = tmp;
    }
    new (&array[count++]) T(elem);
}

template<typename T>
void vector<T>::pop_back(){
    array[--count].T::~T();
}

template<typename T>
template<typename... Args>
iterator<T> vector<T>::emplace(const_iterator<T> pos, Args... args){
    return insert(pos, T(std::forward<Args>(args)...));
}

template<typename T>
template<typename... Args>
void vector<T>::emplace_back(Args... args){
    emplace(cend(), std::forward<Args>(args)...);
}




template<typename T>
iterator<T> vector<T>::begin(){
    return iterator<T>(array);
}

template<typename T>
iterator<T> vector<T>::end(){
    return begin() + count;
}

template<typename T>
const_iterator<T> vector<T>::cbegin() const{
    return const_iterator<T>(array);
}

template<typename T>
const_iterator<T> vector<T>::cend() const{
    return cbegin() + count;
}

template<typename T>
reverse_iterator<T> vector<T>::rbegin(){
    return reverse_iterator<T>(array + count - 1);
}

template<typename T>
reverse_iterator<T> vector<T>::rend(){
    return rbegin() + count;
}

template<typename T>
const_reverse_iterator<T> vector<T>::crbegin() const{
    return const_reverse_iterator<T>(array + count - 1);
}

template<typename T>
const_reverse_iterator<T> vector<T>::crend() const{
    return crbegin() + count;
}



template<typename T>
T& vector<T>::operator [](size_t index){
    return array[index];
}

template<typename T>
const T& vector<T>::operator [](size_t index) const{
    return array[index];
}


template<typename T>
T& vector<T>::at(size_t index){
    if(index < 0 || index >= count) throw("out of range");
    return (*this)[index];
}

template<typename T>
const T& vector<T>::at(size_t index) const{
    if(index < 0 || index >= count) throw("out of range");
    return (*this)[index];
}

template<typename T>
T& vector<T>::front(){
    return array[0];
}

template<typename T>
const T& vector<T>::front() const{
    return array[0];
}

template<typename T>
T& vector<T>::back(){
    return array[count - 1];
}

template<typename T>
const T& vector<T>::back() const{
    return array[count - 1];
}

template<typename T>
T* vector<T>::data(){
    return array;
}
template<typename T>
const T* vector<T>::data() const{
    return array;
}


template<typename T>
vector<T>& vector<T>::operator =(const vector& rhs){
    if(this == &rhs) return *this;
    release();
    if(volume < rhs.volume){
        delete [] (char*) array;
        volume = rhs.volume;
        array = (T*) new char[volume * sizeof(T)];
    }
    count = rhs.count;
    for(size_t i = 0; i < count; i++){
        new (&array[i]) T(rhs[i]);
    }
    return *this;
}

template<typename T>
vector<T>& vector<T>::operator =(vector&& rhs) noexcept{
    if(this != &rhs){
        release();
        delete [] (char*) this->array;
        this->volume = rhs.volume;
        this->count = rhs.count;
        this->array = rhs.array;
        rhs.array = nullptr;
    }
    return *this;
}


template<typename T>
void swap(vector<T>& lhs, vector<T>& rhs){
    T* tmp1 = lhs.array;
    lhs.array = rhs.array;
    rhs.array = tmp1;

    size_t tmp2 = lhs.count;
    lhs.count = rhs.count;
    rhs.count = tmp2;

    tmp2 = lhs.volume;
    lhs.volume = rhs.volume;
    rhs.volume = tmp2;
}


template<typename T>
bool operator <(const vector<T>& lhs, const vector<T>& rhs)
{
    for(size_t i = 0; i < lhs.size(); i++){
        if(lhs[i] < rhs[i]) return true;
        if(rhs[i] < lhs[i]) return false;
    }
    if(lhs.size() < rhs.size()) return true;
    return false;
}

template<typename T>
bool operator >(const vector<T>& lhs, const vector<T>& rhs)
{
    return rhs < lhs;
}

template<typename T>
bool operator ==(const vector<T>& lhs, const vector<T>& rhs)
{
    if(lhs.size() != rhs.size()) return false;
    for(size_t i = 0; i < lhs.size(); i++){
        if(lhs[i] != rhs[i]) return false;
    }
    return true;
}

template<typename T>
bool operator <=(const vector<T>& lhs, const vector<T>& rhs)
{
    return !(lhs > rhs);
}

template<typename T>
bool operator >=(const vector<T>& lhs, const vector<T>& rhs)
{
    return !(lhs < rhs);
}

template<typename T>
bool operator !=(const vector<T>& lhs, const vector<T>& rhs)
{
    return !(lhs == rhs);
}


#endif


