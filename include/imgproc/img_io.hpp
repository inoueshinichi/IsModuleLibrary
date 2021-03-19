#ifndef IS_IMGPROC_IMG_IO_HPP
#define IS_IMGPROC_IMG_IO_HPP

#include "nbla/is_nd_array.hpp"

/*ポリシークラス*/
#include "imgproc/bmp_policy.hpp"
#include "imgproc/png_policy.hpp"
#include "imgproc/jpg_policy.hpp"

#include "utils/format_string.hpp"

#include <iostream>

namespace Is
{
    namespace imgproc
    {
        using std::string;
        using std::shared_ptr;

        template <class FormatPolicy>
        class ImageIo
        {
            FormatPolicy format_policy_;
        public:
            ImageIo() = default;
            ~ImageIo() = default;
            ImageIo(const ImageIo&) = delete;
            ImageIo& operator=(const ImageIo&) = delete;
            ImageIo(ImageIo&&) = delete;
            ImageIo& operator=(ImageIo&&) = delete;

            void save(const string& filename, const nbla::Context& ctx, 
                      shared_ptr<nbla::IsNdArray> ndarray, bool is_dump = false)
            {
                using byte = unsigned char;
                using namespace nbla;
                if (!ndarray) 
                {
                    throw std::runtime_error("IsNdArray is nullptr.");
                }

                byte* data = ndarray->cast_data_and_get_pointer<byte>(ctx);
                int32_t channels = 0;
                int32_t width = 0;
                int32_t height = 0;
                Shape_t shape = ndarray->shape(); // (N,M,C)
                Size_t ndim = ndarray->ndim();
                if (ndim == 2) {
                    height = shape.at(0);
                    width = shape.at(1);
                    channels = 1;
                } else if (ndim == 3) {
                    height = shape.at(0);
                    width = shape.at(1);
                    channels = shape.at(2);
                } else {
                    throw std::runtime_error(utils::format_string(
                        "Unmatch shape of ndarray for bitmap file format. Given is %d", ndarray->ndim()));
                }
                
                // Bitmapファイルに書き出し
                format_policy_.save(filename, data, width, height, channels, is_dump);
            }

            void load(const string& filename, const nbla::Context& ctx, 
                      shared_ptr<nbla::IsNdArray>& ndarray, bool is_dump = false)
            {
                using byte = unsigned char;
                using namespace nbla;
                int32_t channels = 0;
                int32_t width = 0;
                int32_t height = 0;

                // Bitmapファイルを読み込む
                format_policy_.load(filename, width, height, channels, is_dump);

                ndarray = IsNdArray::zeros<byte>(ctx, Shape_t{height, width, channels});
                byte* data = ndarray->cast_data_and_get_pointer<byte>(ctx);

                // NdArrayにコピー
                format_policy_.get_data(data);
            }
        };
    } // namespace imgproc
}
#endif