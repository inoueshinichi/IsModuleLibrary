#include <nbla/function/min.hpp>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Min, const vector<int>&, bool, bool, bool)
        // axes
        // keep_dims
        // with_index
        // only_index

        template <typename T>
        void Min<T>::execute_impl_reduce(const T* x, T* y, int outer_size, int reduction_size)
        {
            // Saving index is a bit inefficient if backward is not required.
            int* ind = this->index_buff_->cast_data_and_get_pointer<int>(this->ctx_, true);
            for (int o = 0; o < outer_size; ++o)
            {
                int min_index = 0;
                // T min_val = 1e+8; // minの初期値(最も大きい値) これだと、型ごとで最大値が同じなのでマズイと思う。 @inoueshinichi
                T min_val = std::numeric_limits<T>::max();

                // minを求める
                for (int i = 0; i < reduction_size; ++i) 
                {
                    const T val = x[o * reduction_size + i];
                    if (val < min_val) 
                    {
                        min_val = val;
                        min_index = i;
                    }
                }
                y[o] = min_val;
                ind[o] = min_index;
            }
        }
    }
}