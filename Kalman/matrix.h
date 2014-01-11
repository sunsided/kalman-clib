#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdint.h>
#include "compiler.h"

/*!
* \def EXTERN_INLINE_MATRIX Helper inline to switch from local inline to extern inline
*/
#ifndef EXTERN_INLINE_MATRIX
#define EXTERN_INLINE_MATRIX EXTERN_INLINE
#endif

/**
* Matrix data type definition.
*/
typedef float matrix_data_t;

/**
* \brief Matrix definition
*/
typedef struct {
    /**
    * \brief Number of rows
    */
    const uint_fast8_t rows;

    /**
    * \brief Number of columns
    */
    const uint_fast8_t cols;

    /**
    * \brief Pointer to the data array of size {\see rows} x {\see cols}.
    */
    matrix_data_t *const data;
} matrix_t;

/**
* \brief Initializes a matrix structure.
* \param[in] mat The matrix to initialize
* \param[in] rows The number of rows
* \param[in] cols The number of columns
* \param[in] buffer The data buffer (of size {\see rows} x {\see cols}).
*/
void matrix_init(matrix_t *mat, const uint_fast8_t rows, const uint_fast8_t cols, matrix_data_t *buffer) PURE COLD;

/**
* \brief Inverts a lower triangular matrix.
* \param[in] lower The lower triangular matrix to be inverted.
* \param[in] inverse The calculated inverse of the lower triangular matrix.
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_invert_lower(const matrix_t *RESTRICT const lower, matrix_t *RESTRICT inverse) PURE HOT;

/*!
* \brief Performs a matrix multiplication such that {\ref c} = {\ref x} * {\ref b}
* \param[in] a Matrix A
* \param[in] x Vector x
* \param[in] c Resulting vector C (will be overwritten)
* \param[in] aux Auxiliary vector that can hold a column of {\ref b}
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_mult_vector(const matrix_t *RESTRICT const a, const matrix_t *RESTRICT const x, const matrix_t *RESTRICT const c) PURE HOT;

/*!
* \brief Performs a matrix multiplication such that {\ref c} = {\ref a} * {\ref b}
* \param[in] a Matrix A
* \param[in] b Matrix B
* \param[in] c Resulting matrix C (will be overwritten)
* \param[in] aux Auxiliary vector that can hold a column of {\ref b}
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_mult(const matrix_t *const a, const matrix_t *const b, const matrix_t *RESTRICT const c, matrix_data_t *baux) PURE HOT;

/*!
* \brief Performs a matrix multiplication with transposed B such that {\ref c} = {\ref a} * {\ref b'}
* \param[in] a Matrix A
* \param[in] b Matrix B
* \param[in] c Resulting matrix C (will be overwritten)
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_mult_transb(const matrix_t *const a, const matrix_t *const b, const matrix_t *RESTRICT const c) PURE HOT;

/*!
* \brief Performs a matrix multiplication with transposed B and adds the result to {\ref c} such that {\ref c} = {\ref c} + {\ref a} * {\ref b'}
* \param[in] a Matrix A
* \param[in] b Matrix B
* \param[in] c Resulting matrix C (will be added to)
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_multadd_transb(const matrix_t *const a, const matrix_t *const b, const matrix_t *RESTRICT const c) PURE HOT;

/*!
* \brief Performs a matrix multiplication with transposed B and scales the result such that {\ref c} = {\ref a} * {\ref b'} * {\ref scale}
* \param[in] a Matrix A
* \param[in] b Matrix B
* \param[in] scale Scaling factor
* \param[in] c Resulting matrix C(will be overwritten)
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_multscale_transb(const matrix_t *const a, const matrix_t *const b, register const matrix_data_t scale, const matrix_t *RESTRICT const c) PURE HOT;

/*!
* \brief Gets a matrix element
* \param[in] mat The matrix to initialize
* \param[in] rows The row
* \param[in] cols The column
* \return The value at the given cell.
*/
EXTERN_INLINE_MATRIX matrix_data_t matrix_get(const matrix_t *const mat, const register uint_fast8_t row, const register uint_fast8_t column) PURE
{
    register uint_fast16_t address = row * mat->cols + column;
    return mat->data[address];
}

/*!
* \brief Gets a matrix element
* \param[in] mat The matrix to initialize
* \param[in] rows The row
* \param[in] cols The column
* \return The value at the given cell.
*/
EXTERN_INLINE_MATRIX void matrix_set(matrix_t *mat, const register uint_fast8_t row, const register uint_fast8_t column, const register matrix_data_t value) PURE COLD
{
    register uint_fast16_t address = row * mat->cols + column;
    mat->data[address] = value;
}

/*!
* \brief Gets a pointer to a matrix row
* \param[in] mat The matrix to initialize
* \param[in] rows The row
* \param[out] row_data A pointer to the given matrix row
*/
EXTERN_INLINE_MATRIX void matrix_get_row_pointer(const matrix_t *const mat, const register uint_fast8_t row, matrix_data_t **row_data) PURE
{
    register uint_fast16_t address = row * mat->cols;
    *row_data = &mat->data[address];
}

/*!
* \brief Gets a copy of a matrix column
* \param[in] mat The matrix to initialize
* \param[in] rows The column
* \param[in] row_data Pointer to an array of the correct length to hold a column of matrix {\ref mat}.
*/
EXTERN_INLINE_MATRIX void matrix_get_column_copy(const matrix_t *const mat, const register uint_fast8_t column, register matrix_data_t *row_data) PURE HOT
{
    // start from the back, so target index is equal to the index of the last row.
    register uint_fast8_t target_index = mat->rows - 1;

    // also, the source index is the column..th index
    const register int_fast16_t stride = mat->cols;
    register int_fast16_t source_index = target_index * stride + column;

    // fetch data
    row_data[target_index] = mat->data[source_index];
    while (target_index != 0)
    {
        --target_index;
        source_index -= stride;

        row_data[target_index] = mat->data[source_index];
    }
}

/*!
* \brief Gets a copy of a matrix row
* \param[in] mat The matrix to initialize
* \param[in] rows The row
* \param[in] row_data Pointer to an array of the correct length to hold a row of matrix {\ref mat}.
*/
EXTERN_INLINE_MATRIX void matrix_get_row_copy(const matrix_t *const mat, const register uint_fast8_t row, register matrix_data_t *row_data) PURE
{
    register uint_fast8_t target_index = mat->cols;
    register int_fast16_t source_index = (row + 1) * mat->cols;

    // fetch data
    row_data[target_index] = mat->data[source_index];
    while (target_index != 0)
    {
        --target_index;
        --source_index;
        row_data[target_index] = mat->data[source_index];
    }
}

#undef EXTERN_INLINE_MATRIX
#endif