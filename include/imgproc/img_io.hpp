#ifndef IS_IMGPROC_IMG_IO_HPP
#define IS_IMGPROC_IMG_IO_HPP

#include "nbla/is_nd_array.hpp"

/*ポリシークラス*/
#include "imgproc/bmp_policy.hpp"
#include "imgproc/png_policy.hpp"
#include "imgproc/jpg_policy.hpp"

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

            void save(shared_ptr<nbla::IsNdArray> ndarray, const string& filename)
            {

                format_policy_.save(filename, nullptr, 0, 0, 0);
            }

            void load(shared_ptr<nbla::IsNdArray> ndarray, const string& filename)
            {
                format_policy_.load(filename, nullptr, 0, 0, 0);
            }
        };
    } // namespace imgproc
}
#endif