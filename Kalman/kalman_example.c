
// create the filter structure
#define KALMAN_NAME example
#define KALMAN_NUM_STATES 4
#define KALMAN_NUM_INPUTS 0
#include "kalman_factory_filter.h"

// create the measurement structure
#define KALMAN_MEASUREMENT_NAME gyroscope
#define KALMAN_NUM_MEASUREMENTS 3
#include "kalman_factory_measurement.h"

// clean up
#include "kalman_factory_cleanup.h"

void test_kalman()
{
    kalman_filter_example_init();
    kalman_filter_example_measurement_gyroscope_init();

    kalman_filter_example.x.data[0] = 1;
    kalman_filter_example_measurement_gyroscope.z.data[0] = 1;
}