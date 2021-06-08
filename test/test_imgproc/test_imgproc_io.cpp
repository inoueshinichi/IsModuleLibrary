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
    TEST(imgproc_io, bmp)
    {
        using namespace std;
        using namespace Is::imgproc;
        using namespace Is::imgproc::format_policy;
        using namespace Is::nbla;
        using namespace Is::utils;
        using byte = unsigned char;
        Context ctx_cpu;

        string dummy_filename = "/home/inoue/Images/Mono/256X256/Tree.bmp";
        // string dummy_filename = "/home/inoue/Images/Mono/256X256/lenna.bmp";

        // Bmp
        ImageIo<BmpFilePolicy> io_bmp;
        auto test_ndarray = NdArray::create();

        if (!io_bmp.load(dummy_filename, ctx_cpu, test_ndarray, true))
            return;

        auto test_strides = test_ndarray->strides();
        auto test_shape = test_ndarray->shape();
        byte * data = test_ndarray->cast_data_and_get_pointer<byte>(ctx_cpu);
        std::printf("size(%ld, %ld, %ld)\n", test_shape[0], test_shape[1], test_shape[2]);

        for (int c = 0; c < test_shape[0]; ++c)
        {
            for (int y = 0; y < test_shape[1]; ++y)
            {
                for (int x = 0; x < test_shape[2]; ++x)
                {
                    if (y == x)
                        data[c * test_strides[0] + y * test_strides[1] + x * test_strides[2]] = 0;
                }
            }
        }

        // Matplotlib
        int height = test_shape[0];
        int width = test_shape[1];
        // plt::imshow(data, height, width, 1); // BMPファイルの表示
        // plt::show();

        dummy_filename = "/home/inoue/Desktop/Tree_out.bmp";
        io_bmp.save(dummy_filename, ctx_cpu, test_ndarray, true);
    }


    
}

int main(int, char **)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}