#pragma once

#include <nbla/function/utils/base_transform_unary.hpp>

#include <cmath>

namespace Is
{
    namespace nbla
    {
        NBLA_DEFINE_TRANSFORM_UNARY(Sin, std::sin(x));
    }
}