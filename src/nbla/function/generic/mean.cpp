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
            using namespace ::nbla::eigen;
            ConstMatrixMap<T> mx(x, outer_size, reduction_size);
            ColVectorMap<T> my(y, outer_size);
            my = mx.rowwise().sum() / reduction_size;
        }

    }
}