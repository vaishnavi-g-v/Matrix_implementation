#include <iostream>
#include <string>
#include "matrix.hpp"

#define NROWS1  3
#define NCLMS1  4
#define NROWS2  4

// using arrays to initialize the matrices
constexpr int a1[NROWS1][NCLMS1] =  {
                                        {1, 2, 3, 4},
                                        {4, 5, 6, 7},
                                        {8, 9, 10, 11}
                                    };

#define NROWS1_P  4
#define NCLMS1_P  2
constexpr int a1_p[NROWS1_P][NCLMS1_P] =    {
                                                {3, 2},
                                                {9, 21},
                                                {32, 11},
                                                {42, 14}
                                            };

// Keeping dimensions of a2 same as a1
constexpr int a2[NROWS1][NCLMS1] =  {
                                        {4, 9, 2, 3},
                                        {8, 5, 1, 6},
                                        {12, 19, 20, 21}
                                    };

// use this to verify correct answer for the addition of matrices corresponding to a1 and a2
constexpr int a1a2_add_res[NROWS1][NCLMS1] ={
                                                {5, 11, 5, 7},
                                                {12, 10, 7, 13},
                                                {20, 28, 30, 32}
                                            };

// verification
constexpr int a1_transpose[NCLMS1][NROWS1] ={
                                                {1, 4, 8},
                                                {2, 5, 9},
                                                {3, 6, 10},
                                                {4, 7, 11}
                                            };

// verification
constexpr int a2_minus_a1_res[NROWS1][NCLMS1] = {
                                                    {3, 7, -1, -1},
                                                    {4, 0, -5, -1},
                                                    {4, 10, 10, 10}
                                                };

#define NROWS3      2
#define NCLMS3      3

constexpr int a3[NROWS3][NCLMS3] =  {
                                        {4, 9, 2},
                                        {12, 19, 20}
                                    };

// Square matrix
#define NROWS_S1      3
#define NCLMS_S1      3
constexpr int as1[NROWS_S1][NCLMS_S1] = {
                                            {4, 9, 2},
                                            {8, 5, 1},
                                            {12, 19, 20}
                                        };

template<typename T>
bool check_eq (T val1, T val2)
{
    if (val1 == val2)
        return true;
    else
        return false;
}

#define CHECK_EQ(val1, val2) \
    if (!check_eq(val1, val2)) { \
        std::cout << "Value mismatch: val1 = " << val1 << ", val2 = " << val2 << std::endl; \
        exit(1); \
    }

template <typename T, int nclms>
matrix<T> init_matrix1 (matrix<T>& m, const T m1[][nclms], int nrows)
{
    matrix<int> mr {nrows, nclms};
    CHECK_EQ(m.rows(), nrows);
    CHECK_EQ(m.columns(), nclms);

    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < nclms; ++j) {
            m(i+1, j+1) = m1[i][j];
            CHECK_EQ(m(i+1, j+1), m1[i][j]);    // checking if () operator is working properly
            mr(i+1, j+1) = m(i+1, j+1);
        }
    }
    return mr;
}

void test_init()    // testing initialization
{
    std::cout << "Start test: Init" << std::endl;
    matrix<int> m {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m, a1, NROWS1);   // m and a1 are of same dimensions
    std::cout << "End test: Init PASS" << std::endl;
}

void test_move_init()
{
    std::cout << "Start test: Move initialization" << std::endl;
    matrix<int> m {NROWS1, NCLMS1};
    auto m1 = init_matrix1<int, NCLMS1>(m, a1, NROWS1);
    // not able to automatically invoke move operator - therefore explicility calling - compiler issue?
    matrix<int> mr = std::move(m1);

    // checking move
    for (int i = 1; i <= NROWS1; ++i) {
        for (int j = 1; j <= NCLMS1; ++j) {
            CHECK_EQ(mr(i, j), m(i, j));
        }
    }
    std::cout << "End test: Move initialization PASS" << std::endl;
}

void test_copy_init()
{
    std::cout << "Start test: Copy initialization" << std::endl;
    matrix<int> m {NROWS1, NCLMS1};
    auto m1 = init_matrix1<int, NCLMS1>(m, a1, NROWS1);
    matrix<int> mr = m1;    // copy initialization

    // checking copy
    for (int i = 1; i <= NROWS1; ++i) {
        for (int j = 1; j <= NCLMS1; ++j) {
            CHECK_EQ(mr(i, j), m(i, j));
        }
    }
    std::cout << "End test: Copy initialization PASS" << std::endl;
}

void test_copy_assign()
{
    std::cout << "Start test: Copy assignment" << std::endl;
    matrix<int> m {NROWS1, NCLMS1};
    auto m1 = init_matrix1<int, NCLMS1>(m, a1, NROWS1);

    // This should result in copy assignment
    matrix<int> m2;
    m2 = m;

    // checking
    for (int i = 1; i <= NROWS1; ++i) {
        for (int j = 1; j <= NCLMS1; ++j) {
            CHECK_EQ(m2(i, j), m(i, j));
        }
    }
    std::cout << "End test: Copy assignment PASS" << std::endl;
}

void test_move_assign()
{
    std::cout << "Start test: Move assignment" << std::endl;
    matrix<int> m {NROWS1, NCLMS1};
    auto m1 = init_matrix1<int, NCLMS1>(m, a1, NROWS1);

    // This should result in move assignment
    matrix<int> m2;
    m2 = std::move(m1);

    // checking
    for (int i = 1; i <= NROWS1; ++i) {
        for (int j = 1; j <= NCLMS1; ++j) {
            CHECK_EQ(m2(i, j), m(i, j));
        }
    }
    std::cout << "End test: Move assignment PASS" << std::endl;
}

void test_equal()
{
    std::cout << "Start test: Operator ==" << std::endl;
    matrix<int> m {NROWS1, NCLMS1};
    auto m1 = init_matrix1<int, NCLMS1>(m, a1, NROWS1);

    if (!check_eq(m, m1)) exit(1);
    std::cout << "End test: Operator == PASS" << std::endl;
}

void test_binary_add()
{
    std::cout << "Start test: Binary add operator m1 + m2" << std::endl;
    matrix<int> m1 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m1, a1, NROWS1);
    // m1.print();

    matrix<int> m2 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m2, a2, NROWS1);
    // m2.print();

    // Expected value after binary addition
    matrix<int> mexp {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(mexp, a1a2_add_res, NROWS1);
    // mexp.print();

    // m1 <= a1, m1 <= a2
    auto mr = m1 + m2;
    // mr.print();
    for (int i = 1; i <= NROWS1; ++i) {
        for (int j = 1; j <= NCLMS1; ++j) {
            CHECK_EQ(mr(i, j), mexp(i, j));
        }
    }
    std::cout << "End test: Binary add operator m1+m2 PASS" << std::endl;
}

void test_binary_selfadd()
{
    std::cout << "Start test: Binary self add operator m1 += m2" << std::endl;
    matrix<int> m1 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m1, a1, NROWS1);
    // m1.print();

    matrix<int> m2 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m2, a2, NROWS1);
    // m2.print();

    // Expected value after binary addition
    matrix<int> mexp {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(mexp, a1a2_add_res, NROWS1);
    // mexp.print();

    m1 += m2;
    // m1.print();
    for (int i = 1; i <= NROWS1; ++i) {
        for (int j = 1; j <= NCLMS1; ++j) {
            CHECK_EQ(m1(i, j), mexp(i, j));
        }
    }
    std::cout << "End test: Binary self add operator m1 += m2 PASS" << std::endl;
}

void test_binary_selfminus()
{
    std::cout << "Start test: Binary Self minus operator m1 -= m2" << std::endl;
    matrix<int> m1 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m1, a1, NROWS1);
    // m1.print();

    matrix<int> m2 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m2, a2, NROWS1);
    // m2.print();

    // Expected value after binary addition
    matrix<int> mexp {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(mexp, a2_minus_a1_res, NROWS1);
    // mexp.print();

    m2 -= m1;
    m2.print();
    for (int i = 1; i <= NROWS1; ++i) {
        for (int j = 1; j <= NCLMS1; ++j) {
            CHECK_EQ(m2(i, j), mexp(i, j));
        }
    }
    std::cout << "End test: Binary Self Minus operator m1 -= m2 PASS" << std::endl;
}

void test_binary_minus()
{
    std::cout << "Start test: Binary Minus operator m1 - m2" << std::endl;
    matrix<int> m1 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m1, a1, NROWS1);
    // m1.print();

    matrix<int> m2 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m2, a2, NROWS1);
    // m2.print();

    // Expected value after binary addition
    matrix<int> mexp {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(mexp, a2_minus_a1_res, NROWS1);
    // mexp.print();

    auto mr = m2 - m1;
    mr.print();
    for (int i = 1; i <= NROWS1; ++i) {
        for (int j = 1; j <= NCLMS1; ++j) {
            CHECK_EQ(mr(i, j), mexp(i, j));
        }
    }
    std::cout << "End test: Binary Minus operator m1 - m2 PASS" << std::endl;
}

void test_transpose()
{
    std::cout << "Start test: transpose" << std::endl;
    matrix<int> m1 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m1, a1, NROWS1);

    matrix<int> mexp {NCLMS1, NROWS1};
    init_matrix1<int, NROWS1>(mexp, a1_transpose, NCLMS1);

    auto mr = m1.transpose();
    // checking
    if (!check_eq(mr, mexp))
        exit(1);

    std::cout << "End test: transpose PASS" << std::endl;
}

void test_product()
{
    std::cout << "Start test: Product" << std::endl;
    matrix<int> m1 {NROWS1, NCLMS1};
    init_matrix1<int, NCLMS1>(m1, a1, NROWS1);

    matrix<int> m2 {NROWS1_P, NCLMS1_P};
    init_matrix1<int, NCLMS1_P>(m2, a1_p, NROWS1_P);

    auto mr = m1 * m2;
    mr.print();
    std::cout << "End test: Product PASS" << std::endl;
}

int main ()
{
    test_init();
    test_move_init();
    test_copy_init();
    test_copy_assign();
    test_move_assign();
    test_equal();
    test_binary_add();
    test_binary_selfadd();
    test_binary_selfminus();
    test_binary_minus();
    test_transpose();
    test_product();
}
