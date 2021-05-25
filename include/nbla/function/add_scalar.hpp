#pragma once

#include <nbla/function/utils/base_transform_unary.hpp>

namespace Is
{
    namespace nbla
    {
        /**
         * @brief Elementwise add scalar
         * 
         * y_i = x_i + v
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
        NBLA_DEFINE_TRANSFORM_UNARY_1_INPLACE(AddScalar, x + (T)a0, double)
    }
}