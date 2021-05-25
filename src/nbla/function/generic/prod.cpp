#include <nbla/function/prod.hpp>


namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Prod, const vector<int> &, bool)


        template <typename T>
        void Prod<T>::execute_impl_reduce(const T* x, T* y, int outer_size, int reduction_size) 
        {
            for (int o = 0; o < outer_size; ++o) 
            {
                T& val = y[o];
                val = 1;
                for (int i = 0; i < reduction_size; ++i)
                {
                    val *= x[o * reduction_size + i];
                }
            }
        }
    }
}