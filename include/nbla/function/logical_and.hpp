#pragma once

#include <nbla/function/utils/base_transform_binary.hpp>

#include <cmath>

namespace Is
{
    namespace nbla
    {
        NBLA_DEFINE_TRANSFORM_BINARY(LogicalAnd, (bool(x0) & bool(x1)))
    }
}