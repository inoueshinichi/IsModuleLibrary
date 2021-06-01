#include <nbla/function/max.hpp>

#include <cstring>


namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Max, const vector<int>&, bool, bool, bool)


        template <typename T>
        void Max<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            Sum<T>::setup_impl(inputs, outputs);

            int outer_size = inputs[0]->size() / this->reduction_size_;
            this->index_buff_ = make_shared<NdArray>(Shape_t{outer_size});

            if (this->with_index_ && !this->only_index_)
                outputs[1]->reshape(outputs[0]->shape(), true);
        }


        template <typename T>
        void Max<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            Sum<T>::execute_impl(inputs, outputs); // 内部でthis->execute_impl_reduce()が実行される.

            // Indexを取得する場合
            if (this->with_index_ || this->only_index_) 
            {
                auto idx_sp_arr = this->only_index_ ? outputs[0] : outputs[1];
                Array* idx_arr = idx_sp_arr->cast(get_dtype<size_t>(), this->ctx_, true);
                Array* idx_buf = this->index_buff_->get(get_dtype<int>(), this->ctx_);

                idx_arr->copy_from(idx_buf);
            }
        }


        template <typename T>
        void Max<T>::execute_impl_reduce(const T *x, T *y, int outer_size, int reduction_size)
        {
            // Saving index is a bit inefficient if backward is not required.
            int* ind = this->index_buff_->cast_data_and_get_pointer<int>(this->ctx_, true);
            for (int o = 0; o < outer_size; ++o)
            {
                int max_index = 0;
                // T max_val = -1e+8; // maxの初期値(最も小さい値) これだと、型ごとで最小値が同じなのでマズイと思う。 @inoueshinichi
                T max_val = std::numeric_limits<T>::min();

                // maxを求める
                for (int i = 0; i < reduction_size; ++i) 
                {
                    const T val = x[o * reduction_size + i];
                    if (val > max_val) 
                    {
                        max_val = val;
                        max_index = i;
                    }
                }
                y[o] = max_val;
                ind[o] = max_index;
            }
        }


    }
}