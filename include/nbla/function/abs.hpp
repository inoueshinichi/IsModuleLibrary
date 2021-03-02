#ifndef IS_NBLA_FUNCTION_ABS_HPP
#define IS_NBLA_FUNCTION_ABS_HPP

#include "nbla/function/utils/base_transform_unary.hpp"

namespace Is
{
    namespace nbla
    {
        NBLA_DEFINE_TRANSFORM_UNARY_NO_GRAD(Abs, std::abs(x));
    }
}

#endif