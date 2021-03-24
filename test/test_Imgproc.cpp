#include "imgproc/img_io.hpp"
#include "nbla/is_nd_array.hpp"

#include <string>

int main(int argc, char** argv)
{
    using namespace std;
    using namespace Is::imgproc;
    using namespace Is::imgproc::format_policy;
    using namespace Is::nbla;
    using byte = unsigned char;
    Context ctx_cpu;

    IsNdArrayPtr test_ndarray;
    string dummy_filename = "C:\\Users\\InoueShinichi\\Desktop\\Github_Projects\\IsNdarrayLibrary\\data\\lena256.bmp";

    // Test Image Io Policy
    {
        // Bmp
        ImageIo<BmpFilePolicy> io_bmp;
        io_bmp.load(dummy_filename, ctx_cpu, test_ndarray, true);
        io_bmp.save(dummy_filename, ctx_cpu, test_ndarray, true);
        // // Png
        // ImageIo<PngFilePolicy> io_png;
        // io_png.save(test_ndarray, dummy_filename);
        // io_png.load(test_ndarray, dummy_filename);
        // // Jpeg
        // ImageIo<JpgFilePolicy> io_jpg;
        // io_jpg.save(test_ndarray, dummy_filename);
        // io_jpg.load(test_ndarray, dummy_filename);
    }
    

    return 0;
}