#pragma once

#include <nbla/nd_array.hpp>

// Generation
#include <nbla/function/randn.hpp>

// Operation
#include <nbla/function/add_scalar.hpp>
#include <nbla/function/mul_scalar.hpp>

#include <nbla/function/transpose.hpp>
#include <nbla/function/sum.hpp>
#include <nbla/function/broadcast.hpp>
#include <nbla/function/reshape.hpp>
#include <nbla/function/slice.hpp>

#include <memory>


namespace Is
{
    namespace nbla
    {
        // -------------------------------------------------------

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
            Randn<F> operation(ctx, mu, sigma, shape, seed);
            auto output = NdArray::create();
            operation.setup({}, {output});
            operation.execute({}, {output});
            return output;
        }



        // -------------------------------------------------------

        // add_scalar
        template <typename T>
        NdArrayPtr add_scalar(const Context& ctx, NdArrayPtr input, double val)
        {
            AddScalar<T> operation(ctx, val, true);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return input;
        }


        // mul_scalar
        template <typename T>
        NdArrayPtr mul_scalar(const Context& ctx, NdArrayPtr input, double val)
        {
            MulScalar<T> operation(ctx, val);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        
        // transpose
        template <typename T>
        NdArrayPtr transpose(const Context &ctx, NdArrayPtr input, const std::vector<int> &axes)
        {
            Transpose<T> operation(ctx, axes);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // sum
        template <typename T>
        NdArrayPtr sum(const Context &ctx, NdArrayPtr input, int axis = 0, bool keep_dims = false)
        {
            Sum<T> operation(ctx, {axis}, keep_dims);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // broadcast
        template <typename T>
        NdArrayPtr broadcast(const Context& ctx, NdArrayPtr input, NdArrayPtr output)
        {
            Broadcast<T> operation(ctx, input->shape());
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // reshape
        template <typename T>
        NdArrayPtr reshape(const Context &ctx, NdArrayPtr input, const std::vector<int64_t> &shape)
        {
            Reshape<T> operation(ctx, shape, false);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // slice
        template <typename T>
        NdArrayPtr slice(const Context &ctx, NdArrayPtr input, 
            const vector<int> &starts, const vector<int> &stops, const vector<int> &steps)
        {
            Slice<T> operation(ctx, starts, stops, steps);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }
    }
}