#include <nbla/array.hpp>
#include <nbla/function/matrix_diag.hpp>
#include <nbla/nd_array.hpp>

#include <algorithm>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(MatrixDiag)


        template <typename T>
        void MatrixDiag<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            // Create new shape
            Shape_t shape_x = inputs[0]->shape();
            Shape_t shape_y = shape_x;
            last_ndim_ = shape_x[shape_x.size() - 1];
            shape_y.push_back(last_ndim_); // use last dimension of the input shape

            // Reshape output
            outputs[0]->reshape(shape_y, true);
        }


        template <typename T>
        void MatrixDiag<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            for (int i = 0; i < inputs[0]->size(); ++i)
             {
                for (int j = 0; j < last_ndim_; ++j)
                {
                    if (i % last_ndim_ == j) 
                    {
                        y[i * last_ndim_ + j] = x[i];
                    }
                    else
                    {
                        y[i * last_ndim_ + j] = 0.;
                    }
                }
            }
        }


        
    }
}