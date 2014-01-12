// create the filter structure
#define KALMAN_NAME gravity
#define KALMAN_NUM_STATES 3
#define KALMAN_NUM_INPUTS 0
#include "kalman_factory_filter.h"

// create the measurement structure
#define KALMAN_MEASUREMENT_NAME position
#define KALMAN_NUM_MEASUREMENTS 1
#include "kalman_factory_measurement.h"

// clean up
#include "kalman_factory_cleanup.h"

/*!
* An example for the Kalman Filter factories
*/
static void kalman_ballistic_init()
{
    /************************************************************************/
    /* initialize the filter structures                                     */
    /************************************************************************/
    kalman_t *kf = kalman_filter_gravity_init();
    kalman_measurement_t *kfm = kalman_filter_gravity_measurement_position_init();

    /************************************************************************/
    /* set initial state                                                    */
    /************************************************************************/
    matrix_t *x = kalman_get_state_vector(kf);
    x->data[0] = 0; // s_i
    x->data[1] = 0; // v_i
    x->data[2] = 0; // g_i

    /************************************************************************/
    /* set state transition                                                 */
    /************************************************************************/
    matrix_t *A = kalman_get_state_transition(kf);
    
    // transition of x to s
    matrix_set(A, 0, 0, 1);   // 1
    matrix_set(A, 0, 1, 1);   // T
    matrix_set(A, 0, 2, 0.5); // 0.5 * T^2
    
    // transition of x to v
    matrix_set(A, 1, 0, 0);   // 0
    matrix_set(A, 1, 1, 1);   // 1
    matrix_set(A, 1, 2, 1);   // T

    // transition of x to g
    matrix_set(A, 2, 0, 0);   // 0
    matrix_set(A, 2, 1, 0);   // 0
    matrix_set(A, 2, 2, 1);   // 1

    /************************************************************************/
    /* set covariance                                                       */
    /************************************************************************/
    matrix_t *P = kalman_get_system_covariance(kf);

    matrix_set_symmetric(P, 0, 0, 1);   // var(s)
    matrix_set_symmetric(P, 0, 1, 0);   // cov(s,v)
    matrix_set_symmetric(P, 0, 2, 0);   // cov(s,g)

    matrix_set_symmetric(P, 1, 1, 1);   // var(v)
    matrix_set_symmetric(P, 1, 2, 0);   // cov(v,g)

    matrix_set_symmetric(P, 2, 2, 1);   // var(g)

    /************************************************************************/
    /* set measurement transformation                                       */
    /************************************************************************/
    matrix_t *H = kalman_get_measurement_transformation(kfm);

    matrix_set(H, 0, 0, 1);     // z = 1*s 
    matrix_set(H, 0, 1, 0);     //   + 0*v
    matrix_set(H, 0, 2, 0);     //   + 0*g

    /************************************************************************/
    /* set process noise                                                    */
    /************************************************************************/
    matrix_t *R = kalman_get_process_noise(kfm);

    matrix_set(R, 0, 0, 1);     // var(s)
}