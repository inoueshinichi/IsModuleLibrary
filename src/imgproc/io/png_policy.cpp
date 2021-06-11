#include "imgproc/io/png_policy.hpp"

namespace Is
{
    namespace imgproc
    {
        namespace format_policy
        {
            /*----------------------------------------------------------------------------------*/
            PngFile::PngFile()
            {
                
            }

            PngFile::~PngFile()
            {

            }

            void PngFile::clear()
            {

            }

            void PngFile::set_data(byte *data, int width, int height,
                                   int channels, int insert_color)
            {

            }

            bool PngFile::get_data(byte* data, int extract_color)
            {
                return true;
            }

            bool PngFile::save(const string &filename, bool is_dump)
            {
                std::cout << "save : PngFile" << std::endl;
                return true;
            }

            bool PngFile::load(const string &filename, int &width, int &height, 
                               int &channels, bool is_dump)
            {
                std::cout << "load : PngFile" << std::endl;
                return true;
            }
        } // namespace format_policy
    } // namespace imgproc
}