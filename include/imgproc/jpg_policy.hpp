#ifndef IS_IMGPROC_JPG_POLICY_HPP
#define IS_IMGPROC_JPG_POLICY_HPP
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

            class IMGPROC_API JpgFilePolicy
            {
                void set_data(byte* data, int insert_color = -1);

            public:
                JpgFilePolicy();
                virtual ~JpgFilePolicy();
                JpgFilePolicy(const JpgFilePolicy&) = delete;
                JpgFilePolicy& operator=(const JpgFilePolicy&) = delete;
                JpgFilePolicy(JpgFilePolicy&&) = default;
                JpgFilePolicy& operator=(JpgFilePolicy&&) = default;

                void get_data(byte* data, int extract_color = -1);
                
                void save(const string& filename, byte* data, int32_t width, int32_t height, int32_t channels, bool is_dump);
                std::tuple<int32_t, int32_t, int32_t> load(const string& filename, bool is_dump);
                void dump() const;
            };
        } // namespace format_policy
    } // namespace imgproc
}
#endif