
#define KALMAN_NAME example
#define KALMAN_NUM_STATES 4
#define KALMAN_NUM_INPUTS 0

#include "kalman_factory_init.h"
#include "kalman_factory_cleanup.h"

void test_kalman()
{
    kalman_filter_example_init();
    kalman_filter_example.x.data[0] = 1;
}