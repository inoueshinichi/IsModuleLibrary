#include "imgproc/win32/win32_graphics.hpp"
#include "utils/format_string.hpp"

#include <cstring>

namespace Is
{
    namespace imgproc
    {
        namespace win32
        {
            WinBmp::WinBmp() 
                : bmp_(nullptr_)
                , width_(0)
                , height_(0)
                , mem_width_(0)
                , channels_(0)
                , data_size_(0) {}

            WinBmp::~WinBmp() 
            {
                clear();
            }

            void WinBmp::clear()
            {
                if (bmi_info_) {
                    delete[] bmi_info_;
                    bmi_info_ = nullptr;
                }
                if (bmp_) {
                    delete[] bmp_;
                    bmp_ = nullptr;
                }
                width_ = 0;
                height_ = 0;
                mem_width_ = 0;
                channels = 0;
                data_size_ = 0;
            }

            void setup(int32_t width, int32_t height, int channels)
            {
                clear();

                size_t info_size = 0;
                if (channels == 1)
                    info_size = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
                else
                    info_size = sizeof(BITMAPINFOHEADER);
                
                bmi_info_ = (BITMAPINFO*) new BYTE[info_size];
                int padding = 0;
                size_t data_size = 0;
                uint16_t bit_count = 0; 
                size_t mem_width = 0;
                if (channels == 1)
                {
                    bit_count = 8;
                    padding = width % 4;
                    if (padding)
                        mem_width = width + (4 - padding);
                    else
                        mem_width = width;
                } 
                // else if (channels == 2) 
                // {
                //     bit_count = 16;
                //     padding = 2 * width % 4;
                //     if (padding)
                //         mem_width = 2 * width + (4 - padding);
                //     else
                //         mem_width = 2 * width;
                // } 
                else if (channels == 3) 
                {
                    bit_count = 24;
                    padding = 3 * width % 4;
                    if (padding)
                        mem_width = 3 * width + (4 - padding);
                    else
                        mem_width = 3 * width;
                } 
                else if (channels == 4) 
                {
                    bit_count = 32;
                    padding = 4 * width % 4;
                    if (padding)
                        mem_width = 4 * width + (4 - padding);
                    else
                        mem_width = 4 * width;
                } 
                else 
                {
                    throw std::runtime_error(
                        utils::format_string("Channels is not match. Given is %d", 
                                                channels));
                }
                
                this->mem_width_ = mem_width;
                data_size = mem_width * height;
                this->channels_ = channels;

                /*BmpFileHeader*/
                bmp_file_header_.bfYype = (0x42 << 8) | 0x4d; // 'B', 'M'
                bmp_file_header_.bfSize = sizeof(BITMAPFILEHEADER) + info_size + data_size;
                bmp_file_header_.bfReserved1 = (WORD)0;
                bmp_file_header_.bfReserved1 = (WORD)0;
                bmp_file_header_.bfOffBits = sizeof(BITMAPFILEHEADER) + info_size;

                /*BmiInfoHeader*/ 
                bmi_info_->bmiHeader.biSize = (DWORD)sizeof(BITMAPINFOHEADER);
                bmi_info_->bmiHeader.biWidth = (LONG)width;
                bmi_info_->bmiHeader.biHeight = (LONG)height;
                bmi_info_->bmiHeader.biPlanes = (WORD)1;
                bmi_info_->bmiHeader.biBitCount = (WORD)bit_count;
                bmi_info_->bmiHeader.biCompression = (DWORD)0; // 無圧縮
                bmi_info_->bmiHeader.biSizeImage = (DWORD)data_size;
                bmi_info_->bmiHeader.biXPelsPerMeter = (LONG)0; // 無効
                bmi_info_->bmiHeader.biYPelsPerMeter = (LONG)0; // 無効
                bmi_info_->bmiHeader.biClrUsed = (DWORD)0; // 全カラー使う
                bmi_info_->bmiHeader.biClrImportant = (DWORD)0; // 全カラー使う

                if (channels == 1)
                {
                    /*カラーパレット*/
                    for (int n = 0; n < 256; ++n)
                    {
                        bmi_info_->bmiColors[n].rgbBlue = BYTE(n);
                        bmi_info_->bmiColors[n].rgbGreen = BYTE(n);
                        bmi_info_->bmiColors[n].rgbRed BYTE(n);
                        bmi_info_->bmiColors[n].rgbReserved = 0;
                    }
                }
                
                /*データ部*/
                bmp_ = new byte[data_size];
                std::memset((void*)bmp, 0, data_size);
            }

            void WinBmp::set_data(BYTE* data, int insert_color)
            {
                if (!bmp_ || !bmi_info_)
                    return;
                
                size_t wdata = 0;
                size_t wbmp = mem_width_ * (height_ - 1);
                if (channels_ == 1)
                {
                    for (int j = 0; j < height_; ++j)
                    {
                        for (int i = 0; i < width_; ++i)
                        {
                            /*4バイト境界の端数を無視して保存*/
                            bmp_[wbmp + i] = data[wdata + i];
                        }
                        wdata += width_;
                        wbmp -= mem_width_;
                    }
                }
                else if (channels_ == 3)
                {
                    if (insert_color > 0 && insert_color < channels_)
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0, k = 0; k < 3 * width_; i+=3)
                            {
                                bmp_[wbmp + i + insert_color] = data[wdata + i + insert_color];
                            }
                        }
                    }
                    else
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0; i < 3 * width_; i+=3)
                            {
                                /*4バイト境界の端数を無視して保存*/
                                bmp_[wbmp + i + 0] = data[wdata + i + 0];
                                bmp_[wbmp + i + 1] = data[wdata + i + 1];
                                bmp_[wbmp + i + 2] = data[wdata + i + 2];
                            }
                            wdata += 3 * width_;
                            wbmp -= mem_width_;
                        }
                    }
                }
                else // channels == 4
                {
                    if (insert_color > 0 && insert_color < channels_)
                    {
                        for (int j = 0; j < height_; ++j)
                        {
                            for (int i = 0, k = 0; k < 4 * width_; i+=4)
                            {
                                bmp_[wbmp + i + insert_color] = data[wdata + i + insert_color];
                            }
                        }
                    }
                    else
                    {
                        /*常に4バイトの倍数なので、一括コピー*/
                        std::memomove(bmp_, data, sizeof(BYTE) * mem_width_ * height_);
                    }
                }
            }

            void WinBmp::get_data(BYTE* data)
            {

            }
            

            void WinBmp::view_on_window(HDC hdc, const RECT& rect, int mode)
            {
                if (!bmp_ || !bmi_info_)
                    return;
                
                ::SetStretchBltMode(hdc, mode);
                ::StretchDIBits(
                    hdc,
                    rect.left,
                    rect.top,
                    rect.right,
                    rect.bottom,
                    0,
                    0,
                    width_,
                    height_,
                    bmp_,
                    bmi_info_,
                    DIB_RGB_COLORS,
                    SRCCOPY);
            }
        } // namespace win32
    } // namespace imgproc
}