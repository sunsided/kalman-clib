#include "kalman.h"

/*!
* \brief Gets a pointer to the state vector.
* \param[in] kf The Kalman Filter structure to correct.
* \return The state vector.
*/
matrix_t* kalman_get_state(kalman_t *kf)
{
    return &(kf->x);
}

/*!
* \brief Gets a pointer to the input vector.
* \param[in] kf The Kalman Filter structure to correct.
* \return The input vector.
*/
matrix_t* kalman_get_input(kalman_t *kf)
{
    return &(kf->u);
}

/*!
* \brief Gets a pointer to the input covariance matrix.
* \param[in] kf The Kalman Filter structure to correct.
* \return The input covariance matrix.
*/
matrix_t* kalman_get_input_covariance(kalman_t *kf)
{
    return &(kf->Q);
}

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
void kalman_initialize(kalman_t *kf, uint_fast8_t num_states, uint_fast8_t num_inputs, matrix_data_t *A, matrix_data_t *x, matrix_data_t *B, matrix_data_t *u, matrix_data_t *P, matrix_data_t *Q)
{
    // TODO: Implement
}