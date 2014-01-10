#include <stdio.h>

#include "matrix.h"
#include "cholesky.h"

void main()
{
    int result;

    // data buffer for the original and decomposed matrix
    matrix_data_t d[3*3] = {1, 0.5, 0, 
                            0.5, 1, 0, 
                            0, 0, 1};

    // data buffer for the inverted matrix
    matrix_data_t di[3*3] = {0, 0, 0, 
                            0, 0, 0, 
                            0, 0, 0};

    // prepare matrix structures
    matrix_t m, im;

    // original and decomposed matrix
    m.cols = 3;
    m.rows = 3;
    m.data = d;

    // inverted matrix
    im.cols = 3;
    im.rows = 3;
    im.data = di;

    // decompose matrix to lower triangular
    result = cholesky_decompose_lower(&m);

    // invert matrix using lower triangular
    matrix_invert_lower(&m, &im);

    while (1) {};
}