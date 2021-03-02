#ifndef IS_WIN32_GRAPHICS_HPP
#define IS_WIN32_GRAPHICS_HPP

#include "imgproc/common.hpp"

#include <Windows.h>
#include <atlstr.h>

static_assert(_MSC_FULL_VER >= 190023506, 
              "Compile with newer than Visual Studio 2015 [v14.0] Update 1.");

namespace Is
{
    namespace imgproc
    {
        namespace win32
        {
            class IMGPROC_API WinBmp
            {
            protected:
                /*Windows Bitmap*/
                BITMAPFILEHEADER bmp_file_header_;
                BITMAPINFO* bmi_info_;

                BYTE* bmp_;
                int32_t width_;
                int32_t height_;
                size_t mem_width_;
                int channels_;
                size_t data_size_;
            public:
                WinBmp();
                virtual ~WinBmp();

                void clear();
                void setup(int32_t width, int32_t height, int channels);
                void set_data(BYTE* data, int insert_color = -1);
                void get_data(BYTE* data);
                void view_on_window(HDC hdc, const RECT& rect, int mode = COLORONCOLOR);
            };
        } // namespace win32
    } // namespace imgproc
}
#endif