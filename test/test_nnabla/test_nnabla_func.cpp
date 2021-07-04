// googletest
#include <gtest/gtest.h>

// nnabla core
#include <nbla/context.hpp>
#include <nbla/global_context.hpp>
#include <nbla/nd_array_extra.hpp>

// nnabla func
#include <nbla/function/add_scalar.hpp>
#include <nbla/function/randn.hpp>

// utility
#include <test_utils.hpp>

#include <iostream>
#include <string>
#include <cstdio>
#include <iomanip>
#include <cstring>

using namespace Is::nbla;

namespace
{
    TEST(nnabla_func, rand)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        float low = 0.0;
        float high = 100;
        int seed = 2021;
        Shape_t shape = {3, 5, 5};

        auto rand_ndarray = rand<float>(shape, low, high, seed);
        show_ndarray_contents<float>(rand_ndarray);
    }

    TEST(nnabla_func, randint)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        int low = 0;
        int high = 100;
        int seed = 2021;
        Shape_t shape = {3, 5, 5};

        auto randint_ndarray = randint<int>(shape, low, high, seed);

        show_ndarray_contents<int>(randint_ndarray);
    }

    TEST(nnabla_func, randn)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        float mu = 0.0;
        float sigma = 1.0;
        int seed = 2021;
        Shape_t shape = {3, 5, 5};
        auto ndarray_out_d = randn<double>(shape, mu, sigma, seed);
        show_ndarray_contents<double>(ndarray_out_d);
        std::cout << ndarray_out_d->get_data_pointer<double>(ctx_cpu)[0] << std::endl;

        auto ndarray_out_f = randn<float>(shape, mu, sigma, seed);
        show_ndarray_contents<float>(ndarray_out_f);
        std::cout << ndarray_out_f->get_data_pointer<float>(ctx_cpu)[0] << std::endl;
    }

    // --------------------------------------------------------------------------------------

    TEST(nnabla_func, reshape)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // ones
        auto ndarray_ones = ones<float>(Shape_t{3, 5, 5});
        float *data = ndarray_ones->cast_data_and_get_pointer<float>(ctx_cpu);
        auto shape = ndarray_ones->shape();
        auto strides = ndarray_ones->strides();
        data[0] = 0.0;
        data[1 * strides[0] + 1 * strides[1] + 1 * strides[2]] = 0.0;
        data[2 * strides[0] + 2 * strides[1] + 2 * strides[2]] = 0.0;
        show_ndarray_contents<float>(ndarray_ones);

        auto out_ndarray = reshape<float>(ndarray_ones, Shape_t{3, 25});
        auto out_shape = out_ndarray->shape();
        show_ndarray_contents<float>(out_ndarray);
        std::printf("out: size:%ld (%ld, %ld)\n", out_shape.size(), out_shape[0], out_shape[1]);
    }

    TEST(nnabla_func, transpose)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // ones
        auto ndarray_ones = ones<float>(Shape_t{3, 5, 5});
        auto shape = ndarray_ones->shape();
        auto strides = ndarray_ones->strides();
        float *data = ndarray_ones->cast_data_and_get_pointer<float>(ctx_cpu);
        for (int c = 0; c < shape[0]; ++c)
        {
            for (int y = 0; y < shape[1]; ++y)
            {
                for (int x = 0; x < shape[2]; ++x)
                {
                    data[c * strides[0] + y * strides[1] + x * strides[2]] = c;
                }
            }
        }
        std::printf("in-shape: (%ld, %ld, %ld)\n", shape[0], shape[1], shape[2]);
        show_ndarray_contents<float>(ndarray_ones);

        auto out_ndarray = transpose<float>(ndarray_ones, {1, 2, 0});
        auto out_shape = out_ndarray->shape();
        show_ndarray_contents<float>(out_ndarray);

        std::cout << "transpose" << std::endl;
        std::printf("out-shape: (%ld, %ld, %ld)\n", out_shape[0], out_shape[1], out_shape[2]);
    }

    TEST(nnabla_func, slice)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        auto ndarray_in = ones<float>(Shape_t{3, 16, 16});
        auto shape_in = ndarray_in->shape();
        auto strides_in = ndarray_in->strides();
        float *data_in = ndarray_in->cast_data_and_get_pointer<float>(ctx_cpu);
        std::printf("in: size:%ld (%ld, %ld, %ld)\n", shape_in.size(), shape_in[0], shape_in[1], shape_in[2]);
        for (int c = 0; c < 1; ++c)
        {
            for (int y = 5; y < 10; ++y)
            {
                for (int x = 5; x < 10; ++x)
                {
                    data_in[c * strides_in[0] + y * strides_in[1] + x * strides_in[2]] = 5.0;
                }
            }
        }
        show_ndarray_contents<float>(ndarray_in);

        auto ndarray_out = slice<float>(ndarray_in, {0, 4, 4}, {2, 11, 11}, {1, 1, 1});
        auto shape_out = ndarray_out->shape();
        auto strides_out = ndarray_out->strides();
        float *data_out = ndarray_out->cast_data_and_get_pointer<float>(ctx_cpu);
        std::printf("slice: size:%ld (%ld, %ld, %ld)\n", shape_out.size(), shape_out[0], shape_out[1], shape_out[2]);
        show_ndarray_contents<float>(ndarray_out);
    }

    TEST(nnabla_func, broadcast)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // // input-1
        // auto ndarray_in1 = ones<float>(Shape_t{3, 5, 5});
        // auto shape_in1 = ndarray_in1->shape();
        // auto strides_in1 = ndarray_in1->strides();
        // float *data_in1 = ndarray_in1->cast_data_and_get_pointer<float>(ctx_cpu);
        // for (int c = 0; c < shape_in1[0]; ++c)
        // {
        //     for (int y = 0; y < shape_in1[1]; ++y)
        //     {
        //         for (int x = 0; x < shape_in1[2]; ++x)
        //         {
        //             data_in1[c * strides_in1[0] + y * strides_in1[1] + x * strides_in1[2]] = c;
        //         }
        //     }
        // }
        // std::printf("in1-shape: (%ld, %ld, %ld)\n", shape_in1[0], shape_in1[1], shape_in1[2]);
        // show_ndarray_contents<float>(ndarray_in1);

        // input-2
        auto ndarray_in2 = ones<float>(Shape_t{1, 5, 5});
        auto shape_in2 = ndarray_in2->shape();
        std::printf("in2-shape: (%ld, %ld, %ld)\n", shape_in2[0], shape_in2[1], shape_in2[2]);
        show_ndarray_contents<float>(ndarray_in2);

        // broadcasetはshapeで与えたTensorサイズに拡張する.
        auto out_ndarray = broadcast<float>(ndarray_in2, {3, 5, 5});
        auto out_shape = out_ndarray->shape();
        std::printf("out: size:%ld (%ld, %ld, %ld)\n", out_shape.size(), out_shape[0], out_shape[1], out_shape[2]);
        show_ndarray_contents<float>(out_ndarray);
    }

    // --------------------------------------------------------------------------------------

    TEST(nnabla_func, sum)
    {
        const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        // input
        auto ndarray_in = ones<float>(Shape_t{3, 5, 5});
        show_ndarray_contents<float>(ndarray_in);

        // sum with axis=0
        auto ndarray_out1 = sum<float>(ndarray_in, 0);
        auto shape_out1 = ndarray_out1->shape();
        std::printf("sumed: size:%ld (%ld, %ld)\n", shape_out1.size(), shape_out1[0], shape_out1[1]);
        show_ndarray_contents<float>(ndarray_out1);

        // sum with axis=1
        auto ndarray_out2 = sum<float>(ndarray_in, 1);
        auto shape_out2 = ndarray_out2->shape();
        std::printf("sumed: size:%ld (%ld, %ld)\n", shape_out2.size(), shape_out2[0], shape_out2[1]);
        show_ndarray_contents<float>(ndarray_out2);

        // sum with axis=2
        auto ndarray_out3 = sum<float>(ndarray_in, 2);
        auto shape_out3 = ndarray_out3->shape();
        std::printf("sumed: size:%ld (%ld, %ld)\n", shape_out3.size(), shape_out3[0], shape_out3[1]);
        show_ndarray_contents<float>(ndarray_out3);

        // sum with axis=2 and keep_dims = true
        auto ndarray_out4 = sum<float>(ndarray_in, 2, true);
        auto shape_out4 = ndarray_out4->shape();
        std::printf("sumed: size:%ld (%ld, %ld, %ld)\n", shape_out4.size(), shape_out4[0], shape_out4[1], shape_out4[2]);
        show_ndarray_contents<float>(ndarray_out4);
    }

    // TEST(nnabla_func, add_scalar)
    // {
    //     using namespace Is::nbla;
    //     string device_id{"cpu"};
    //     const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

    //     // zero
    //     auto ndarray_zeros = zeros<float>(Shape_t{3, 32, 32});
    //     show_ndarray_contents<float>(ndarray_zeros);

    //     auto out_ndarray = add_scalar<float>(ndarray_zeros, 25.5); // 80ms (3, 2000, 2000)

    //     // 118ms (3, 2000, 2000)
    //     // auto shape = ndarray_zeros->shape();
    //     // auto strides = ndarray_zeros->strides();
    //     // float* data = ndarray_zeros->cast_data_and_get_pointer<float>(ctx_cpu);
    //     // for (int c = 0; c < shape[0]; ++c)
    //     // {
    //     //     for (int y = 0; y < shape[1]; ++y)
    //     //     {
    //     //         for (int x = 0; x < shape[2]; ++x)
    //     //         {
    //     //             data[c * strides[0] + y * strides[1] + x * strides[2]] = 25.5;
    //     //         }
    //     //     }
    //     // }

    //     show_ndarray_contents<float>(out_ndarray);
    // }


    // TEST(nnabla_func, mul_scalar)
    // {
    //     using namespace Is::nbla;
    //     string device_id{"cpu"};
    //     const auto &ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

    //     // ones
    //     auto ndarray_ones = ones<float>(Shape_t{3, 256, 256});
    //     // show_ndarray_contents<float>(ndarray_ones);

    //     auto out_ndarray = mul_scalar<float>(ndarray_ones, 63.2);
    //     // show_ndarray_contents<float>(out_ndarray);
    // }


    // TEST(dummy, naive_mul)
    // {
    //     int channels = 3;
    //     int height = 256;
    //     int width = 256;
    //     size_t datasize = sizeof(float) * channels * height * width;
    //     float* data = new float[datasize];
    //     std::memset(data, 1, datasize);

    //     for (int c = 0; c < channels; ++c)
    //     {
    //         for (int y = 0; y < height; ++y)
    //         {
    //             for (int x = 0; x < width; ++x)
    //             {
    //                 data[c * channels + y * height + x * width] *= 55.5;
    //             }
    //         }
    //     }

    //     delete[] data;
    // }


    


    


    

    


    


    
}


int main(int, char**)
{
    std::cout << "TEST nnabla func" << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}