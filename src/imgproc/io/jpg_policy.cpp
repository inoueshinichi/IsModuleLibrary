#include "imgproc/io/jpg_policy.hpp"

namespace Is
{
    namespace imgproc
    {
        namespace format_policy
        {
            /*----------------------------------------------------------------------------------*/
            JpgFile::~JpgFile()
            {
                
            }

            JpgFile::JpgFile()
            {

            }

            void JpgFile::set_data(byte* data, int insert_color)
            {

            }

            bool JpgFile::get_data(byte* data, int extract_color)
            {
                return true;
            }

            bool JpgFile::save(const string &filename, bool is_dump)
            {
                std::cout << "save : JpgFile" << std::endl;
                return true;
            }

            bool JpgFile::load(const string &filename, int &width, int &height, int &channels, bool is_dump)
            {
                std::cout << "load : JpgFile" << std::endl;

                return true;
            }
        } // namespace format_policy
    } // namespace imgproc
}