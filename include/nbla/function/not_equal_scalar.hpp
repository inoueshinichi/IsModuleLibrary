#pramga once

#include <nbla/function/utils/base_transform_unary.hpp>

#include <cmath>


namespace Is
{
    namespace nbla
    {
        /**
         * @brief Taking elementwise binary test of `not equal` with scalar.
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
        NBLA_DEFINE_TRANSFORM_UNARY_1(NotEqualScalar, x != (T)a0, double)
    }
}