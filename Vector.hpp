#pragma once
#include <iostream>
#include <algorithm>

template <typename T>
class linearList {
    public:
        virtual bool empty() const = 0;
        virtual int size() const = 0;
        virtual T& at(int atindx) const = 0;
        virtual void erase(int atindx) = 0;
        virtual void insert(int atindx, const T& elem) = 0;
        virtual void output(std::ostream& out) const = 0;
        virtual ~linearList() {};
};

template <typename T>
class Vector : public linearList<T> {
#define VEC_INIT_CAPACITY           10
#define VEC_CAPACITY_ADD_FACTOR     2
    
  public:
    Vector (int init_capacity = VEC_INIT_CAPACITY, double capacity_add_factor = VEC_CAPACITY_ADD_FACTOR);
    Vector (std::initializer_list<T> init_lst);

    // Copy constructor
    Vector (const Vector<T>&);

    // Copy assignment
    Vector<T>& operator =(const Vector<T>&);

    // Move constructor
    Vector (Vector<T>&&);


    // Move assignment
    Vector<T>& operator = (Vector<T>&&);

    ~Vector ();

    bool empty () const {return lstsize == 0;};
    int size () const {return lstsize;};
    int capacity() const {return arsize;};
    T& at (int atindx) const;
    void insert (int atindx, const T& elem);
    void push_back(const T& elem);
    void pop_back();
    void erase (int atindx);
    void output (std::ostream& out) const;
    T* begin() {return elems;};
    T* end() {return elems+lstsize;};
    // == operator needs to be added

    void    resize(int sz);
    // Make the list empty
    void clear ();

    T& operator[] (int index);
    const T& operator[] (int index) const;

  private:
    void checkListSizeThreshold ();
    void checkindex(int index) const;
    T* elems;
    int arsize;
    int init_arsize;
    int lstsize;
    double arl_capacity_add_factor;
};

template <typename T>
static void change_len_1d (T*& elems, int& len, int nlen) {
    if (len == nlen)
        return;

    T* nelems;
    nelems = new T[nlen];
    std::copy(elems, elems+std::min(len, nlen), nelems);
    delete [] elems;
    elems = nelems;
    len = nlen;
}

template <typename T>
void Vector<T>::checkListSizeThreshold () {
    if (lstsize < arsize/4) {
        int nlen = std::max(arsize/2, init_arsize);
        change_len_1d(elems, arsize, nlen);
    }
}

template<typename T>
Vector<T>::Vector (int init_capacity, double capacity_add_factor) {
    if (init_capacity < 1) {
        throw std::length_error {"Invalid array length"};
    }

    elems = new T[init_capacity];
    init_arsize = arsize = init_capacity;
    arl_capacity_add_factor = capacity_add_factor;
    lstsize = 0;
}

template <typename T>
Vector<T>::Vector (std::initializer_list<T> init_lst): Vector(static_cast<int>(init_lst.size()) * 2) {
    std::copy(init_lst.begin(), init_lst.end(), elems);
    lstsize = static_cast<int>(init_lst.size());
}

// Copy constructor
template <typename T>
Vector<T>::Vector (const Vector<T>& lst) {
    elems = new T[lst.arsize];
    arsize = lst.arsize;
    lstsize = lst.lstsize;
    std::copy(lst.elems, lst.elems+lst.lstsize, elems);
}

// Copy assignment
template <typename T>
Vector<T>& Vector<T>::operator = (const Vector<T>& lst) {
    auto p = new T[lst.arsize];
    std::copy(lst.elems, lst.elems+lst.lstsize, p);
    delete [] elems; // Delete old elements
    elems = p;
    arsize = lst.arsize;
    lstsize = lst.lstsize;
    return *this;
}

// Move constructor
template <typename T>
Vector<T>::Vector (Vector<T>&& lst) {
    elems = lst.elems;
    arsize = lst.arsize;
    lstsize = lst.lstsize;
    
    // Clear the rvalue
    lst.elems = nullptr;
    lst.arsize = 0;
    lst.lstsize = 0;
}

// Move assignment
template <typename T>
Vector<T>& Vector<T>::operator =(Vector<T>&& lst) {
    delete [] elems;

    elems = lst.elems;
    arsize = lst.arsize;
    lstsize = lst.lstsize;

    // Clear the rvalue
    lst.elems = nullptr;
    lst.arsize = 0;
    lst.lstsize = 0;
    return *this;
}
 
template <typename T>
Vector<T>::~Vector () {
    delete [] elems;
}

template <typename T>
T& Vector<T>::at (int atindx) const {
    checkindex(atindx);
    return elems[atindx];
}

template <typename T>
void Vector<T>::insert (int atindx, const T& elem) {
    if (arsize == lstsize) {
        change_len_1d(elems, arsize, arsize * arl_capacity_add_factor);
    }
    if (atindx != lstsize) {
        checkindex(atindx);
        std::copy_backward(elems+atindx, elems+lstsize, elems+lstsize+1);
    }
    elems[atindx] = elem;
    ++lstsize;
}

template <typename T>
void Vector<T>::push_back(const T& elem) {
    if (arsize == lstsize) {
        change_len_1d(elems, arsize, arsize * arl_capacity_add_factor);
    }
    elems[lstsize] = elem;
    ++lstsize;
}

// Just erase the element at the right end of the list
// Do not return any value. If you need it, use/implement
// a separate method, say top, to get that element before pop.
template <typename T>
void Vector<T>::pop_back() {
    if (lstsize == 0)
        return;
    elems[lstsize-1] = T {};
    --lstsize;
    checkListSizeThreshold();
}

template <typename T>
void Vector<T>::erase (int atindx) {
    checkindex(atindx);
    std::copy(elems+atindx+1, elems+lstsize, elems+atindx);
    --lstsize;
    checkListSizeThreshold();
}

template <typename T>
void Vector<T>::output (std::ostream& out) const {
    for (int i = 0; i < lstsize; i++) {
        out << elems[i] << " ; ";
    }
    out << std::endl;
}

template <typename T>
void Vector<T>::resize(int sz)
{
    // If the original list size (lstsize) is less than the new one,
    // NULL elements (ie., default elements) are automatically added.
    // Nothing specific to do in this case
    if (sz > arsize) {
        int nlen = arsize;
        while (sz > nlen) {
            nlen *= arl_capacity_add_factor;
        }
        change_len_1d(elems, arsize, nlen);
    }

    // If the original size is more than the new one, extra elements
    // are removed. Just setting the elements to default constructor
    // will do the deed.
    if (sz < lstsize) {
        for (int i = sz; i < lstsize; i++) {
            // of course, copy assignment of type T should be supported...duh!!!
            elems[i] = T {};
        }
    }

    lstsize = sz;
}

template <typename T>
void Vector<T>::clear () {

    // for (int i = 0; i < lstsize; i++) { 
    //     elems[i] = T {}; 
    // } 
    // lstsize = 0; 

    this->resize(0);
}

template <typename T>
T& Vector<T>::operator[] (int index) {
    checkindex(index);
    return elems[index];
}

template <typename T>
const T& Vector<T>::operator[] (int index) const {
    checkindex(index);
    return elems[index];
}

template <typename T>
void Vector<T>::checkindex (int index) const {
    if (index < 0 || index >= lstsize)
        throw std::length_error("Invalid index");
}

template<typename T>
std::ostream& operator<<(std::ostream& out, Vector<T> ar) {
    out << "array_list[" << ar.size() << "/" << ar.capacity() << "]: ";
    ar.output(out);
    return out;
}


