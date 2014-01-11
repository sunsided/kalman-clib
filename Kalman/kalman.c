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