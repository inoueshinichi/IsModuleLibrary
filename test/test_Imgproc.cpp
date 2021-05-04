#include <imgproc/img_io.hpp>
#include <nbla/nd_array.hpp>

#include <string>

int main(int argc, char** argv)
{
    using namespace std;
    using namespace Is::imgproc;
    using namespace Is::imgproc::format_policy;
    using namespace Is::nbla;
    using byte = unsigned char;
    Context ctx_cpu;

    // string dummy_filename = "C:\\Users\\InoueShinichi\\Desktop\\Github_Projects\\IsNdarrayLibrary\\data\\lena256.bmp";
    string dummy_filename = "/home/inoue/Desktop/Lenna.bmp";


    // Test Image Io Policy
    {
        // Bmp
        ImageIo<BmpFilePolicy> io_bmp;
        NdArrayPtr test_ndarray = io_bmp.load(dummy_filename, ctx_cpu, true);
        // io_bmp.save(dummy_filename, ctx_cpu, test_ndarray, true);

        // // Png
        // ImageIo<PngFilePolicy> io_png;
        // io_png.load(dummy_filename, ctx_cpu, test_ndarray, true);
        // io_png.save(dummy_filename, ctx_cpu, test_ndarray, true);
        
        // // Jpeg
        // ImageIo<JpgFilePolicy> io_jpg;
        // io_jpg.save(test_ndarray, dummy_filename);
        // io_jpg.load(test_ndarray, dummy_filename);
    }
    

    return 0;
}