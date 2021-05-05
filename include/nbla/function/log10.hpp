#pragma once

#include <nbla/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace Is
{
    namespace nbla
    {
        NBLA_DEFINE_TRANSFORM_UNARY(Log10, std::log10(x))
    }
}