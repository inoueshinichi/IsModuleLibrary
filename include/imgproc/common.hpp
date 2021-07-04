#pragma once

/*Context*/
#include <nbla/global_context.hpp>
#include <nbla/singleton_manager.hpp>

/*NdArray*/
#include <nbla/nd_array.hpp>
#include <nbla/nd_array_extra.hpp>

#include <imgproc/defs.hpp>

#include <utils/format_string.hpp>
#include <utils/split.hpp>
#include <utils/string_join.hpp>

#include <stdexcept>
#include <exception>
#include <cassert>
#include <cmath>
#include <limits>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <cstring>
#include <string>
#include <sstream>

#include <tuple>
#include <vector>
#include <deque>
#include <list>

#define IMGPROC_CHECK_IMAGE_SHAPE(NDARRAY)                                        \
    {                                                                             \
        auto ndim = NDARRAY->ndim();                                              \
        auto shape = NDARRAY->shape();                                            \
        auto strides = NDARRAY->strides();                                        \
        auto dtype = NDARRAY->dtype();                                            \
                                                                                  \
        if (!(dtype == nbla::dtypes::FLOAT || dtype == nbla::dtypes::DOUBLE))     \
        {                                                                         \
            throw std::runtime_error(                                             \
                utils::format_string("Miss matching dtypes::FLOAT. Given is %s.", \
                                     nbla::dtype_to_string(dtype).c_str()));      \
        }                                                                         \
                                                                                  \
        if (ndim !=3)                                                             \
        {                                                                         \
            throw std::runtime_error(                                             \
                utils::format_string(                                             \
                    "Miss matching 3 ndim. Given is %d",                          \
                    ndim));                                                       \
        }                                                                         \
        else                                                                      \
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

