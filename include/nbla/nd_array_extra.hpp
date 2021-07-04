#pragma once

#include <nbla/nd_array.hpp>

// Generation
#include <nbla/function/rand.hpp>
#include <nbla/function/randint.hpp>
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
        NdArrayPtr zeros(const Shape_t& shape)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto ndarray = NdArray::create(shape);
            ndarray->zero();
            ndarray->cast_data_and_get_pointer<T>(ctx);
            return ndarray;
        }


        // ones
        template <typename T>
        NdArrayPtr ones(const Shape_t& shape)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto ndarray = NdArray::create(shape);
            ndarray->fill(1);
            ndarray->cast_data_and_get_pointer<T>(ctx);
            return ndarray;
        }


        // eye
        template <typename T>
        NdArrayPtr eye(int rows, int cols)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
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
        NdArrayPtr randn(const Shape_t& shape, float mu, float sigma, int seed = -1)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Randn<F> operation(ctx, mu, sigma, shape, seed);
            auto output = zeros<F>(shape);
            operation.setup({}, {output});
            operation.execute({}, {output});
            return output;
        }


        // rand
        template <typename F>
        NdArrayPtr rand(const Shape_t& shape, float low, float high, int seed = -1)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Rand<F> operation(ctx, low, high, shape, seed);
            auto output = zeros<F>(shape);
            operation.setup({}, {output});
            operation.execute({}, {output});
            return output;
        }

        // randint
        template <typename T>
        NdArrayPtr randint(const Shape_t &shape, int low, int high, int seed = -1)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Randint<T> operation(ctx, low, high, shape, seed);
            auto output = zeros<T>(shape);
            operation.setup({}, {output});
            operation.execute({}, {output});
            return output;
        }

        // -------------------------------------------------------

        // add_scalar
        template <typename T>
        NdArrayPtr add_scalar(NdArrayPtr input, double val)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            AddScalar<T> operation(ctx, val, true);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return input;
        }


        // mul_scalar
        template <typename T>
        NdArrayPtr mul_scalar(NdArrayPtr input, double val)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            MulScalar<T> operation(ctx, val);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        
        // transpose
        template <typename T>
        NdArrayPtr transpose(NdArrayPtr input, const std::vector<int64_t> &axes)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Transpose<T> operation(ctx, axes);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // broadcast
        template <typename T>
        NdArrayPtr broadcast(NdArrayPtr input, const Shape_t& shape)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Broadcast<T> operation(ctx, shape);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // reshape
        template <typename T>
        NdArrayPtr reshape(NdArrayPtr input, const std::vector<int64_t> &shape)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Reshape<T> operation(ctx, shape, false);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // slice
        template <typename T>
        NdArrayPtr slice(NdArrayPtr input, const vector<int64_t> &starts, 
                         const vector<int64_t> &stops, const vector<int64_t> &steps)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Slice<T> operation(ctx, starts, stops, steps);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // sum
        template <typename T>
        NdArrayPtr sum(NdArrayPtr input, int axis = 0, bool keep_dims = false)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Sum<T> operation(ctx, {axis}, keep_dims);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }
    }
}