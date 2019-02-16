#ifndef __FY_ITERATOR_HPP__
#define __FY_ITERATOR_HPP__


template<typename T> class iterator;
template<typename T> class reverse_iterator;
template<typename T> using const_iterator = iterator<const T>;
template<typename T> using const_reverse_iterator = reverse_iterator<const T>;

template<typename T>
int operator -(const iterator<T>& lhs, const iterator<T>& rhs);
template<typename T>
bool operator <(const iterator<T>& lhs, const iterator<T>& rhs);
template<typename T>
bool operator >(const iterator<T>& lhs, const iterator<T>& rhs);
template<typename T>
bool operator <=(const iterator<T>& lhs, const iterator<T>& rhs);
template<typename T>
bool operator >=(const iterator<T>& lhs, const iterator<T>& rhs);
template<typename T>
bool operator ==(const iterator<T>& lhs, const iterator<T>& rhs);
template<typename T>
bool operator !=(const iterator<T>& lhs, const iterator<T>& rhs);

template<typename T>
iterator<T> operator +(const iterator<T>& lhs, int n);
template<typename T>
iterator<T> operator -(const iterator<T>& lhs, int n);
template<typename T>
iterator<T> operator +(int n, const iterator<T>& rhs);
template<typename T>
iterator<T> operator -(int n, const iterator<T>& rhs);

template<typename T>
int operator -(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs);
template<typename T>
bool operator <(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs);
template<typename T>
bool operator >(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs);
template<typename T>
bool operator <=(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs);
template<typename T>
bool operator >=(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs);
template<typename T>
bool operator ==(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs);
template<typename T>
bool operator !=(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs);

template<typename T>
reverse_iterator<T> operator +(const reverse_iterator<T>& lhs, int n);
template<typename T>
reverse_iterator<T> operator -(const reverse_iterator<T>& lhs, int n);
template<typename T>
reverse_iterator<T> operator +(int n, const reverse_iterator<T>& rhs);
template<typename T>
reverse_iterator<T> operator -(int n, const reverse_iterator<T>& rhs);


template<typename T>
class iterator{
protected:
    T* it;

public:

    typedef int difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::random_access_iterator_tag iterator_category;

    iterator() = delete;
    iterator(const iterator &other);
    iterator(T* const &other);
    ~iterator() = default;

    iterator operator ++();
    iterator operator ++(int);
    iterator operator --();
    iterator operator --(int);

    T& operator *();
    T* operator ->();

    iterator operator +=(int n);
    iterator operator -=(int n);

    operator const_iterator<T>() const;

    friend iterator operator + <T>(const iterator& lhs, int n);
    friend iterator operator - <T>(const iterator& lhs, int n);
    friend iterator operator + <T>(int n, const iterator& rhs);
    friend iterator operator - <T>(int n, const iterator& rhs);

    friend int operator - <T>(const iterator& lhs, const iterator& rhs);

    friend bool operator < <T>(const iterator& lhs, const iterator& rhs);
    friend bool operator > <T>(const iterator& lhs, const iterator& rhs);
    friend bool operator <= <T>(const iterator& lhs, const iterator& rhs);
    friend bool operator >= <T>(const iterator& lhs, const iterator& rhs);
    friend bool operator == <T>(const iterator& lhs, const iterator& rhs);
    friend bool operator != <T>(const iterator& lhs, const iterator& rhs);
};


template<typename T>
class reverse_iterator{
protected:
    T* it;

public:

    typedef int difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::random_access_iterator_tag iterator_category;

    reverse_iterator() = delete;
    reverse_iterator(const reverse_iterator &other);
    reverse_iterator(T* const &other);
    ~reverse_iterator() = default;

    reverse_iterator operator ++();
    reverse_iterator operator ++(int);
    reverse_iterator operator --();
    reverse_iterator operator --(int);

    T& operator *();
    T* operator ->();

    reverse_iterator operator +=(int n);
    reverse_iterator operator -=(int n);

    operator const_reverse_iterator<T>(void) const;

    friend reverse_iterator operator + <T>(const reverse_iterator& lhs, int n);
    friend reverse_iterator operator - <T>(const reverse_iterator& lhs, int n);
    friend reverse_iterator operator + <T>(int n, const reverse_iterator& rhs);
    friend reverse_iterator operator - <T>(int n, const reverse_iterator& rhs);

    friend int operator - <T>(const reverse_iterator& lhs, const reverse_iterator& rhs);

    friend bool operator < <T>(const reverse_iterator& lhs, const reverse_iterator& rhs);
    friend bool operator > <T>(const reverse_iterator& lhs, const reverse_iterator& rhs);
    friend bool operator <= <T>(const reverse_iterator& lhs, const reverse_iterator& rhs);
    friend bool operator >= <T>(const reverse_iterator& lhs, const reverse_iterator& rhs);
    friend bool operator == <T>(const reverse_iterator& lhs, const reverse_iterator& rhs);
    friend bool operator != <T>(const reverse_iterator& lhs, const reverse_iterator& rhs);
};


//iterator


template<typename T>
iterator<T>::iterator(T* const &other) :it(other){}

template<typename T>
iterator<T>::iterator(const iterator &other) :it(other.it){}

template<typename T>
iterator<T> iterator<T>::operator ++() {
    ++it;
    return *this;
}

template<typename T>
iterator<T> iterator<T>::operator ++(int) {
    iterator ret(*this);
    ++it;
    return ret;
}

template<typename T>
iterator<T> iterator<T>::operator --() {
    --it;
    return *this;
}

template<typename T>
iterator<T> iterator<T>::operator --(int) {
    iterator ret(*this);
    --it;
    return ret;
}

template<typename T>
T& iterator<T>::operator *(){
    return *it;
}

template<typename T>
T* iterator<T>::operator ->(){
    return it;
}


template<typename T>
iterator<T> iterator<T>::operator +=(int n){
    it += n;
    return *this;
}


template<typename T>
iterator<T> iterator<T>::operator -=(int n){
    it -= n;
    return *this;
}

template<typename T>
iterator<T>::operator const_iterator<T>() const{
    return const_iterator<T>(it);
}

template<typename T>
int operator -(const iterator<T>& lhs, const iterator<T>& rhs) {
    return lhs.it - rhs.it;
}

template<typename T>
bool operator <(const iterator<T>& lhs, const iterator<T>& rhs) {
    return lhs.it < rhs.it;
}

template<typename T>
bool operator >(const iterator<T>& lhs, const iterator<T>& rhs) {
    return lhs.it > rhs.it;
}

template<typename T>
bool operator <=(const iterator<T>& lhs, const iterator<T>& rhs) {
    return !(lhs > rhs);
}

template<typename T>
bool operator >=(const iterator<T>& lhs, const iterator<T>& rhs) {
    return !(lhs < rhs);
}

template<typename T>
bool operator ==(const iterator<T>& lhs, const iterator<T>& rhs) {
    return lhs.it == rhs.it;
}


template<typename T>
bool operator !=(const iterator<T>& lhs, const iterator<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T>
iterator<T> operator +(const iterator<T>& lhs, int n){
    iterator<T> ret(lhs.it + n);
    return ret;
}

template<typename T>
iterator<T> operator -(const iterator<T>& lhs, int n){
    iterator<T> ret(lhs.it - n);
    return ret;
}

template<typename T>
iterator<T> operator +(int n, const iterator<T>& rhs){
    return rhs + n;
}
template<typename T>
iterator<T> operator -(int n, const iterator<T>& rhs){
    return rhs - n;
}

//reverse_iterator


template<typename T>
reverse_iterator<T>::reverse_iterator(const reverse_iterator &other): it(other.it){}

template<typename T>
reverse_iterator<T>::reverse_iterator(T* const &other): it(other){}

template<typename T>
reverse_iterator<T> reverse_iterator<T>::operator ++(){
    --it;
    return *this;
}

template<typename T>
reverse_iterator<T> reverse_iterator<T>::operator ++(int) {
    reverse_iterator ret(*this);
    --it;
    return ret;
}

template<typename T>
reverse_iterator<T> reverse_iterator<T>::operator --() {
    ++it;
    return *this;
}

template<typename T>
reverse_iterator<T> reverse_iterator<T>::operator --(int) {
    reverse_iterator ret(*this);
    ++it;
    return ret;
}

template<typename T>
T& reverse_iterator<T>::operator *(){
    return *it;
}

template<typename T>
T* reverse_iterator<T>::operator ->(){
    return it;
}

template<typename T>
reverse_iterator<T> reverse_iterator<T>::operator +=(int n){
    it -= n;
    return *this;
}

template<typename T>
reverse_iterator<T> reverse_iterator<T>::operator -=(int n){
    it += n;
    return *this;
}

template<typename T>
reverse_iterator<T>::operator const_reverse_iterator<T>() const{
    return const_reverse_iterator<T>(it);
}

template<typename T>
int operator -(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs) {
    return rhs.it - lhs.it;
}


template<typename T>
bool operator <(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs) {
    return rhs.it < lhs.it;
}

template<typename T>
bool operator >(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs) {
    return lhs.it < rhs.it;
}

template<typename T>
bool operator <=(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs) {
    return !(lhs > rhs);
}

template<typename T>
bool operator >=(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs) {
    return !(lhs < rhs);
}

template<typename T>
bool operator ==(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs) {
    return lhs.it == rhs.it;
}

template<typename T>
bool operator !=(const reverse_iterator<T>& lhs, const reverse_iterator<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T>
reverse_iterator<T> operator +(const reverse_iterator<T>& lhs, int n){
    reverse_iterator<T> ret(lhs.it - n);
    return ret;
}

template<typename T>
reverse_iterator<T> operator -(const reverse_iterator<T>& lhs, int n){
    reverse_iterator<T> ret(lhs.it + n);
    return ret;
}

template<typename T>
reverse_iterator<T> operator +(int n, const reverse_iterator<T>& rhs){
    return rhs + n;
}
template<typename T>
reverse_iterator<T> operator -(int n, const reverse_iterator<T>& rhs){
    return rhs - n;
}











#endif
