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

```c
#include <kalman.h>
#include <kalman_factory_filter.h>

/* Use the generated filter struct and init function */
kalman_filter_gravity_init();
```

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
