#include <imgproc/filter/blur.hpp>

/*NdArray*/
#include <nbla/nd_array.hpp>
#include <nbla/nd_array_extra.hpp>

#include <utils/format_string.hpp>
#include <utils/split.hpp>
#include <utils/string_join.hpp>

#include <cmath>
#include <limits>
#include <numeric>
#include <algorithm>

#include <stdexcept>
#include <exception>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#define CHECK_IMAGE_SHAPE(NDARRAY)                                                \
    using byte = unsigned char;                                                   \
    {                                                                             \
        auto ndim = NDARRAY->ndim();                                              \
        auto shape = NDARRAY->shape();                                            \
        auto strides = NDARRAY->strides();                                        \
        if (ndim ==1 || ndim > 3)                                                 \
        {                                                                         \
            throw std::runtime_error(                                             \
                utils::format_string(                                             \
                    "Miss matching 2 or 3 ndim. Given is %d",                     \
                    ndim));                                                       \
        }                                                                         \
        if (ndim == 3)                                                            \
        {                                                                         \
            /*(C, H, W)*/                                                         \
            int channels = shape[0];                                              \
            if (channels > 3)                                                     \
            {                                                                     \
                throw std::runtime_error(                                         \
                    utils::format_string(                                         \
                        "Miss matching channels of top shape (<=3). Given is %d", \
                        channels));                                               \
            }                                                                     \
        }                                                                         \
    }

namespace Is
{
    namespace imgproc
    {
        NdArrayPtr blur(const Context& ctx, NdArrayPtr src, int fsize)
        {
            CHECK_IMAGE_SHAPE(src);

            auto ndim = src->ndim();
            auto shape = src->shape();
            auto strides = src->strides();
            byte *data = src->cast_data_and_get_pointer<byte>(ctx);

            std::string msg = utils::format_string(
                "ndim: %d, size of shape: %d, size of strides: %d",
                ndim,
                shape.size(),
                strides.size()
            );
            std::cout << msg << std::endl;

            int channels = shape[0];
            int height = shape[1];
            int width = shape[2];

            for (int c = 0; c < channels; ++c)
            {
                for (int y = 0; y < height; ++y)
                {
                    for (int x = 0; x < width; ++x)
                    {
                        if (x >= 50 && x < 100)
                            data[c * strides[0] + y * strides[1] + x * strides[2]] = 0; // テスト
                    }
                }
            }

            return src;
        }
    }
}