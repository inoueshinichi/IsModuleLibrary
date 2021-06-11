#include <gtest/gtest.h>

#include "matplotlibcpp.h"
namespace plt = matplotlibcpp;

#include <utils/time_measure.hpp>

#include <imgproc/io/img_io.hpp>
#include <imgproc/filter/average_filter.hpp>

#include <nbla/nd_array.hpp>
#include <nbla/nd_array_extra.hpp>

#include <string>
#include <cstdio>

namespace 
{
    TEST(imgproc_func, average_filter)
    {
        using namespace std;
        using namespace Is::imgproc;
        using namespace Is::imgproc::format_policy;
        using namespace Is::nbla;
        using namespace Is::utils;
        using byte = unsigned char;
        Context ctx_cpu;

        string dummy_filename = "/home/inoue/Images/Mono/256X256/Tree.bmp";

        // Bmp
        ImageIo<BmpFile> io_bmp;
        auto test_ndarray = NdArray::create();

        if (!io_bmp.load(dummy_filename, ctx_cpu, test_ndarray, true))
            return;

        auto test_strides = test_ndarray->strides();
        auto test_shape = test_ndarray->shape();
        byte *data = test_ndarray->cast_data_and_get_pointer<byte>(ctx_cpu);
        std::printf("size(%ld, %ld, %ld)\n", test_shape[0], test_shape[1], test_shape[2]);

        // 自作のaverage_filter関数
        auto output_ndarray = invoke_tm_ns_chrono_ret(
            average_filter, ctx_cpu, test_ndarray, 5, 5);

        dummy_filename = "/home/inoue/Desktop/Tree_out.bmp";
        io_bmp.save(dummy_filename, ctx_cpu, output_ndarray, true);
    }
}

int main(int, char**)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}