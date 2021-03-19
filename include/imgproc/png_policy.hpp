#ifndef IS_IMGPROC_PNG_POLICY_HPP
#define IS_IMGPROC_PNG_POLICY_HPP
#include "imgproc/common.hpp"


namespace Is
{
    namespace imgproc
    {
        namespace format_policy
        {
            using std::string;
            using byte = unsigned char;

            class IMGPROC_API PngFilePolicy
            {
            public:
                void save(const string& filename, byte* data, int32_t width, int32_t height, int channels);
                void load(const string& filename, byte* data, int32_t width, int32_t height, int channels);
            };
        } // namespace format_policy
    } // namespace imgproc
}
#endif