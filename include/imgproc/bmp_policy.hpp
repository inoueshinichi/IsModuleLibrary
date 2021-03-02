#ifndef IS_IMGPROC_BMP_POLICY_HPP
#define IS_IMGPROC_BMP_POLICY_HPP
#include "imgproc/common.hpp"

namespace Is
{
    namespace imgproc
    {
        namespace format_policy
        {
            using std::string;
            using byte = unsigned char;

            class IMGPROC_API BmpFilePolicy
            {
                /*BMPファイルヘッダ*/
                typedef struct BmpFileHeader {
                    uint16_t bf_type;
                    uint32_t bf_size;
                    uint16_t bf_reserved1;
                    uint16_t bf_reserved2;
                    uint32_t bf_offset_bits;
                }BmpFileHeader;

                /*BMP情報ヘッダ*/
                typedef struct BmpInfoHeader {
                    uint32_t bi_size; // 40
                    int32_t bi_width;
                    int32_t bi_height; // 値が負の場合はトップダウン画像となる
                    uint16_t bi_planes; // 常に1
                    uint16_t bi_bitcount; // 0, 1, 4, 8, 16, 24, 32
                    uint32_t bi_compression; // 0, 1, 2, 3, 4, 5
                    uint32_t bi_size_image;
                    int32_t bi_x_pels_per_meter;
                    int32_t bi_y_pels_per_meter;
                    uint32_t bi_color_used; // ビットマップで実際に使用するカラーパレット内のカラーインデックスの数
                    uint32_t bi_clor_important; // ビットマップを表示するために必要なカラーインデックスの数
                }BmpInfoHeader;

                /*BMPカラーパレット*/
                typedef struct RgbQuad {
                   byte rgb_blue;
                   byte rgb_green;
                   byte rgb_red;
                   byte rgb_reserved;
                }RgbQuad;

                /*BMP情報*/
                typedef struct BmiInfo {
                    BmpInfoHeader bmi_header;
                    RgbQuad bmi_color[1];
                }BmiInfo;

                // メンバ変数
                BmpFileHeader bmp_file_header_;
                BmiInfo*      bmi_info;
                byte*         bmp;
                int           channels;

                // メンバ関数
                void clear();
                void setup(int32_t width, int32_t height, int channels);
                void set_data(byte* data);
                void get_data(byte* data);
                
            public:
                BmpFilePolicy() = default;
                ~BmpFilePolicy();
                BmpFilePolicy(const BmpFilePolicy&) = delete;
                BmpFilePolicy& operator=(const BmpFilePolicy&) = delete;
                BmpFilePolicy(BmpFilePolicy&&) = delete;
                BmpFilePolicy& operator=(BmpFilePolicy&&) = delete;

                void save(const string& filename, byte* data, int32_t width, int32_t height, int chanenls);
                void load(const string& filename, byte* data, int32_t width, int32_t height, int chanenls);
            };
        } // namespace format_policy
    } // namespace imgproc
}
#endif