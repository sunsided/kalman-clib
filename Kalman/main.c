#include <stdio.h>
#include <assert.h>

#include "matrix.h"
#include "cholesky.h"

/**
* \brief Tests matrix inversion using Cholesky decomposition
*/
void matrix_inverse()
{
    int result;

    // data buffer for the original and decomposed matrix
    matrix_data_t d[3 * 3] = { 1, 0.5, 0,
        0.5, 1, 0,
        0, 0, 1 };

    // data buffer for the inverted matrix
    matrix_data_t di[3 * 3] = { 0, 0, 0,
        0, 0, 0,
        0, 0, 0 };

    // prepare matrix structures
    matrix_t m, mi;

    // initialize the matrices
    matrix_init(&m, 3, 3, d);
    matrix_init(&mi, 3, 3, di);

    // decompose matrix to lower triangular
    result = cholesky_decompose_lower(&m);

    // invert matrix using lower triangular
    matrix_invert_lower(&m, &mi);

    // test the result
    matrix_data_t test = matrix_get(&mi, 1, 1);
    assert(test >= 1.3);
}

/*!
* \brief Tests column and row fetching
*/
void matrix_copy_cols_and_rows()
{

    // data buffer for the original and decomposed matrix
    matrix_data_t d[3 * 4] = { 1, 2, 3, 10,
        4, 5, 6, 11,
        7, 8, 9, 12 };

    // data buffer for the copied row
    matrix_data_t v[4] = { 0, 0, 0 };
    matrix_data_t *vp;

    // prepare matrix structure
    matrix_t m;

    // initialize the matrix
    matrix_init(&m, 3, 4, d);

    // fetch the column
    matrix_get_column_copy(&m, 2, v);
    assert(v[0] == 3);
    assert(v[1] == 6);
    assert(v[2] == 9);

    // fetch the row
    matrix_get_row_copy(&m, 2, v);
    assert(v[0] == 7);
    assert(v[1] == 8);
    assert(v[2] == 9);
    assert(v[3] == 12);

    // fetch the row
    matrix_get_row_pointer(&m, 2, &vp);
    assert(vp[0] == 7);
    assert(vp[1] == 8);
    assert(vp[2] == 9);
    assert(vp[3] == 12);
}

/*!
*  \brief Tests matrix multiplication
*/
void matrix_multiply_aux()
{
    matrix_data_t ad[3 * 3] = { 1, 0.5, 0,
        0.5, 1, 0,
        0, 0, 1 };

    matrix_data_t bd[3 * 4] = { 1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12};

    matrix_data_t cd[3 * 4] = { 0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0 };

    matrix_data_t aux[3] = { 0, 0, 0 };

    // prepare matrix structures
    matrix_t a, b, c;

    // initialize the matrices
    matrix_init(&a, 3, 3, ad);
    matrix_init(&b, 3, 4, bd);
    matrix_init(&c, 3, 4, cd);

    // multiply
    matrix_mult(&a, &b, &c, aux);
    assert(cd[0] == 3.5);
    assert(cd[1] == 5);
    assert(cd[4] == 5.5);
    assert(cd[11] == 12);
}

/*!
*  \brief Tests matrix multiplication with transposed B
*/
void matrix_multiply_transb()
{
    matrix_data_t ad[3 * 3] = { 1, 0.5, 0,
        0.5, 1, 0,
        0, 0, 1 };

    matrix_data_t bd[3 * 3] = { 1, 2, 3,
        5, 6, 7,
        9, 10, 11 };

    matrix_data_t cd[3 * 3] = { 0, 0, 0,
        0, 0, 0,
        0, 0, 0 };
    
    // prepare matrix structures
    matrix_t a, b, c;

    // initialize the matrices
    matrix_init(&a, 3, 3, ad);
    matrix_init(&b, 3, 3, bd);
    matrix_init(&c, 3, 3, cd);

    // multiply
    matrix_mult_transb(&a, &b, &c);
    assert(cd[0] == 2);
    assert(cd[1] == 8);
    assert(cd[4] == 8.5);
    assert(cd[8] == 11);
}

/**
* \brief Main entry point
*/
void main()
{
    matrix_inverse();
    matrix_copy_cols_and_rows();
    matrix_multiply_aux();
    matrix_multiply_transb();
}
