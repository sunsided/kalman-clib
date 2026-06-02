// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2014-2024 Markus Mayer
//
// Part of kalman-clib - https://github.com/sunsided/kalman-clib
// Licensed under the MIT License. See LICENSE.md in the project root for details.

#include <assert.h>
#include <math.h>
#include <stdint.h>

#define EXTERN_INLINE_MATRIX static INLINE
#define EXTERN_INLINE_KALMAN static INLINE

#include "kalman.h"
#include "kalman_unittests.h"

/*!
* \brief Asserts that two matrix values are equal within a small tolerance.
*
* The expected values in these tests are exact integers reached by multiply-add
* chains, so they are representable in the default float matrix_data_t and would
* even pass an exact `==`. We compare with a tolerance instead so the tests stay
* correct if matrix_data_t becomes a less exact type, or if a translation unit is
* built with -ffast-math (which may reassociate the floating-point operations).
*/
static void expect_close(matrix_data_t actual, matrix_data_t expected)
{
    const double tol = 1e-3;
    assert(fabs((double)actual - (double)expected) <= tol);
}

/*!
* \brief Regression test for the covariance prediction with zero inputs.
*
* Reproduces the layout the filter factory produces for a filter without inputs
* (see kalman_factory_filter.h): B is (num_states x 0) and Q is (0 x 0), both with
* a NULL data buffer. The covariance update must reduce to P = A*P*A' and must NOT
* feed the empty/NULL B and Q matrices into the matrix routines.
*
* This pins finding #1: the input guard used to test B.rows (== num_states, always
* > 0) instead of B.cols (== num_inputs). With the buggy guard, kalman_predict_Q
* calls matrix_mult / matrix_multadd_transb with B/Q whose data == NULL, tripping
* the data-pointer assertions in matrix.c. With the fix the B*Q*B' branch is skipped.
*/
static void test_predict_zero_inputs(void)
{
    enum { N = 2 };

    matrix_data_t A[N * N] = { 1, 2,
                               0, 1 };
    matrix_data_t P[N * N] = { 1, 0,
                               0, 1 };

    matrix_data_t aux[N] = { 0, 0 };
    matrix_data_t predicted_x[N] = { 0, 0 };
    matrix_data_t temp_P[N * N] = { 0, 0, 0, 0 };
    // temp_BQ is (N x 0) == zero elements; mirror the factory by giving it a real
    // (non-NULL) backing buffer regardless.
    matrix_data_t temp_BQ[1] = { 0 };

    kalman_t kf;
    kalman_filter_initialize(&kf, N, /*num_inputs*/ 0,
                             A, /*x*/ (matrix_data_t *)0,
                             /*B*/ (matrix_data_t *)0, /*u*/ (matrix_data_t *)0,
                             P, /*Q*/ (matrix_data_t *)0,
                             aux, predicted_x, temp_P, temp_BQ);

    kalman_predict_Q(&kf);

    // Expected P' = A*P*A' (no B*Q*B' contribution). With P = I this is A*A':
    //   [[1*1+2*2, 1*0+2*1], [0*1+1*2, 0*0+1*1]] = [[5, 2], [2, 1]]
    expect_close(P[0], 5);
    expect_close(P[1], 2);
    expect_close(P[2], 2);
    expect_close(P[3], 1);
}

/*!
* \brief Regression test for the covariance prediction with inputs.
*
* Ensures the B*Q*B' term is still applied when the filter has inputs, guarding
* against a regression that disables or mis-scales the input contribution.
*/
static void test_predict_with_inputs(void)
{
    enum { N = 1, M = 1 };

    matrix_data_t A[N * N] = { 1 };
    matrix_data_t P[N * N] = { 3 }; // p = 3
    matrix_data_t B[N * M] = { 2 }; // b = 2
    matrix_data_t Q[M * M] = { 5 }; // q = 5

    matrix_data_t aux[N] = { 0 };
    matrix_data_t predicted_x[N] = { 0 };
    matrix_data_t temp_P[N * N] = { 0 };
    matrix_data_t temp_BQ[N * M] = { 0 };

    kalman_t kf;
    kalman_filter_initialize(&kf, N, M,
                             A, /*x*/ (matrix_data_t *)0,
                             B, /*u*/ (matrix_data_t *)0,
                             P, Q,
                             aux, predicted_x, temp_P, temp_BQ);

    kalman_predict_Q(&kf);

    // Expected P' = A*P*A' + B*Q*B' = 1*3*1 + 2*5*2 = 3 + 20 = 23
    expect_close(P[0], 23);
}

/*!
* \brief Regression test for in-place matrix subtraction with output aliasing input a.
*
* Mirrors kalman_correct's `matrix_sub(P, temp_KHP, P)` (P = P - K*H*P). Pins finding
* #2: matrix_sub must produce correct results when c aliases a, even after the input
* loses its (incorrect) `restrict` qualifier. Build with optimization for this to bite.
*/
static void test_matrix_sub_aliased(void)
{
    matrix_data_t pd[2 * 2] = { 10, 20, 30, 40 };
    matrix_data_t kd[2 * 2] = { 1, 2, 3, 4 };

    matrix_t p, k;
    matrix_init(&p, 2, 2, pd);
    matrix_init(&k, 2, 2, kd);

    // output (c) aliases input (a), as in kalman_correct
    matrix_sub(&p, &k, &p);

    expect_close(pd[0], 9);
    expect_close(pd[1], 18);
    expect_close(pd[2], 27);
    expect_close(pd[3], 36);
}

/*!
* \brief Regression tests for the Kalman filter core.
*/
void kalman_unittests()
{
    test_predict_zero_inputs();
    test_predict_with_inputs();
    test_matrix_sub_aliased();
}
