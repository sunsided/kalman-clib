#ifndef _CHOLESKY_H_
#define _CHOLESKY_H_

#include "matrix.h"

/**
* \brief Decomposes a matrix into lower triangular form using Cholesky decomposition.
*/
int cholesky_decompose_lower(matrix_t* mat);

#endif