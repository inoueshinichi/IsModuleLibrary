#include <iostream>
#include <string>
#include <cstdio>
#include <iomanip>
#include <cstring>

using namespace Is::nbla;

auto property = [](const std::string& test_name, auto&& func, auto&&... args) {
    std::cout << "[ENTER] " << test_name << "---------------" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    std::forward<decltype(func)>(func)(std::forward<args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = end - start;
    double duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << duration_ms << " [ms]" << std::endl;

    std::cout << "[EXIT] " << test_name << "---------------" << std::endl;
    std::cout << std::endl;
};


void show_status_memory(Memory *memory) 
{
    std::cout << "memory: " << std::hex << memory << std::dec << std::endl;
    std::cout << "bytes: " << memory->bytes() << std::endl;
    std::cout << "device_id: " << memory->device_id() << std::endl;
    std::cout << "ptr: " << std::hex << memory->pointer() << std::dec << std::endl;
    std::cout << "locked: " << std::boolalpha << memory->locked() << std::endl;
    std::cout << "disabled: " << std::boolalpha << memory->disabled() << std::endl;
    std::cout << "next: " << std::hex << memory->next() << std::dec << std::endl;
    std::cout << "prev: " << std::hex << memory->prev() << std::dec << std::endl;
    std::cout << "active bytes: " << memory->bytes_active() << std::endl;
    std::cout << "---------------------------" << std::endl;
}


template <typename T>
void show_ndarray_contents(NdArrayPtr ndarray)
{
    const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
    Shape_t shape = ndarray->shape();
    Stride_t strides = ndarray->strides();
    Size_t ndim = ndarray->ndim();
    const T *data = ndarray->cast_data_and_get_pointer<T>(ctx);

    if (ndim ==2)
    {
        std::cout << "[";
        for (int i = 0; i < shape[0]; ++i)
        {
            std::cout << "[";
            for (int j = 0; j < shape[1]; ++j)
            {
                T tmp = data[i * strides[0] + j * strides[1]];
                std::string elem = format_string("%3f", (float)tmp);
                std::cout << elem;

                if (j != shape[1] - 1)
                    std::cout << "  ";
            }
            std::cout << "]";

            if (i != shape[0] - 1)
                std::cout << std::endl;
        }
        std::cout << "]" << std::endl;
    }
    else if (ndim == 3)
    {
        std::cout << "[";
        for (int c = 0; c < shape[0]; ++c)
        {
            std::cout << "[";
            for (int j = 0; j < shape[1]; ++j)
            {
                if (j != 0)
                    std::cout << "  ";
                std::cout << "[";
                for (int i = 0; i < shape[2]; ++i)
                {
                    T tmp = data[c * strides[0] + j * strides[1] + i * strides[2]];
                    std::string elem = format_string("%.2f", (float)tmp);
                    std::cout << elem;
                    if (i != shape[2] - 1)
                        std::cout << " ";
                }

                if (j == shape[1] - 1)
                {
                    if (c != shape[0] - 1)
                        std::cout << "]]" << std::endl;
                    else
                        std::cout << "]]";
                }
                else
                    std::cout << "]" << std::endl;
            }
        }
        std::cout << "]" << std::endl;
    }
    else
    {

    }
}
