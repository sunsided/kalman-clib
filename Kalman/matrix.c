#include <stdint.h>
#include "matrix.h"

/**
* \brief Initializes a matrix structure.
* \param[in] mat The matrix to initialize
* \param[in] rows The number of rows
* \param[in] cols The number of columns
* \param[in] buffer The data buffer (of size {\see rows} x {\see cols}).
*/
void matrix_init(matrix_t *mat, uint_fast8_t rows, uint_fast8_t cols, matrix_data_t *buffer)
{
    mat->cols = 3;
    mat->rows = 3;
    mat->data = buffer;
}

/**
* \brief Inverts a lower triangular matrix.
* \param[in] lower The lower triangular matrix to be inverted.
* \param[out] inverse The inverse of the lower triangular matrix.
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_invert_lower(const matrix_t *RESTRICT const lower, matrix_t *RESTRICT  inverse)
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