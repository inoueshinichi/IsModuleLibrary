#include <nbla/function/mean.hpp>
#include <nbla/utils/eigen.hpp>


namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Mean, const vector<int>&, bool)

        template <typename T>
        void Mean<T>::execute_impl_reduce(const T *x, T *y, 
                                          int outer_size,
                                          int reduction_size) 
        {
            using namespace ::Is::nbla::eigen;
            ConstMatrixMap<T> mx(x, outer_size, reduction_size);
            ColVectorMap<T> my(y, outer_size);
            my = mx.rowwise().sum() / reduction_size;
        }

#define NBLA_INSTANTIATE_IMPL(API, TYPE) \
    template API void Mean<TYPE>::execute_impl_reduce(const TYPE *x, TYPE *y, int outer_size, int reduction_size);

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