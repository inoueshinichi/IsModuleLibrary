#pragma once

#include <nbla/nd_array.hpp>

#include <nbla/function/randn.hpp>

#include <memory>


namespace Is
{
    namespace nbla
    {
        // zeros
        template <typename T>
        NdArrayPtr zeros(const Context& ctx, const Shape_t& shape)
        {
            auto ndarray = NdArray::create(shape);
            ndarray->zero();
            ndarray->cast_data_and_get_pointer<T>(ctx);
            return ndarray;
        }


        // ones
        template <typename T>
        NdArrayPtr ones(const Context& ctx, const Shape_t& shape)
        {
            auto ndarray = NdArray::create(shape);
            ndarray->fill(1);
            ndarray->cast_data_and_get_pointer<T>(ctx);
            return ndarray;
        }


        // eye
        template <typename T>
        NdArrayPtr eye(const Context& ctx, int rows, int cols)
        {
            Shape_t shape{rows, cols};
            auto ndarray = NdArray::create(shape);
            ndarray->zero();
            T* data = ndarray->cast_data_and_get_pointer<T>(ctx);

            Stride_t strides = ndarray->strides();
            int min_len = std::min(rows, cols);
            for (int i = 0; i < min_len; ++i)
            {
                data[i * strides[0] + i * strides[1]] = (T)1;
            }
            return ndarray;
        }

        
        // randn
        template <typename F>
        NdArrayPtr randn(const Context& ctx, const Shape_t& shape, float mu, float sigma, int seed = -1)
        {
            auto ptr_randn = Randn<F>(ctx, mu, sigma, shape, seed).copy();
            auto ndarray_out = NdArray::create();
            NdArrays inputs;
            NdArrays outputs{ ndarray_out };
            ptr_randn->setup(inputs, outputs);
            ptr_randn->execute(inputs, outputs);
            return outputs[0];
        }


        // // add_scalar
        // template <typename T>
        // NdArrayPtr add_scalar(const Context& ctx, NdArrayPtr input, double val)
        // {
        //     AddScalar<float> ptr_add_scalar(ctx_cpu, val, false);
        //     auto output = NdArray::create();
        //     ptr_add_scalar.setup({input}, {output});
        //     ptr_add_scalar.execute({input}, {output});
        //     return output;
        // }

    }
}