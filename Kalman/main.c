#include <stdio.h>

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
}

/**
* \brief Main entry point
*/
void main()
{
    matrix_inverse();

    while (1) {};
}
