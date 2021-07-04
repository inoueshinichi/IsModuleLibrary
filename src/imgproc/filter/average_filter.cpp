#include <imgproc/filter/utils_filter.hpp>
#include <imgproc/filter/average_filter.hpp>
#include <imgproc/saturate_cast.hpp>

namespace Is
{
    namespace imgproc
    {
        NdArrayPtr average_filter(NdArrayPtr src, int ksize_x, int ksize_y)
        {
            IMGPROC_CHECK_IMAGE_SHAPE(src);

            const auto& ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto ndim = src->ndim();
            auto sh = src->shape();
            auto st = src->strides();
            float *data_src = src->cast_data_and_get_pointer<float>(ctx);
            using uchar = unsigned char;

            // std::string msg = utils::format_string(
            //     "ndim: %d, size of shape: %d, size of strides: %d",
            //     ndim,
            //     shape.size(),
            //     strides.size()
            // );
            // std::cout << msg << std::endl;
            // -------------------------------------------------------

            int C = sh[0];
            int H = sh[1];
            int W = sh[2];

            if (ksize_x % 2 == 0)
                ksize_x += 1;
            if (ksize_y % 2 == 0)
                ksize_y += 1;
            int hlf_ksx = (int)(ksize_x / 2);
            int hlf_ksy = (int)(ksize_y / 2);

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
            auto dst = zeros<uchar>(sh);
            uchar *data_dst = dst->cast_data_and_get_pointer<uchar>(ctx);

            float value;
            int tx, ty;
            int offset_x, offset_y;
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
                                ty = y + j;
                                tx = x + i ;
                                value += kernel[j * ksize_x + i] * data_ex[c * exc + ty* exh + tx * exw];
                            }
                        }
                        data_dst[c * st[0] + y * st[1] + x * st[2]] = saturate_cast<uchar>(value);
                    }
                }
            }
            delete[] kernel;

            std::cout << "TEST blur" << std::endl;

            
            return dst;
        }
    }
}