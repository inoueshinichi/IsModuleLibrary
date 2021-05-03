#include <nbla/array.hpp>
#include <nbla/function/broadcast.hpp>
#include <nbla/nd_array.hpp>


#include <cstring>
#include <iostream>


namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Broadcast, const vector<int>&);

        template <typename T>
        void Broadcast<T>::setup_impl(const NdArrays& inputs,
                                      const NdArrays& outputs)
        {
            // 入力Tensor
            auto inshape = inputs[0]->shape();
            auto ndim = inputs[0]->ndim();

            // 動的な入力なのでチェック
            if (ndim > 0)
            {
                NBLA_CHECK(shape_.size() == static_cast<unsigned>(ndim),
                           error_code::value,
                           "Number of dimension must match. "
                           "Shape: %d != input: %d.",
                           shape_.size(), ndim);
            }

            // X stride and Y shape.
            stride_x_.reshape({ ndim }, true);
            shape_y_.reshape({ ndim }, true);
            Context cpu = Context().set_array_class("CpuCachedArray");
            int* stride_x = stride_x_.cast_data_and_get_pointer<int>(cpu, true);
            int* shape_y = shape_y_.cast_data_and_get_pointer<int>(cpu, true);
            auto stride_x_in = inputs[0]->strides();

            // Check shape, and store ndarrays.
            for (int d = 0, d < ndim; ++d)
            {
                shape_y[d] = shape_[d];
                if (inshape[d] == shape_[d])
                {
                    stride_x[d] = stride_x_in[d];
                    continue;
                }

                NBLA_CHECK(inshape[d] == 1, 
                           error_code::value,
                           "Size of a dimension broadcasted must be "
                           "1 (%d at the dimension %d).",
                           inshape[d], d);
            }

            Shape_t outshape(shape_.begin(), shape_.end());
            outputs[0]->reshape(outshape, true);
        }