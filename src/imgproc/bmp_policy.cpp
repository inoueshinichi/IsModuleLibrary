#include "imgproc/bmp_policy.hpp"

namespace Is
{
    namespace imgproc
    {
        namespace format_policy
        {
            BmpFilePolicy::~BmpFIlePolicy()
            {
                { clear(); }
            }

            void BmpFilePolicy::clear()
            {
                if (bmi_info)
                {
                    delete[] bmi_info;
                    bmi_info = nullptr;
                }
                if (bmp)
                {
                    delete[] bmp;
                    bmp = nullptr;
                }
            }

            void BmpFilePolicy::setup(int32_t width, int32_t height, int channels)
            {
                clear();

                size_t info_size = 0;
                if (channels == 1)
                    info_size = sizeof(BmpInfoHeader) + sizeof(RgbQuad) * 256;
                else
                    info_size = sizeof(BmpInfoHeader);

                bmi_info = (BmiInfo*) new byte[info_size];
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
                
                data_size = mem_width * height;
                this->channels = channels;

                /*BmpFileHeader*/
                bmp_file_header_.bf_type = (0x42 << 8) | 0x4d; // 'B', 'M'
                bmp_file_header_.bf_size = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader) + data_size;
                bmp_file_header_.bf_reserved1 = (uint16_t)0;
                bmp_file_header_.bf_reserved1 = (uint16_t)0;
                bmp_file_header_.bf_offset_bits = sizeof(BmpFileHeader) + info_size;

                /*BmiInfoHeader*/ 
                bmi_info->bmi_header.bi_size = sizeof(BmpInfoHeader);
                bmi_info->bmi_header.bi_width = width;
                bmi_info->bmi_header.bi_height = height;
                bmi_info->bmi_header.bi_planes = 1;
                bmi_info->bmi_header.bi_bitcount = bit_count;
                bmi_info->bmi_header.bi_compression = 0; // 無圧縮
                bmi_info->bmi_header.bi_size_image = data_size;
                bmi_info->bmi_header.bi_x_pels_per_meter = 0; // 無効
                bmi_info->bmi_header.bi_y_pels_per_meter = 0; // 無効
                bmi_info->bmi_header.bi_color_used = 0; // 全カラー使う
                bmi_info->bmi_header.bi_clor_important = 0; // 全カラー使う

                if (channels == 1)
                {
                     /*カラーパレット*/
                    for (int n = 0; n < 256; ++n)
                    {
                        bmi_info->bmi_color[n].rgb_blue = byte(n);
                        bmi_info->bmi_color[n].rgb_green = byte(n);
                        bmi_info->bmi_color[n].rgb_red = byte(n);
                        bmi_info->bmi_color[n].rgb_reserved = 0;
                    }
                }
               
                /*データ部*/
                bmp = new byte[data_size];
                std::memset((void*)bmp, 0, data_size);
            }

            void BmpFilePolicy::set_data(byte* data)
            {

            }

            void BmpFilePolicy::get_data(byte* data)
            {

            }

            /*----------------------------------------------------------------------------------*/

            void BmpFilePolicy::save(const string& filename, byte* data, 
                                     int32_t width, int32_t height, int chanenls)
            {
                std::cout << "save : BmpFilePolicy" << std::endl;
            }

            void BmpFilePolicy::load(const string& filename, byte* data, 
                                    int32_t width, int32_t height, int chanenls)
            {
                std::cout << "load : BmpFilePolicy" << std::endl;
            }
        }
    }
}