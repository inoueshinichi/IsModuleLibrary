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

#define NBLA_INSTANTIATE_IMPL(API, TYPE) \
    template API void Prod<TYPE>::execute_impl_reduce(const TYPE *x, TYPE *y, int outer_size, int reduction_size);

        NBLA_INSTANTIATE_IMPL(NBLA_API, char)
        NBLA_INSTANTIATE_IMPL(NBLA_API, unsigned char)
        NBLA_INSTANTIATE_IMPL(NBLA_API, short)
        NBLA_INSTANTIATE_IMPL(NBLA_API, unsigned short)
        NBLA_INSTANTIATE_IMPL(NBLA_API, int)
        NBLA_INSTANTIATE_IMPL(NBLA_API, unsigned int)
        NBLA_INSTANTIATE_IMPL(NBLA_API, long)
        NBLA_INSTANTIATE_IMPL(NBLA_API, unsigned long)
        NBLA_INSTANTIATE_IMPL(NBLA_API, long long)
        NBLA_INSTANTIATE_IMPL(NBLA_API, unsigned long long)
        NBLA_INSTANTIATE_IMPL(NBLA_API, float)
        NBLA_INSTANTIATE_IMPL(NBLA_API, double)
        NBLA_INSTANTIATE_IMPL(NBLA_API, long double)
    }
}