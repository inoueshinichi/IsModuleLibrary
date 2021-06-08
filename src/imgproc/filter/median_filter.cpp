#include <imgproc/filter/median_filter.hpp>

namespace Is
{
    namespace imgproc
    {
        NdArrayPtr median_filter(const Context& ctx, NdArrayPtr src, 
                                 int ksize_x, int ksize_y)
        {
            IMGPROC_CHECK_IMAGE_SHAPE(src);

            auto ndim = src->ndim();
            auto shape = src->shape();
            auto strides = src->strides();
            float *data = src->cast_data_and_get_pointer<float>(ctx);

            std::cout << "TEST median filter" << std::endl;

            return src;
        }
    }
}