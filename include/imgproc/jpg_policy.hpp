#ifndef IS_IMGPROC_JPG_POLICY_HPP
#define IS_IMGPROC_JPG_POLICY_HPP
#include "imgproc/common.hpp"

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
            public:
                void save(const string& filename, byte* data, int32_t width, int32_t height, int chanenls);
                void load(const string& filename, byte* data, int32_t width, int32_t height, int chanenls);
            };
        } // namespace format_policy
    } // namespace imgproc
}
#endif