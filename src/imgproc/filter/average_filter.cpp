#include <imgproc/filter/utils_filter.hpp>
#include <imgproc/filter/average_filter.hpp>

namespace Is
{
    namespace imgproc
    {
        NdArrayPtr average_filter(NdArrayPtr src, int ksize_x, int ksize_y)
        {
            IMGPROC_CHECK_IMAGE_SHAPE(src);

            const auto& ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto ndim = src->ndim();
            auto shape = src->shape();
            auto strides = src->strides();
            float *data_src = src->cast_data_and_get_pointer<float>(ctx);

            // std::string msg = utils::format_string(
            //     "ndim: %d, size of shape: %d, size of strides: %d",
            //     ndim,
            //     shape.size(),
            //     strides.size()
            // );
            // std::cout << msg << std::endl;
            // -------------------------------------------------------

            int C = shape[0];
            int H = shape[1];
            int W = shape[2];

            if (ksize_x % 2 == 0)
                ksize_x += 1;
            if (ksize_y % 2 == 0)
                ksize_y += 1;
            int hlf_ksx = ksize_x / 2;
            int hlf_ksy = ksize_y / 2;

            int ksize = ksize_x * ksize_y;
            float *kernel = new float[ksize];
            for (int k = 0; k < ksize; ++k)
                kernel[k] = (float)(1.0 / ksize);

            auto extend = padding<float>(src, hlf_ksx, hlf_ksy, "mean");
            auto ex_shape = extend->shape();
            int exc = ex_shape[0];
            int exh = ex_shape[1];
            int exw = ex_shape[2];
            float *data_ex = extend->cast_data_and_get_pointer<float>(ctx);
            auto dst = zeros<float>(shape);
            float *data_dst = dst->cast_data_and_get_pointer<float>(ctx);

            float value;
            int tx, ty;
            for (int c = 0; c < C; ++c)
            {
                for (int y = 0; y < H; ++y)
                {
                    for (int x = 0; x < W; ++x)
                    {
                        value = 0.0f;
                        // kernel
                        for (int j = 0; j < ksize_y; ++j)
                        {
                            for (int i = 0; i < ksize_x; ++i)
                            {
                                ty = y - hlf_ksy + j;
                                tx = x - hlf_ksx + i;
                                value += kernel[j * ksize_x + i] * data_ex[c * exc + ty * exh + tx * exw];
                            }
                        }

                    }
                }
            }

                std::cout << "TEST blur" << std::endl;

            delete[] kernel;
            return dst;
        }
    }
}