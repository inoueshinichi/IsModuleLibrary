#include <nbla/array.hpp>
#include <nbla/function/add2.hpp>
#include <nbla/function/bc_add2.hpp>
#include <nbla/nd_array.hpp>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Add2, bool)

        template <typename T>
        void Add2<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            if (inputs[0]->shape() == inputs[1]->shape()) 
            {
                outputs[0]->reshape(inputs[0]->shape(), true);
                if (inplace_)
                {
                    outputs[0]->set_array(inputs[0]->array());
                }
                return;
            }

            // ブロードキャスト
            // Trying to fallback to broadcastable Add2.
            this->fall_back_func_ = create_BcAdd2(this->ctx_, inplace_);
            this->fall_back_func_->setup(inputs, outputs);
        }


        template <class T>
        void Add2<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            const T* x0 = inputs[0]->get_data_pointer<T>(this->ctx_);
            const T* x1 = inputs[1]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, !inplace_);

            for (int s = 0; s < inputs[0]->size(); s++) 
            {
                y[s] = x0[s] + x1[s]; // 要素毎の加算
            }
        }
    }
}