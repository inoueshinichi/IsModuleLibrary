#include "imgproc/png_policy.hpp"

namespace Is
{
    namespace imgproc
    {
        namespace format_policy
        {
            /*----------------------------------------------------------------------------------*/
            PngFilePolicy::~PngFilePolicy()
            {
                
            }

            PngFilePolicy::PngFilePolicy()
            {

            }

            void PngFilePolicy::set_data(byte* data, int insert_color)
            {

            }

            void PngFilePolicy::get_data(byte* data, int extract_color)
            {

            }
            
            void PngFilePolicy::save(const string& filename, byte* data, int32_t width, int32_t height, int32_t channels, bool is_dump)
            {
                std::cout << "save : PngFilePolicy" << std::endl;
            }

            std::tuple<int32_t, int32_t, int32_t> PngFilePolicy::load(const string& filename, bool is_dump)
            {
                std::cout << "load : PngFilePolicy" << std::endl;
                int32_t width = 0;
                int32_t height = 0;
                int32_t channels = 0;
                return std::make_tuple(width, height, channels);
            }
        } // namespace format_policy
    } // namespace imgproc
}