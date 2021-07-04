#include <nbla/array.hpp>
#include <nbla/function/constant.hpp>
#include <nbla/nd_array.hpp>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Constant, float, const vector<int>&)
        // value
        // shape

        template <typename T>
        void Constant<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            Shape_t out_shape(shape_.begin(), shape_.end());
            outputs[0]->reshape(out_shape, true);
        }


        template <typename T>
        void Constant<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            if (val_ == 0) 
            {
                outputs[0]->zero();
                return;
            }
            outputs[0]->fill(val_);
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, char)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, unsigned char)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, short)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, unsigned short)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, int)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, unsigned int)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, unsigned long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, long long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, unsigned long long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, float)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, double)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Constant, long double)
    }
}