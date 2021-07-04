#pragma once

#include <imgproc/common.hpp>

namespace Is
{
    namespace imgproc
    {
        using namespace nbla;

        /**
         * @brief 平滑化ファルタ
         * 
         * @param ctx 
         * @param src 
         * @param fsize 
         * @return IMGPROC_API 
         */
        IMGPROC_API NdArrayPtr average_filter(NdArrayPtr src, int ksize_x, int ksize_y);
    }
}