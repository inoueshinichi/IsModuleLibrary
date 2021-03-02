#include "imgproc/jpg_policy.hpp"

namespace Is
{
    namespace imgproc
    {
        namespace format_policy
        {
            /*----------------------------------------------------------------------------------*/

            

             void JpgFilePolicy::save(const string& filename, byte* data, int32_t width, int32_t height, int chanenls)
            {
                std::cout << "save : JpegFilePolicy" << std::endl;
            }

            void JpgFilePolicy::load(const string& filename, byte* data, int32_t width, int32_t height, int chanenls)
            {
                std::cout << "load : JpegFilePolicy" << std::endl;

            }
        } // namespace format_policy
    } // namespace imgproc
}