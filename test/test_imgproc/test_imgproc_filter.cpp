#include <gtest/gtest.h>

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include <utils/time_measure.hpp>

// imgproc
#include <imgproc/common.hpp>
#include <imgproc/io/img_io.hpp>

// filter
#include <imgproc/filter/utils_filter.hpp>
#include <imgproc/filter/average_filter.hpp>

// utility
#include <test_utils.hpp>

#include <string>
#include <cstdio>

using uchar = unsigned char;
using namespace Is::nbla;
using namespace Is::utils;
using namespace Is::imgproc;

namespace 
{
    // TEST(imgproc_func, padding)
    // {
    //     const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();

    //     // src
    //     auto ndarray_ones = ones<float>(Shape_t{3, 16, 16});
    //     auto in_ndim = ndarray_ones->ndim();
    //     auto in_shape = ndarray_ones->shape();
    //     auto in_strides = ndarray_ones->strides();
    //     std::printf("in-shape: %ld (%ld,%ld,%ld)\n", in_shape.size(), in_shape[0], in_shape[1], in_shape[2]);
    //     std::printf("in-strides: %ld (%ld,%ld,%ld)\n", in_strides.size(), in_strides[0], in_strides[1], in_strides[2]);
    //     float *data = ndarray_ones->cast_data_and_get_pointer<float>(ctx);

    //     for (int c = 0; c < in_shape[0]; ++c)
    //     {
    //         for (int y = 0; y < in_shape[1]; ++y)
    //         {
    //             for (int x = 0; x < in_shape[2]; ++x)
    //             {
    //                 // 縞模様
    //                 for (int s = 1; s < 5; ++s)
    //                 {
    //                     if (x == s || y == s || x == in_shape[2] - s || y == in_shape[1] - s)
    //                         data[c * in_strides[0] + y * in_strides[1] + x * in_strides[2]] = s;
    //                 }
    //             }
    //         }
    //     }
    //     show_ndarray_contents<float>(ndarray_ones);

    //     auto ndarray_padding = padding<float>(ndarray_ones, 5, 5, "reflect");  // ダンプ
    //     show_ndarray_contents<float>(ndarray_padding);
    // }

    TEST(imgproc_func, average_filter)
    {
        const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        std::cout << "GlobalContext after load: " << ctx.to_string() << std::endl;

        string dummy_filename = "/home/inoue/Images/Mono/256X256/Tree.bmp";
       
        // Bmp
        ImageIo<format_policy::BmpFile> io_bmp;
        auto test_ndarray = NdArray::create();

        if (!io_bmp.load(dummy_filename, test_ndarray, true))
            return;

        auto test_strides = test_ndarray->strides();
        auto test_shape = test_ndarray->shape();

        float *data = test_ndarray->cast_data_and_get_pointer<float>(ctx);
        std::printf("size(%ld, %ld, %ld)\n", test_shape[0], test_shape[1], test_shape[2]);

        // 自作のaverage_filter関数
        auto output_ndarray = invoke_tm_ns_chrono_ret(
            average_filter, test_ndarray, 5, 5);

        dummy_filename = "/home/inoue/Desktop/average_filter_tree_out.bmp";
        io_bmp.save(dummy_filename, output_ndarray, true);
    }
}

int main(int, char**)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}