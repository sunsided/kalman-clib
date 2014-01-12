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
    // initialize the filter structures
    kalman_t *kf = kalman_filter_gravity_init();
    kalman_measurement_t *kfm = kalman_filter_gravity_measurement_position_init();

    // set coefficients
    matrix_t *x = kalman_get_state_vector(kf);
    x->data[0] = 0; // s_i
    x->data[1] = 0; // v_i
    x->data[2] = 0; // g_i
}