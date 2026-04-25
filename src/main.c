// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2014-2023 Markus Mayer
//
// Part of kalman-clib — https://github.com/sunsided/kalman-clib
// Licensed under the MIT License. See LICENSE.md in the project root for details.

#include "matrix_unittests.h"
#include "kalman_example_gravity.h"

/**
* \brief Main entry point
*/
int main(void)
{
    matrix_unittests();

    kalman_gravity_demo();
    kalman_gravity_demo_lambda();

    return 0;
}
