/************************************************************************/
/* Check for inputs                                                     */
/************************************************************************/

#ifndef KALMAN_NAME
#error KALMAN_NAME needs to be defined prior to inclusion of this file.
#endif

#ifndef KALMAN_NUM_STATES
#error KALMAN_NUM_STATES needs to be defined prior to inclusion of this file.
#elif KALMAN_NUM_STATES <= 0
#error KALMAN_NUM_STATES must be a positive integer
#endif

#ifndef KALMAN_NUM_INPUTS
#error KALMAN_NUM_INPUTS needs to be defined prior to inclusion of this file.
#elif KALMAN_NUM_INPUTS < 0
#error KALMAN_NUM_INPUTS must be a positive integer or zero if no inputs are used
#endif

/************************************************************************/
/* Prepare dimensions                                                   */
/************************************************************************/

#define __KALMAN_A_ROWS     KALMAN_NUM_STATES
#define __KALMAN_A_COLS     KALMAN_NUM_STATES

#define __KALMAN_P_ROWS     KALMAN_NUM_STATES
#define __KALMAN_P_COLS     KALMAN_NUM_STATES

#define __KALMAN_x_ROWS     KALMAN_NUM_STATES
#define __KALMAN_x_COLS     1

#define __KALMAN_B_ROWS     KALMAN_NUM_STATES
#define __KALMAN_B_COLS     KALMAN_NUM_INPUTS

#define __KALMAN_u_ROWS     KALMAN_NUM_INPUTS
#define __KALMAN_u_COLS     1

#define __KALMAN_Q_ROWS     KALMAN_NUM_INPUTS
#define __KALMAN_Q_COLS     KALMAN_NUM_INPUTS

/************************************************************************/
/* Name helper macro                                                    */
/************************************************************************/

#define __CONCAT(x, y)                                  x ## y

#define KALMAN_FILTER_BASENAME_HELPER(name)             __CONCAT(kalman_filter_, name)
#define KALMAN_FILTER_BASENAME                          KALMAN_FILTER_BASENAME_HELPER(KALMAN_NAME)
#define KALMAN_BASENAME_HELPER(basename)                __CONCAT(basename, _)

/************************************************************************/
/* Name macro                                                           */
/************************************************************************/

#define KALMAN_BUFFER_NAME(element)                     KALMAN_BASENAME_HELPER(KALMAN_FILTER_BASENAME) ## element ## _buffer
#define KALMAN_FUNCTION_NAME(name)                      KALMAN_BASENAME_HELPER(KALMAN_FILTER_BASENAME) ## name
#define KALMAN_STRUCT_NAME                              KALMAN_FILTER_BASENAME

/************************************************************************/
/* Construct Kalman filter                                              */
/************************************************************************/

#include "compiler.h"
#include "matrix.h"
#include "kalman.h"

#define __KALMAN_BUFFER_A   KALMAN_BUFFER_NAME(A)
#define __KALMAN_BUFFER_P   KALMAN_BUFFER_NAME(P)
#define __KALMAN_BUFFER_x   KALMAN_BUFFER_NAME(x)

static matrix_data_t __KALMAN_BUFFER_A[__KALMAN_A_ROWS * __KALMAN_A_COLS];
static matrix_data_t __KALMAN_BUFFER_P[__KALMAN_P_ROWS * __KALMAN_P_COLS];
static matrix_data_t __KALMAN_BUFFER_x[__KALMAN_x_ROWS * __KALMAN_x_COLS];

#if KALMAN_NUM_INPUTS > 0

#define __KALMAN_BUFFER_B   KALMAN_BUFFER_NAME(B)
#define __KALMAN_BUFFER_Q   KALMAN_BUFFER_NAME(Q)
#define __KALMAN_BUFFER_u   KALMAN_BUFFER_NAME(u)

static matrix_data_t __KALMAN_BUFFER_B[__KALMAN_B_ROWS * __KALMAN_B_COLS];
static matrix_data_t __KALMAN_BUFFER_Q[__KALMAN_Q_ROWS * __KALMAN_Q_COLS];
static matrix_data_t __KALMAN_BUFFER_u[__KALMAN_x_ROWS * __KALMAN_u_COLS];

#else

#define __KALMAN_BUFFER_B ((matrix_data_t*)0)
#define __KALMAN_BUFFER_Q ((matrix_data_t*)0)
#define __KALMAN_BUFFER_u ((matrix_data_t*)0)

#endif

static kalman_t KALMAN_STRUCT_NAME;

/*!
* \brief Initializes the Kalman Filter
*/
STATIC_INLINE KALMAN_FUNCTION_NAME(init)()
{
    kalman_initialize(&KALMAN_STRUCT_NAME, KALMAN_NUM_STATES, KALMAN_NUM_INPUTS, __KALMAN_BUFFER_A, __KALMAN_BUFFER_x, __KALMAN_BUFFER_B, __KALMAN_BUFFER_u, __KALMAN_BUFFER_P, __KALMAN_BUFFER_Q);
}

/************************************************************************/
/* Clean up                                                             */
/************************************************************************/

// remove x macros
#undef __KALMAN_x_ROWS
#undef __KALMAN_x_COLS
#undef __KALMAN_BUFFER_x

// remove u macros
#undef __KALMAN_u_ROWS
#undef __KALMAN_u_COLS
#undef __KALMAN_BUFFER_u

// remove A macros
#undef __KALMAN_A_ROWS
#undef __KALMAN_A_COLS
#undef __KALMAN_BUFFER_A

// remove P macros
#undef __KALMAN_P_ROWS
#undef __KALMAN_P_COLS
#undef __KALMAN_BUFFER_P

// remove B macros
#undef __KALMAN_B_ROWS
#undef __KALMAN_B_COLS
#undef __KALMAN_BUFFER_B

// remove Q macros
#undef __KALMAN_Q_ROWS
#undef __KALMAN_Q_COLS
#undef __KALMAN_BUFFER_Q

// remove name helper macros
#undef KALMAN_BASENAME_HELPER
#undef KALMAN_FILTER_BASENAME
#undef KALMAN_FILTER_BASENAME_HELPER
#undef __CONCAT

// remove name macros
#undef KALMAN_STRUCT_NAME
#undef KALMAN_FUNCTION_NAME
#undef KALMAN_BUFFER_NAME