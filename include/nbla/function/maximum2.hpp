#pragma once

#include <nbla/function/utils/base_transform_binary.hpp>

namespace Is
{
    namespace nbla
    {
        /**
         * @brief Elementwise maximum
         * y_i = max(x0_i, x1_i)
         * 
         * Inputs:
         * - N-D array.
         * - N-D array.
         * 
         * Outputs:
         * - N-D array.
         * 
         * @param T Data type for computation.
         */
        NBLA_DEFINE_TRANSFORM_BINARY(Maximum2, (x0 > x1) ? x0 : x1)
    }
}