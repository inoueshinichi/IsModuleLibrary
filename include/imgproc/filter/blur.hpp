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

        /**
         * @brief 平滑化フィルタ
         * 
         */
        IMGPROC_API NdArrayPtr blur(const Context &ctx, NdArrayPtr src, int fsize);
    }
}