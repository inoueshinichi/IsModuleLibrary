#include <imgproc/filter/average_filter.hpp>

namespace Is
{
    namespace imgproc
    {
        NdArrayPtr average_filter(const Context& ctx, NdArrayPtr src, 
                                  int ksize_x, int ksize_y)
        {
            IMGPROC_CHECK_IMAGE_SHAPE(src);

            auto ndim = src->ndim();
            auto shape = src->shape();
            auto strides = src->strides();
            float *data = src->cast_data_and_get_pointer<float>(ctx);

            std::string msg = utils::format_string(
                "ndim: %d, size of shape: %d, size of strides: %d",
                ndim,
                shape.size(),
                strides.size()
            );
            std::cout << msg << std::endl;

            int channels = shape[0];
            int height = shape[1];
            int width = shape[2];

            for (int c = 0; c < channels; ++c)
            {
                for (int y = 0; y < height; ++y)
                {
                    for (int x = 0; x < width; ++x)
                    {
                        if (x >= 50 && x < 100)
                            data[c * strides[0] + y * strides[1] + x * strides[2]] = 0; // テスト
                    }
                }
            }

            std::cout << "TEST blur" << std::endl;

            return src;
        }
    }
}