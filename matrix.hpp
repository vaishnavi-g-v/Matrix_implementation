#include <iostream>
#include <stdexcept>
#include <cstring>
#include <concepts>
#include "Vector.hpp"

template <typename T> 
requires std::integral<T> || std::floating_point<T>
class matrix {
    private:
        int nrows;
        int nclms;
        // flattening a 2D matrix to a 1D vector
        Vector<T> elems;

    public:
        // default constructor
        matrix (int nrows = 10, int nclms = 10);
        ~matrix ();

        // copy and move constructors/initialization
        matrix (const matrix<T>& a);
        matrix (matrix<T>&& a);

        // copy and move assignment
        matrix<T>& operator =(const matrix<T>& a);
        matrix<T>& operator =(matrix<T>&& a);

        // printing with m,n value
        void print ();

        int rows() const {return nrows;};
        int columns() const {return nclms;};

        matrix<T> transpose () const;
        // 1 <= row <= nrows and 1 <= clm <= nclms;
        T& operator () (int row, int clm);
        const T& operator () (int row, int clm) const;

        // Arithmatic operations
        matrix<T> operator +() const; // Unary +
        matrix<T> operator +(matrix<T> a) const; // Binary +
        matrix<T>& operator +=(const matrix<T>& a);
        matrix<T> operator -(); // Unary -
        matrix<T> operator -(matrix<T> a) const; // Binary -
        matrix<T>& operator -=(const matrix<T>& a);
        matrix<T> operator *(const matrix<T>& a) const; // matrix multiplication

        bool operator ==(const matrix<T>& a) const;
        bool operator !=(const matrix<T>& a) const {return !(*this == a);};

};

template <typename T>
matrix<T>::matrix (int nrows, int nclms)
{
    if (nrows < 0 || nclms < 0) {
        throw std::invalid_argument ("number of rows and columns must be non-negative value");
    }

    if ( ((nrows == 0) || (nclms == 0)) && ((nrows != 0) || (nclms != 0)) ) {
        throw std::invalid_argument ("Either both rows and columns should be zero OR non-zero");
    }

    // int size;

    // if(nrows == 0) size = nclms;
    // else if(nclms == 0) size = nrows;
    // else size = nclms*nrows;

    // elems = Vector<T>{size};
    elems.resize(nrows * nclms);
    this->nrows = nrows;
    this->nclms = nclms;
}

template <typename T>
matrix<T>::~matrix ()
{
    nrows = nclms = 0;
}

// Copy constructor
template <typename T>
matrix<T>::matrix (const matrix<T>& a)
{
    std::cout << "Enter Copy constructor\n";
    elems.resize(a.nrows * a.nclms);
    elems = a.elems;
    nrows = a.nrows;
    nclms = a.nclms;
}

// Move constructors
template <typename T>
matrix<T>::matrix (matrix<T>&& a)
{
    std::cout << "Enter move constructor\n";
    nrows = a.nrows;
    nclms = a.nclms;
    elems = std::move(a.elems);
    a.nrows = a.nclms = 0;
}

// Copy assignment
template <typename T>
matrix<T>& matrix<T>::operator = (const matrix<T>& a)
{
    std::cout << "Enter Copy assignment\n";
    elems = a.elems;
    nrows = a.nrows;
    nclms = a.nclms;
    return *this;
}

// Move assignment
template <typename T>
matrix<T>& matrix<T>::operator = (matrix<T>&& a)
{
    std::cout << "Enter move assignment\n";

    nrows = a.nrows;
    nclms = a.nclms;
    elems = std::move(a.elems);
    a.nrows = a.nclms = 0;
    return *this;
}

template <typename T>
void matrix<T>::print ()
{
    for (int i = 1; i <= nrows; ++i) {
        for (int j = 1; j <= nclms; ++j) {
            std::cout << this->operator()(i,j) << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template <typename T>
matrix<T> matrix<T>::transpose () const
{
    // writing result into new matrix
    matrix<T> mt {nclms, nrows};

    for (int i = 1; i <= nrows; ++i) {
        for (int j = 1; j <= nclms; ++j) {
            mt(j, i) = (*this)(i, j);
        }
    }
    return mt;
}

template <typename T>
T& matrix<T>::operator () (int row, int clm)
{
    // accessing row and then column within the 1D vector
    int idx = (row - 1) * nclms + (clm - 1);
    return this->elems[idx];
}

template <typename T>
const T& matrix<T>::operator () (int row, int clm) const
{
    int idx = (row - 1) * nclms + (clm - 1);
    return this->elems[idx];
}

template <typename T>
matrix<T> matrix<T>::operator +() const
{
    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator +(matrix<T> a) const      
{
    // a is not a reference -- passed by values, modifying doesn't affect the initial value
    a += *this;
    return a;
}

template <typename T>
matrix<T>& matrix<T>::operator +=(const matrix<T>& a)
{
    if ( (nrows != a.nrows) || (nclms != a.nclms)) {
        throw std::invalid_argument ("number of rows and/or columns are not the same");
    }

    int nitems = nrows * nclms;
    for (int i = 0; i < nitems; ++i)
        elems[i] += a.elems[i];

    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator -()   // negates all values in matrix
{
    int nitems = nrows * nclms;
    for (int i = 0; i < nitems; ++i)
        elems[i] = -elems[i];    
    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator -(matrix<T> a) const
{
    auto mr = *this;
    mr -= a;
    return mr;
}

template <typename T>
matrix<T>& matrix<T>::operator -=(const matrix<T>& a)
{
    if ( (nrows != a.nrows) || (nclms != a.nclms)) {
        throw std::invalid_argument ("number of rows and/or columns are not the same");
    }

    int nitems = nrows * nclms;
    for (int i = 0; i < nitems; ++i)
        elems[i] -= a.elems[i];

    return *this;
}

template <typename T>
matrix<T> matrix<T>::operator *(const matrix<T>& a) const
{
    matrix<T> mr {nrows, a.nclms};

    if (nclms != a.nrows) {
        throw std::invalid_argument ("number of rows/columns mismatch");
    }
    // basic matrix multiplication
    for (int i = 1; i <= nrows; ++i) {
        for (int j = 1; j <= a.nclms; ++j) {
            int val = 0;
            for (int k = 1; k <= nclms; ++k) {
                val += (*this)(i, k) * a(k, j);
            }
            mr(i, j) = val;
        }
    }

    return mr;
}

template <typename T>
bool matrix<T>::operator == (const matrix<T>& a) const
{
    if ((nrows != a.nrows) || (nclms != a.nclms))
        return false;

    for (int i = 1; i <= nrows; ++i) {
        for (int j = 1; j <= nclms; ++j) {
            if ((*this)(i, j) != a(i, j))
                return false;
        }
    }
    return true;
}

