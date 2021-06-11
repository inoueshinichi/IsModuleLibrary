#pragma once

#include "imgproc/common.hpp"

#include <cstdint>

namespace Is
{
    namespace imgproc
    {
        namespace format_policy
        {
            using std::string;
            using byte = unsigned char;

            class IMGPROC_API PngFile
            {
                int32_t width_;
                int32_t height_;
                size_t mem_width_;
                int32_t channels_;
                size_t datasize_;

                void clear();
                void setup(int32_t width, int32_t height, int32_t channels);
                void dump() const;
                

            public:
                PngFile();
                virtual ~PngFile();
                PngFile(const PngFile &) = delete;
                PngFile &operator=(const PngFile &) = delete;
                PngFile(PngFile &&) = default;
                PngFile &operator=(PngFile &&) = default;

                int width() const { return width_; }
                int height() const { return height_; }
                int channels() const { return channels_; }
                size_t datasize() const { return datasize_; }

                void set_data(byte *data, int width, int height, int channels, int insert_color = -1);
                bool get_data(byte *data, int extract_color = -1);
                bool save(const string &filename, bool is_dump);
                bool load(const string &filename, int &width, int &height, int &channels, bool is_dump);
            };
        } // namespace format_policy
    } // namespace imgproc
}