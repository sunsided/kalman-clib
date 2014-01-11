#include <stdint.h>
#include <assert.h>

#define EXTERN_INLINE_MATRIX INLINE
#include "matrix.h"

/**
* \brief Initializes a matrix structure.
* \param[in] mat The matrix to initialize
* \param[in] rows The number of rows
* \param[in] cols The number of columns
* \param[in] buffer The data buffer (of size {\see rows} x {\see cols}).
*/
void matrix_init(matrix_t *mat, const uint_fast8_t rows, const uint_fast8_t cols, matrix_data_t *buffer)
{
    *(uint_fast8_t*)&mat->cols = cols;
    *(uint_fast8_t*)&mat->rows = rows;
    *(matrix_data_t**)&mat->data = buffer;
}

/*!
* \brief Gets a copy of a matrix column
* \param[in] mat The matrix to initialize
* \param[in] rows The column
* \param[in] row_data Pointer to an array of the correct length to hold a column of matrix {\ref mat}.
*/
void matrix_get_column_copy(matrix_t *mat, const register uint_fast8_t column, matrix_data_t *row_data)
{
    // start from the back, so target index is equal to the index of the last row.
    register uint_fast8_t target_index = mat->rows - 1;

    // also, the source index is the column..th index
    register int_fast16_t stride = mat->cols;
    register int_fast16_t source_index = target_index * stride + column;

    // fetch data
    do
    {
        row_data[target_index] = mat->data[source_index];

        --target_index;
        source_index -= stride;
    }
    while (source_index >= 0);
}

/*!
* \brief Gets a copy of a matrix row
* \param[in] mat The matrix to initialize
* \param[in] rows The row
* \param[in] row_data Pointer to an array of the correct length to hold a row of matrix {\ref mat}.
*/
void matrix_get_row_copy(matrix_t *mat, const register uint_fast8_t row, matrix_data_t *row_data)
{
    register uint_fast8_t target_index = mat->cols;
    register int_fast16_t source_index = (row+1) * mat->cols;

    // fetch data
    do
    {
        --target_index;
        --source_index;
        row_data[target_index] = mat->data[source_index];
    } while (target_index != 0);
}

/**
* \brief Inverts a lower triangular matrix.
* \param[in] lower The lower triangular matrix to be inverted.
* \param[out] inverse The inverse of the lower triangular matrix.
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_invert_lower(const matrix_t *RESTRICT const lower, matrix_t *RESTRICT inverse)
{
    int_fast8_t i, j, k;
    const uint_fast8_t n = lower->rows;
    const matrix_data_t *const  t = lower->data;
    matrix_data_t *a = inverse->data;

    // TODO reorder these operations to avoid cache misses
    
    // inverts the lower triangular system and saves the result
    // in the upper triangle to minimize cache misses
    for(i =0; i < n; ++i ) 
    {
        const matrix_data_t el_ii = t[i*n+i];
        for(j = 0; j <= i; ++j ) 
        {
            matrix_data_t sum = (i==j) ? (matrix_data_t)1.0 : (matrix_data_t)0;
            for(k=i-1; k >=j; --k ) 
            {
                sum -= t[i*n+k]*a[j*n+k];
            }
            a[j*n+i] = sum / el_ii;
        }
    }
    // solve the system and handle the previous solution being in the upper triangle
    // takes advantage of symmetry
    for(i=n-1; i>=0; --i ) 
    {
        const matrix_data_t el_ii = t[i*n+i];
        for(j = 0; j <= i; ++j ) 
        {
            matrix_data_t sum = (i<j) ? 0 : a[j*n+i];
            for(k=i+1; k<n; ++k) 
            {
                sum -= t[k*n+i]*a[j*n+k];
            }
            a[i*n+j] = a[j*n+i] = sum / el_ii;
        }
    }
}

/*!
* \brief Performs a matrix multiplication such that {\ref c} = {\ref a} * {\ref b}
* \param[in] a Matrix A
* \param[in] b Matrix B
* \param[in] c Resulting matrix C
* \param[in] aux Auxiliary vector that can hold a column of {\ref b}
*/
void matrix_mult(const matrix_t *const a, const matrix_t *const b, const matrix_t *RESTRICT const c, matrix_data_t *baux)
{
    uint_fast8_t i, j, k;

    // assert pointer validity
    assert(a != (matrix_t*)0);
    assert(b != (matrix_t*)0);
    assert(c != (matrix_t*)0);
    assert(baux != (matrix_data_t*)0);
    
    // test dimensions of a and b
    assert(a->cols == b->rows);
    
    // test dimension of c
    assert(a->rows == c->rows);
    assert(b->cols == c->cols);

    for (j = 0; j < b->cols; j++) 
    {
        // create a copy of the column in B to avoid cache issues
        for (k = 0; k < b->rows; k++) 
        {
            baux[k] = matrix_get(b, k, j);
        }

        int indexA = 0;
        for (i = 0; i < a->rows; i++) 
        {
            matrix_data_t total = (matrix_data_t)0;
            for (k = 0; k < b->rows;) 
            {
                total += a->data[indexA++]*baux[k++];
            }
            c->data[i*c->cols + j] = total;
        }
    }
}