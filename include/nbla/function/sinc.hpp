#pragma once

#include <nbla/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace Is
{
    namespace nbla
    {
        // Sinc関数
        NBLA_DEFINE_TRANSFORM_UNARY(Sinc, (x == (T)0 ? (T)1 : std::sin(x) / x))
    }
}