#pragma once

#include <imgproc/common.hpp>

/*NdArray*/
#include <nbla/nd_array.hpp>
#include <nbla/nd_array_extra.hpp>

namespace Is
{
    namespace imgproc
    {
        using namespace nbla;

        IMGPROC_API NdArrayPtr median(const Context& ctx, NdArrayPtr src, int ksize_x, int ksize_y);
    }
}