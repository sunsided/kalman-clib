#include <stdint.h>
#include <assert.h>

#include "cholesky.h"

#define EXTERN_INLINE_KALMAN INLINE
#include "kalman.h"

/*!
* \brief Initializes the Kalman Filter
* \param[in] kf The Kalman Filter structure to initialize
* \param[in] num_states The number of state variables
* \param[in] num_inputs The number of input variables
* \param[in] A The state transition matrix ({\ref num_states} x {\ref num_states})
* \param[in] x The state vector ({\ref num_states} x \c 1)
* \param[in] B The input transition matrix ({\ref num_inputs} x {\ref num_inputs})
* \param[in] u The input vector ({\ref num_inputs} x \c 1)
* \param[in] P The state covariance matrix ({\ref num_states} x {\ref num_states})
* \param[in] Q The input covariance matrix ({\ref num_inputs} x {\ref num_inputs})
*/
void kalman_filter_initialize(kalman_t *kf, uint_fast8_t num_states, uint_fast8_t num_inputs, matrix_data_t *A, matrix_data_t *x, matrix_data_t *B, matrix_data_t *u, matrix_data_t *P, matrix_data_t *Q)
{
    matrix_init(&kf->A, num_states, num_states, A);
    matrix_init(&kf->P, num_states, num_states, P);
    matrix_init(&kf->x, num_states, 1, x);

    matrix_init(&kf->B, num_states, num_inputs, B);
    matrix_init(&kf->Q, num_inputs, num_inputs, Q);
    matrix_init(&kf->u, num_inputs, 1, u);
}


/*!
* \brief Sets the measurement vector
* \param[in] kfm The Kalman Filter measurement structure to initialize
* \param[in] num_states The number of states
* \param[in] num_measurements The number of measurements
* \param[in] H The measurement transformation matrix ({\ref num_measurements} x {\ref num_states})
* \param[in] z The measurement vector ({\ref num_measurements} x \c 1)
* \param[in] R The process noise / measurement uncertainty ({\ref num_measurements} x {\ref num_measurements})
* \param[in] y The innovation ({\ref num_measurements} x \c 1)
* \param[in] S The residual covariance ({\ref num_measurements} x {\ref num_measurements})
* \param[in] K The Kalman gain ({\ref num_states} x {\ref num_measurements})
*/
void kalman_measurement_initialize(kalman_measurement_t *kfm, uint_fast8_t num_states, uint_fast8_t num_measurements, matrix_data_t *H, matrix_data_t *z, matrix_data_t *R, matrix_data_t *y, matrix_data_t *S, matrix_data_t *K)
{
    matrix_init(&kfm->H, num_measurements, num_states, H);
    matrix_init(&kfm->R, num_measurements, num_measurements, R);
    matrix_init(&kfm->z, num_measurements, 1, z);

    matrix_init(&kfm->K, num_states, num_measurements, K);
    matrix_init(&kfm->S, num_measurements, num_measurements, S);
    matrix_init(&kfm->y, num_measurements, 1, y);
}


/*!
* \brief Performs the time update / prediction step.
* \param[in] kf The Kalman Filter structure to predict with.
* \param[in] lambda Lambda factor (\c 0 < {\ref lambda} <= \c 1) to forcibly reduce prediction certainty. Smaller values mean larger uncertainty.
*
* This call assumes that the input covariance and variables are already set in the filter structure.
*/
void kalman_predict(kalman_t *kf, matrix_data_t lambda)
{
    // TODO: need those fields in the structure!
    assert(0);
    matrix_t xpredicted; // xpredicted and aux max use the same backing field
    matrix_data_t aux;
    matrix_t temp;
    // temp needs to be at least size(P)
    
    const matrix_t *RESTRICT const A = &kf->A;
    const matrix_t *RESTRICT const B = &kf->B;
    const matrix_t *RESTRICT P = &kf->P;

    /************************************************************************/
    /* Predict next state using system dynamics                             */
    /* x = A*x                                                              */
    /************************************************************************/

    // x = A*x
    matrix_mult_rowvector(A, &kf->x, &xpredicted);
    matrix_copy(&xpredicted, &kf->x);             // TODO: add unit test

    /************************************************************************/
    /* Predict next covariance using system dynamics and input              */
    /* P = A*P*A' * 1/lambda^2 + B*Q*B'                                     */
    /************************************************************************/

    // lambda = 1/lambda^2
    lambda = (matrix_data_t)1.0 / (lambda * lambda); // TODO: This should be precalculated, e.g. using kalman_set_lambda(...);

    // P = A*P*A'
    matrix_mult(A, P, &temp, &aux);                 // temp = A*P
    matrix_multscale_transb(&temp, A, lambda, P);   // P = temp*A' * 1/(lambda^2)

    // P = P + B*Q*B'
    if (kf->B.rows > 0)
    {
        matrix_mult(B, &kf->Q, &temp, &aux);        // temp = B*Q
        matrix_multadd_transb(&temp, B, P);         // P += temp*B'
    }
}

/*!
* \brief Performs the measurement update step.
* \param[in] kf The Kalman Filter structure to correct.
*/
void kalman_correct(kalman_t *kf, kalman_measurement_t *kfm)
{
    // TODO: need those fields in the structure!
    assert(0);
    matrix_data_t aux;  // aux needs to be max(num_measurements, num_states)
    matrix_t Sinv;      // Sinv needs to be at least size(S) --> num_measurements * num_measurements
    matrix_t temp;      // temp needs to be max(num_measurements, num_states) * max(num_measurements, num_states)
    matrix_t temp2;     // temp2 needs to be num_states * num_states

    const matrix_t *RESTRICT P = &kf->P;
    const matrix_t *RESTRICT const H = &kfm->H;
    const matrix_t *RESTRICT K = &kfm->K;
    const matrix_t *RESTRICT S = &kfm->S;
    const matrix_t *y = &kfm->y;
    const matrix_t *x = &kf->x;

    /************************************************************************/
    /* Calculate innovation and residual covariance                         */
    /* y = z - H*x                                                          */
    /* S = H*P*H' + R                                                       */
    /************************************************************************/

    // y = z - H*x
    matrix_mult_rowvector(H, x, y);
    matrix_sub_inplace_b(&kfm->z, y); // TODO: add unit test

    // S = H*P*H' + R
    matrix_mult(H, P, &temp, &aux);    // temp = A*P
    matrix_mult_transb(&temp, H, S);   // S = temp*A
    matrix_add_inplace(S, &kfm->R);    // S += R 
    // TODO: add unit test

    /************************************************************************/
    /* Calculate Kalman gain                                                */
    /* K = P*H' * S^-1                                                      */
    /************************************************************************/

    // K = P*H' * S^-1
    cholesky_decompose_lower(S);
    matrix_invert_lower(S, &Sinv);      // Sinv = S^-1
    matrix_mult_transb(P, H, &temp);    // temp = P*H'
    matrix_mult(&temp, &Sinv, K, &aux); // K = temp*Sinv

    /************************************************************************/
    /* Correct state prediction                                             */
    /* x = x + K*y                                                          */
    /************************************************************************/

    // x = x + K*y 
    matrix_multadd_rowvector(K, y, x);

    /************************************************************************/
    /* Correct state covariances                                            */
    /* P = (I-K*H) * P                                                      */
    /*   = P - K*(H*P)                                                      */
    /************************************************************************/

    // P = P - K*(H*P)
    matrix_mult(H, P, &temp, &aux);         // temp = H*P
    matrix_mult(K, &temp, &temp2, &aux);    // temp2 = K*temp
    matrix_sub(P, &temp2, P);               // P -= temp2 
    // TODO: add unit test
}