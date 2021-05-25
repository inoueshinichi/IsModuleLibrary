#pragma once

#include <nbla/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace Is
{
    namespace nbla
    {
        /**
         * @brief Elementwise mul scalar 
         * y_i = x_i * v
         * 
         * Inputs:
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         * 
         * @param T Data type for computation.
         * @param val Value of the scalar.
         */
        NBLA_DEFINE_TRANSFORM_UNARY_1(MulScalar, x * (T)a0, double)
    }
}