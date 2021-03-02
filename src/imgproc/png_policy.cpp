#include "imgproc/png_policy.hpp"

namespace Is
{
    namespace imgproc
    {
        namespace format_policy
        {
            /*----------------------------------------------------------------------------------*/
            
            void PngFilePolicy::save(const string& filename, byte* data, int32_t width, int32_t height, int chanenls)
            {
                std::cout << "save : PngFilePolicy" << std::endl;
            }

            void PngFilePolicy::load(const string& filename, byte* data, int32_t width, int32_t height, int chanenls)
            {
                std::cout << "load : PngFilePolicy" << std::endl;
            }
        } // namespace format_policy
    } // namespace imgproc
}