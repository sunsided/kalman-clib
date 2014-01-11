/*!
* \brief Initializes a named Kalman filter measurement structure.
*
* This include requires the two defines {\ref KALMAN_MEASUREMENT_NAME} and {\ref KALMAN_NUM_MEASUREMENTS}
* to be set to the base name of the Kalman Filter measurement and to the number of measured outputs,
* as well as all defines from \ref {kalman_factory_filter.h}
*
* It then will instantiate the buffers required for H, R, z and all helper matrices and vectors, 
* as well as the structure for the Kalman filter measurement and the initialization method.
*
* Suppose a Kalman filter named "direction" for which a measurement named "gyroscope" shall be crated 
* which has three measured outputs. You would start by defining the required macros
*
* \code{.c}
* #define KALMAN_MEASUREMENT_NAME gyroscope
* #define KALMAN_NUM_MEASUREMENTS 3
* \endcode
*
* After that, this file must be included
*
* \code{.c}
* #include "kalman_factory_measurement.h"
* \endcode
*
* At this point, the structure \c kalman_filter_direction_measurement_gyroscope will be created (statically) along with
* all the required buffers (i.e. \c kalman_filter_direction_measurement_gyroscope_H_buffer, etc.) and the matrices
* will be initialized and set with the correct dimensions.
*
* In addition, a parameterless static initialization function \code {kalman_filter_direction_measurement_gyroscope_init()} will
* be created which you will need to call manually in order to set up the measurement.
*
* A full example would be

* \code{.c}
* #define KALMAN_NAME example
* #define KALMAN_NUM_STATES 4
* #define KALMAN_NUM_INPUTS 0
* #include "kalman_factory_filter.h"
*
* #define KALMAN_MEASUREMENT_NAME gyroscope
* #define KALMAN_NUM_MEASUREMENTS 3
* #include "kalman_factory_measurement.h"

* #include "kalman_factory_cleanup.h"

* void test_kalman()
* {
*   kalman_filter_example_init();
*   kalman_filter_example_measurement_gyroscope_init();
*
*   kalman_filter_example.x.data[0] = 1;
*   kalman_filter_example_measurement_gyroscope.z.data[0] = 1;
* }
* \endcode
*/

/************************************************************************/
/* Check for inputs                                                     */
/************************************************************************/

#ifndef KALMAN_NAME
#error KALMAN_NAME needs to be defined prior to inclusion of this file.
#endif

#ifndef KALMAN_MEASUREMENT_NAME
#error KALMAN_MEASUREMENT_NAME needs to be defined prior to inclusion of this file.
#endif

#ifndef KALMAN_NUM_STATES
#error KALMAN_NUM_STATES needs to be defined prior to inclusion of this file.
#elif KALMAN_NUM_STATES <= 0
#error KALMAN_NUM_STATES must be a positive integer
#endif

#ifndef KALMAN_NUM_MEASUREMENTS
#error KALMAN_NUM_MEASUREMENTS needs to be defined prior to inclusion of this file.
#elif KALMAN_NUM_MEASUREMENTS < 0
#error KALMAN_NUM_MEASUREMENTS must be a positive integer or zero if no inputs are used
#endif

/************************************************************************/
/* Prepare dimensions                                                   */
/************************************************************************/

// H maps from state to measurement
#define __KALMAN_H_ROWS     KALMAN_NUM_MEASUREMENTS
#define __KALMAN_H_COLS     KALMAN_NUM_STATES

// z contains the measurements
#define __KALMAN_z_ROWS     KALMAN_NUM_MEASUREMENTS
#define __KALMAN_z_COLS     1

// R models the measurement uncertainties / the process noise covariance
#define __KALMAN_R_ROWS     KALMAN_NUM_MEASUREMENTS
#define __KALMAN_R_COLS     KALMAN_NUM_MEASUREMENTS

// y contains the innovation, i.e. difference from predicted to measured values
#define __KALMAN_y_ROWS     KALMAN_NUM_MEASUREMENTS
#define __KALMAN_y_COLS     1

// S contains the innovation covariance (residual covariance)
#define __KALMAN_S_ROWS     KALMAN_NUM_MEASUREMENTS
#define __KALMAN_S_COLS     KALMAN_NUM_MEASUREMENTS

// K models the state covariance gain
#define __KALMAN_K_ROWS     KALMAN_NUM_STATES
#define __KALMAN_K_COLS     KALMAN_NUM_MEASUREMENTS

/************************************************************************/
/* Name helper macro                                                    */
/************************************************************************/

#pragma message("** Instantiating Kalman filter \"" STRINGIFY(KALMAN_NAME) "\" measurement \"" STRINGIFY(KALMAN_MEASUREMENT_NAME) "\" with " STRINGIFY(KALMAN_NUM_MEASUREMENTS) " measured outputs")

#define KALMAN_MEASUREMENT_FUNCTION_NAME_HELPER2(basename)       KALMAN_BASENAME_HELPER(KALMAN_FILTER_BASENAME) ## measurement_ ## basename
#define KALMAN_MEASUREMENT_FUNCTION_NAME_HELPER3(basename)       KALMAN_MEASUREMENT_FUNCTION_NAME_HELPER2(basename) ## _
#define KALMAN_MEASUREMENT_FUNCTION_NAME_HELPER(basename)        KALMAN_MEASUREMENT_FUNCTION_NAME_HELPER3(basename)

#define KALMAN_MEASUREMENT_BASENAME_HELPER(basename)             KALMAN_MEASUREMENT_FUNCTION_NAME_HELPER2(basename)
#define KALMAN_MEASUREMENT_BASENAME                              KALMAN_MEASUREMENT_BASENAME_HELPER(KALMAN_MEASUREMENT_NAME)

/************************************************************************/
/* Name macro                                                           */
/************************************************************************/

#define KALMAN_MEASUREMENT_FUNCTION_NAME(action)                  KALMAN_MEASUREMENT_FUNCTION_NAME_HELPER(KALMAN_MEASUREMENT_NAME) ## action
#define KALMAN_MEASUREMENT_STRUCT_NAME                            KALMAN_FILTER_BASENAME
#define KALMAN_MEASUREMENT_BUFFER_NAME(element)                   KALMAN_MEASUREMENT_BASENAME_HELPER(KALMAN_MEASUREMENT_NAME) ## _ ## element ## _buffer

/************************************************************************/
/* Construct Kalman filter measurement                                  */
/************************************************************************/

#include "compiler.h"
#include "matrix.h"
#include "kalman.h"

#define __KALMAN_BUFFER_H   KALMAN_MEASUREMENT_BUFFER_NAME(H)
#define __KALMAN_BUFFER_R   KALMAN_MEASUREMENT_BUFFER_NAME(R)
#define __KALMAN_BUFFER_z   KALMAN_MEASUREMENT_BUFFER_NAME(z)

#define __KALMAN_BUFFER_K   KALMAN_MEASUREMENT_BUFFER_NAME(K)
#define __KALMAN_BUFFER_S   KALMAN_MEASUREMENT_BUFFER_NAME(S)
#define __KALMAN_BUFFER_y   KALMAN_MEASUREMENT_BUFFER_NAME(y)

#pragma message("Creating Kalman measurement H buffer: " STRINGIFY(__KALMAN_BUFFER_H))
static matrix_data_t __KALMAN_BUFFER_H[__KALMAN_H_ROWS * __KALMAN_H_COLS];

#pragma message("Creating Kalman measurement R buffer: " STRINGIFY(__KALMAN_BUFFER_R))
static matrix_data_t __KALMAN_BUFFER_R[__KALMAN_R_ROWS * __KALMAN_R_COLS];

#pragma message("Creating Kalman measurement z buffer: " STRINGIFY(__KALMAN_BUFFER_z))
static matrix_data_t __KALMAN_BUFFER_z[__KALMAN_z_ROWS * __KALMAN_z_COLS];

#pragma message("Creating Kalman measurement K buffer: " STRINGIFY(__KALMAN_BUFFER_K))
static matrix_data_t __KALMAN_BUFFER_K[__KALMAN_K_ROWS * __KALMAN_K_COLS];

#pragma message("Creating Kalman measurement S buffer: " STRINGIFY(__KALMAN_BUFFER_S))
static matrix_data_t __KALMAN_BUFFER_S[__KALMAN_S_ROWS * __KALMAN_S_COLS];

#pragma message("Creating Kalman measurement y buffer: " STRINGIFY(__KALMAN_BUFFER_y))
static matrix_data_t __KALMAN_BUFFER_y[__KALMAN_y_ROWS * __KALMAN_y_COLS];

#pragma message("Creating Kalman measurement structure: " STRINGIFY(KALMAN_MEASUREMENT_BASENAME))
static kalman_measurement_t KALMAN_MEASUREMENT_BASENAME;

#pragma message ("Creating Kalman measurement initialization function: " STRINGIFY(KALMAN_MEASUREMENT_FUNCTION_NAME(init()) ))

/*!
* \brief Initializes the Kalman Filter measurement
*/
STATIC_INLINE void KALMAN_MEASUREMENT_FUNCTION_NAME(init)()
{
    kalman_measurement_initialize(&KALMAN_MEASUREMENT_BASENAME, KALMAN_NUM_STATES, KALMAN_NUM_MEASUREMENTS, __KALMAN_BUFFER_H, __KALMAN_BUFFER_z, __KALMAN_BUFFER_R, __KALMAN_BUFFER_y, __KALMAN_BUFFER_S, __KALMAN_BUFFER_K);
}

/************************************************************************/
/* Clean up                                                             */
/************************************************************************/

#undef KALMAN_MEASUREMENT_NAME
#undef KALMAN_NUM_MEASUREMENTS

#undef KALMAN_MEASUREMENT_BASENAME_HELPER
#undef KALMAN_MEASUREMENT_BASENAME

#undef KALMAN_MEASUREMENT_FUNCTION_NAME_HELPER2
#undef KALMAN_MEASUREMENT_FUNCTION_NAME_HELPER
#undef KALMAN_MEASUREMENT_FUNCTION_NAME

#undef KALMAN_MEASUREMENT_BUFFER_NAME

#undef __KALMAN_BUFFER_H
#undef __KALMAN_BUFFER_R
#undef __KALMAN_BUFFER_z

#undef __KALMAN_BUFFER_K
#undef __KALMAN_BUFFER_S
#undef __KALMAN_BUFFER_y