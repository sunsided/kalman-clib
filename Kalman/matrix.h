#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdint.h>

typedef float matrix_data_t;

typedef struct {
    uint_fast8_t rows;
    uint_fast8_t cols;
    matrix_data_t *data;
} matrix_t;

/**
* \brief Inverts a lower triangular matrix.
* \param[in] lower The lower triangular matrix to be inverted.
* \param[out] inverse The inverse of the lower triangular matrix.
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_invert_lower(const matrix_t *const lower, matrix_t* inverse);

#endif