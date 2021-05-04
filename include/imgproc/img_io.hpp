#pragma once

#include <nbla/nd_array.hpp>

/*ポリシークラス*/
#include <imgproc/bmp_policy.hpp>
#include <imgproc/png_policy.hpp>
#include <imgproc/jpg_policy.hpp>

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

            void save(const string& filename, const nbla::Context& ctx, 
                      nbla::NdArrayPtr ndarray, bool is_dump = false)
            {
                using byte = unsigned char;
                using namespace nbla;
                if (!ndarray) 
                {
                    throw std::runtime_error("NdArray is nullptr.");
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
                
                // 画像ファイルに書き出し
                format_policy_.save(filename, data, width, height, channels, is_dump);
            }

            nbla::NdArrayPtr load(const string& filename, const nbla::Context& ctx, bool is_dump = false)
            {
                using byte = unsigned char;
                using namespace nbla;
                
                
                // 画像ファイルを読み込む
                auto[width, height, channels] = format_policy_.load(filename, is_dump);
                if (width == 0 || height == 0 || channels == 0)
                {
                    return make_shared<NdArray>();
                }

                auto ndarray = NdArray::create(Shape_t{height, width, channels});
                ndarray->zero();
                byte* data = ndarray->cast_data_and_get_pointer<byte>(ctx);

                // NdArrayにコピー
                format_policy_.get_data(data);

                return ndarray;
            }
        };
    } // namespace imgproc
}