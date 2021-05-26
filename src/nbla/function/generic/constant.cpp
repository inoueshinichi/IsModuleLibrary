#include <nbla/array.hpp>
#include <nbla/function/constant.hpp>
#include <nbla/nd_array.hpp>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Constant, double, const vector<int64_t>&)
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
        

    }
}