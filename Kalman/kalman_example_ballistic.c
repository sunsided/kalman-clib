// create the filter structure
#define KALMAN_NAME ballistic
#define KALMAN_NUM_STATES 4
#define KALMAN_NUM_INPUTS 2
#include "kalman_factory_filter.h"

// create the measurement structure
#define KALMAN_MEASUREMENT_NAME position
#define KALMAN_NUM_MEASUREMENTS 2
#include "kalman_factory_measurement.h"

// clean up
#include "kalman_factory_cleanup.h"

/*!
* An example for the Kalman Filter factories
*/
static void kalman_ballistic_init()
{
    // initialize the filter structures
    kalman_t *kf = kalman_filter_ballistic_init();
    kalman_measurement_t *kfm = kalman_filter_ballistic_measurement_position_init();

    // set coefficients
    matrix_t *x = kalman_get_state_vector(kf);

}