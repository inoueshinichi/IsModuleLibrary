#pragma once

/*NdArray*/
#include <nbla/nd_array.hpp>
#include <nbla/nd_array_extra.hpp>

/*ポリシークラス*/
#include <imgproc/io/bmp_policy.hpp>
#include <imgproc/io/png_policy.hpp>
#include <imgproc/io/jpg_policy.hpp>

#include <utils/format_string.hpp>

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

            bool save(const string& filename, const nbla::Context& ctx, 
                      nbla::NdArrayPtr ndarray, bool is_dump = false)
            {
                using byte = unsigned char;
                using namespace nbla;
                if (!ndarray) 
                {
                    std::cerr << "dArray is nullptr." << std::endl;
                    return false;
                }

                int32_t channels = 0;
                int32_t width = 0;
                int32_t height = 0;
                Shape_t shape = ndarray->shape(); // (c, h, w)
                Size_t ndim = ndarray->ndim();
                if (ndim == 2) 
                {
                    height = shape.at(0);
                    width = shape.at(1);
                    channels = 1;
                } 
                else if (ndim == 3) 
                {
                    channels = shape.at(0);
                    height = shape.at(1);
                    width = shape.at(2);
                } 
                else
                {
                    throw std::runtime_error(utils::format_string(
                        "Unmatch shape of ndarray for bitmap file format. Given is %d", ndarray->ndim()));
                }

                // (w, h, c)
                ndarray->reshape(Shape_t{width, height, channels});
                byte *data = ndarray->cast_data_and_get_pointer<byte>(ctx);

                // 書き込み
                format_policy_.set_data(data, width, height, channels);

                // 画像ファイルに書き出し
                return format_policy_.save(filename, is_dump);
            }

            bool load(const string &filename, nbla::Context &ctx, nbla::NdArrayPtr ndarray, bool is_dump = false)
            {
                using byte = unsigned char;
                using namespace nbla;
                int width = 0;
                int height = 0;
                int channels = 0;

                // 画像ファイルを読み込む
                if (!format_policy_.load(filename, width, height, channels, is_dump))
                {
                    std::cerr << "ndarray nullptr" << std::endl;
                    return false;
                }

                ndarray->zero();
                ndarray->reshape(Shape_t{height, width, channels}, true);
                byte *data = ndarray->cast_data_and_get_pointer<byte>(ctx);

                // NdArrayにコピー
                format_policy_.get_data(data);

                // (c, h, w)
                ndarray->reshape(Shape_t{channels, height, width});

                std::printf("width: %d, height: %d, channels: %d\n", width, height, channels);

                return true;
            }
        };
    } // namespace imgproc
}