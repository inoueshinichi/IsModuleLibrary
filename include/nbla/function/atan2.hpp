#pragma once

#include <nbla/function/utils/base_transform_binary.hpp>

#include <cmath>

namespace Is
{
    namespace nbla
    {
        NBLA_DEFINE_TRANSFORM_BINARY(ATan2, std::atan2(x0, x1));
    }
}