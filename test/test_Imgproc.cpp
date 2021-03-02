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

    auto test_ndarray = IsNdArray::ones<byte>(ctx_cpu, Shape_t{16, 16});
    string dummy_filename = "path/to/dummy.out";

    // Test Image Io Policy
    {
        // Bmp
        ImageIo<BmpFilePolicy> io_bmp;
        io_bmp.save(test_ndarray, dummy_filename);
        io_bmp.load(test_ndarray, dummy_filename);
        // Png
        ImageIo<PngFilePolicy> io_png;
        io_png.save(test_ndarray, dummy_filename);
        io_png.load(test_ndarray, dummy_filename);
        // Jpeg
        ImageIo<JpgFilePolicy> io_jpg;
        io_jpg.save(test_ndarray, dummy_filename);
        io_jpg.load(test_ndarray, dummy_filename);
    }
    

    return 0;
}