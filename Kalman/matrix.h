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
void matrix_init(matrix_t *mat, const uint_fast8_t rows, const uint_fast8_t cols, matrix_data_t *buffer);

/**
* \brief Inverts a lower triangular matrix.
* \param[in] lower The lower triangular matrix to be inverted.
* \param[in] inverse The calculated inverse of the lower triangular matrix.
*
* Kudos: https://code.google.com/p/efficient-java-matrix-library
*/
void matrix_invert_lower(const matrix_t *RESTRICT const lower, matrix_t *RESTRICT inverse);

/*!
* \brief Gets a matrix element
* \param[in] mat The matrix to initialize
* \param[in] rows The row
* \param[in] cols The column
* \return The value at the given cell.
*/
EXTERN_INLINE_MATRIX matrix_data_t matrix_get(matrix_t *mat, const register uint_fast8_t row, const register uint_fast8_t column)
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
EXTERN_INLINE_MATRIX void matrix_set(matrix_t *mat, const register uint_fast8_t row, const register uint_fast8_t column, const register matrix_data_t value)
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
EXTERN_INLINE_MATRIX void matrix_get_row_pointer(matrix_t *mat, const register uint_fast8_t row, matrix_data_t **row_data)
{
    register uint_fast16_t address = row * mat->cols;
    *row_data = &mat->data[address];
}

/*!
* \brief Gets a copy of a matrix row
* \param[in] mat The matrix to initialize
* \param[in] rows The row
* \param[in] row_data Pointer to an array of the correct length to hold a row of matrix {\ref mat}.
*/
void matrix_get_row_copy(matrix_t *mat, const register uint_fast8_t row, matrix_data_t *row_data);

/*!
* \brief Gets a copy of a matrix column
* \param[in] mat The matrix to initialize
* \param[in] rows The column
* \param[in] row_data Pointer to an array of the correct length to hold a column of matrix {\ref mat}.
*/
void matrix_get_column_copy(matrix_t *mat, const register uint_fast8_t column, matrix_data_t *row_data);

#endif