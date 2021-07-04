#include <nbla/array.hpp>
#include <nbla/function/matrix_diag_part.hpp>
#include <nbla/nd_array.hpp>

#include <algorithm>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(MatrixDiagPart)


        template <typename T>
        void MatrixDiagPart<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            // Check shape
            Shape_t shape_x = inputs[0]->shape();
            NBLA_CHECK(shape_x.size() > 1, 
                       error_code::value,
                       "Input dimensions must be greater than 1.");
            
            NBLA_CHECK(shape_x[shape_x.size() - 2] == shape_x[shape_x.size() - 1], // バグを修正 @inoueshinichi
                       error_code::value,
                       "Last and second last dimensions must be the same.");
            
            last_ndim_ = shape_x[shape_x.size() - 1];

            // Create new shape and compute part size
            Shape_t shape_y;
            for (Shape_t::size_type i = 0; i < shape_x.size() - 1; ++i) 
            {
                shape_y.push_back(shape_x[i]);
            }

            // Reshape output
            outputs[0]->reshape(shape_y, true);
        }


        template <typename T>
        void MatrixDiagPart<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            Size_t size = outputs[0]->size();

            for (int i = 0; i < size; ++i) 
            {
                y[i] = x[i * last_ndim_ + i % last_ndim_];
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, char)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, unsigned char)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, short)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, unsigned short)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, int)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, unsigned int)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, unsigned long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, long long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, unsigned long long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, float)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, double)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, MatrixDiagPart, long double)
    }
}