#include <nbla/array.hpp>
#include <nbla/common.hpp>
#include <nbla/function/arange.hpp>
#include <nbla/nd_array.hpp>

#include <limits>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Arange, double, double, double)
        // start
        // stop
        // step

        template <typename T>
        void Arange<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            NBLA_CHECK(this->step_ != 0, 
                       error_code::value,
                       "step argument must not be zero");

            Size_t count = 0;
            if (step_ < 0) 
            {
                for (double value = start_; value > stop_; value += step_)
                {
                    count++;
                }
            } 
            else 
            {
                for (double value = start_; value < stop_; value += step_) 
                {
                    count++;
                }
            }

            outputs[0]->reshape(Shape_t{count}, true);
        }


        template <typename T>
        void Arange<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            NdArray& y = *outputs[0];
            auto y_data = y.cast_data_and_get_pointer<T>(this->ctx_, true);
            auto value = this->start_;

            for (Size_t i = 0; i < y.size(); ++i) 
            {
                y_data[i] = value;
                value += this->step_;
            }
        }

        /**
         * @brief テンプレートの明示的インスタンス化
         * nbla.dllの外部にエクスポートする.
         */
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, char)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, unsigned char)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, short)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, unsigned short)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, int)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, unsigned int)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, unsigned long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, long long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, unsigned long long)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, float)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, double)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Arange, long double)
    }
}