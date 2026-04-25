# Kalman Filter C Implementation #

Microcontroller targeted naive Kalman filter implementation in pure C using code ported from the [Efficient Java Matrix Library](https://code.google.com/p/efficient-java-matrix-library).  
A 🦀 Rust port is available at [sunsided/minikalman-rs](https://github.com/sunsided/minikalman-rs).

### License

The project is licensed under the **MIT license**, a copy of which can be found in [LICENSE.md](LICENSE.md).

## Implemented so far ##
* Memory-optimizing preprocessor based Kalman Filter factory
* Algorithmically optimized matrix/matrix and matrix/vector operations
* Matrix inverse using Cholesky decomposition

## Example filters ##
* Gravity constant estimation using only measured position

## Using the library

### Copy-paste sources

Drop the files directly into your own build — no external dependencies, no library to link.

1. Add `src/cholesky.c`, `src/kalman.c`, and `src/matrix.c` to your source list.
2. Add the `include/` directory to your compiler's include path (`-I include`).
3. Link against the math library (`-lm`).

The library uses a **preprocessor-based factory** to generate filter and measurement structs. Before including the factory headers you must define a few macros:

```c
/* Control inline semantics: use static in a single translation unit */
#define EXTERN_INLINE_MATRIX static inline
#define EXTERN_INLINE_KALMAN static inline

#include <kalman.h>

/* Define a filter named "gravity" with 3 states and 0 inputs */
#define KALMAN_NAME gravity
#define KALMAN_NUM_STATES 3
#define KALMAN_NUM_INPUTS 0
#include <kalman_factory_filter.h>

/* Define a measurement named "position" with 1 output */
#define KALMAN_MEASUREMENT_NAME position
#define KALMAN_NUM_MEASUREMENTS 1
#include <kalman_factory_measurement.h>

/* Undef all KALMAN_* macros so you can define another filter */
#include <kalman_factory_cleanup.h>

/* Now use the generated names */
kalman_t *kf = kalman_filter_gravity_init();
kalman_measurement_t *kfm = kalman_filter_gravity_measurement_position_init();

kalman_predict(kf);
kalman_correct(kf, kfm);
```

To define multiple filters, repeat the `KALMAN_NAME` / `kalman_factory_filter.h` / `kalman_factory_cleanup.h` cycle with different names.

### CMake (installable)

The project can be built, installed, and consumed via `find_package()`:

```sh
cmake -B build -S .
cmake --build build
cmake --install build --prefix /usr/local  # or any prefix
```

Downstream `CMakeLists.txt`:

```cmake
find_package(kalman_clib 1.0 REQUIRED)
target_link_libraries(your_target PRIVATE kalman_clib::kalman_clib m)
```

**Options:**

| Option | Default | Description |
|---|---|---|
| `KALMAN_CLIB_BUILD_EXAMPLES` | `ON` | Build example programs |

License & Origin

- SPDX-License-Identifier: `MIT`
- Upstream: <https://github.com/sunsided/kalman-clib>
- Copyright (c) 2014-2024 Markus Mayer — see [LICENSE.md](LICENSE.md).
