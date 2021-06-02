#include <gtest/gtest.h>

// nnabla core
#include <nbla/context.hpp>
#include <nbla/memory/cpu_memory.hpp>
#include <nbla/memory/allocator.hpp>
#include <nbla/memory/naive_allocator.hpp>
#include <nbla/memory/caching_allocator_with_buckets.hpp>
#include <nbla/array.hpp>
#include <nbla/array_registry.hpp>
#include <nbla/synced_array.hpp>
#include <nbla/nd_array.hpp>

#include <nbla/nd_array_extra.hpp>

// nnabla func
#include <nbla/function/add_scalar.hpp>
#include <nbla/function/randn.hpp>



#include <iostream>
#include <string>
#include <cstdio>
#include <iomanip>
#include <cstring>


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


void show_status_memory(Is::nbla::Memory* memory) 
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
void show_ndarray_contents(const Is::nbla::Context &ctx, Is::nbla::NdArrayPtr ndarray)
{
    using namespace Is::nbla;
    Shape_t shape = ndarray->shape();
    Stride_t strides = ndarray->strides();
    // Size_t size = ndarray->size();
    // Size_t ndim = ndarray->ndim();
    auto synced_array = ndarray->array();
    const T *data = static_cast<const T *>(synced_array->data_ptr(get_dtype<T>(), ctx));

    std::cout << "[";
    for (size_t c = 0; c < shape[0]; ++c)
    {
        std::cout << "[";
        for (size_t j = 0; j < shape[1]; ++j)
        {
            if (j != 0)
                std::cout << "  ";
            std::cout << "[";
            for (size_t i = 0; i < shape[2]; ++i)
            {
                T tmp = data[c * strides[0] + j * strides[1] + i * strides[2]];
                std::cout << tmp;
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


template <typename T>
void show_2d_array(const Is::nbla::Context& ctx, Is::nbla::NdArrayPtr ndarray)
{
    using namespace Is::nbla;
    Shape_t shape = ndarray->shape();
    Stride_t strides = ndarray->strides();
    Size_t size = ndarray->size();
    Size_t ndim = ndarray->ndim();
    auto synced_array = ndarray->array();
    const T *data = static_cast<const T *>(synced_array->data_ptr(get_dtype<T>(), ctx));

    std::cout << "[";
    for (int i = 0; i < shape[0]; ++i)
    {
        std::cout << "[";
        for (int j = 0; j < shape[1]; ++j)
        {
            T tmp = data[i * strides[0] + j * strides[1]];
            std::cout << tmp;

            if (j != shape[1] - 1)
                std::cout << "  ";
        }
        std::cout << "]";

        if (i != shape[0] - 1)
            std::cout << std::endl;
    }
    std::cout << "]" << std::endl;
}

// ---------------------------------------------------------------------------

namespace
{
    TEST(nnabla_core, context)
    {
        using namespace Is::nbla;
        Context ctx_cpu;

        std::cout << ctx_cpu.array_class << std::endl;
        std::cout << ctx_cpu.device_id << std::endl;
        std::cout << ctx_cpu.to_string() << std::endl;
        std::cout << get_array_key_from_context(ctx_cpu) << std::endl;
    }

    
    TEST(nnabla_core, memory)
    {
        using namespace Is::nbla;
        std::string device_id {"cpu"};
        size_t bytes {64};
        size_t second_start_offset = 40;

        // 空メモリ
        CpuMemory mem = CpuMemory(bytes, device_id);
        show_status_memory(&mem);
        
        // メモリ確保
        mem.alloc();
        show_status_memory(&mem);

        // メモリ分割
        auto sub_mem = mem.divide(second_start_offset);
        printf("Memory ->\n");
        show_status_memory(&mem);
        printf("SubMemory ->\n");
        show_status_memory(sub_mem.get());

        // メモリ統合
        mem.try_merge(sub_mem.get());
        printf("SubMemory ->\n");
        show_status_memory(sub_mem.get());
        printf("Memory ->\n");
        show_status_memory(&mem);
    }


    TEST(nnabla_core, allocator)
    {
        using namespace Is::nbla;
        using memory_type = CpuMemory;
        string device_id{ "cpu" };
        size_t bytes{ 64 };

        AllocatorMemory alloc_mem;
        std::shared_ptr<NaiveAllocator<memory_type>> 
            allocator(new NaiveAllocator<memory_type>());
        
        alloc_mem = allocator->alloc(bytes, device_id);
    }


    TEST(nnabla_core, caching_allocator)
    {
        using namespace Is::nbla;
        using memory_type = CpuMemory;
        string device_id{ "cpu" };

        std::shared_ptr<CachingAllocatorWithBuckets<memory_type>> 
            allocator = std::make_shared<CachingAllocatorWithBuckets<memory_type>>();

        // 初めてのメモリ生成(小)では, 内部で1MBのメモリプールが生成されて、512B単位で必要な容量だけ切り崩している
        // 初めてのメモリ生成(大)では, 内部で生成したメモリ数だけ確保する
        AllocatorMemory alloc_mem_small_1 = allocator->alloc(64, device_id);
        AllocatorMemory alloc_mem_small_2 = allocator->alloc(530, device_id);
        AllocatorMemory alloc_mem_large_1 = allocator->alloc((1 << 20) + 100, device_id);

        // キャッシュされているメモリを表示
        std::cout << "Device_Memory_Used_In_Bytes: " 
                << allocator->device_memory_used_in_bytes(device_id)
                << std::endl;
        std::cout << std::endl;

        // キャシュされている不要なメモリを削除
        allocator->free_unused_device_caches(device_id);

        // キャッシュされているメモリを表示
        std::cout << "Device_Memory_Used_In_Bytes: "
                << allocator->device_memory_used_in_bytes(device_id)
                << std::endl;
        std::cout << std::endl;

        // alloc_mem_small_2に対応するMemoryを破棄
        allocator->free(alloc_mem_small_2.memory());

        // キャッシュされているメモリを表示
        std::cout << "Device_Memory_Used_In_Bytes: "
                << allocator->device_memory_used_in_bytes(device_id)
                << std::endl;
    }


    TEST(nnabla_core, array)
    {
        using namespace Is::nbla;
        using byte = unsigned char;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");
        Size_t array_size = 512;

        shared_ptr<Array> cpu_array_1 = shared_ptr<Array>(
            ArrayCreator::create(array_size, dtypes::FLOAT, ctx_cpu));

        std::printf("cpu_array_1: pointer -> %p\n", cpu_array_1->pointer<float>());
        std::printf("cpu_array_1: const_pointer -> %p\n", cpu_array_1->const_pointer<float>());
        std::printf("cpu_array_1: dtype -> %d\n", cpu_array_1->dtype());
        std::printf("cpu_array_1: size -> %d\n", cpu_array_1->size());
        std::cout << "cpu_array_1: context -> " << cpu_array_1->context().to_string() << std::endl;

        Context ctx_cpu_cache({"cpu:float"}, "CpuCachedArray", "0");
        shared_ptr<Array> cpu_cached_array_1 = shared_ptr<Array>(
            ArrayCreator::create(array_size, dtypes::FLOAT, ctx_cpu_cache));

        std::cout << "cpu_cached_array_1: context -> " << cpu_cached_array_1->context().to_string() << std::endl;

        cpu_cached_array_1->zero();
    }


    TEST(nnabla_core, synced_array)
    {
        using namespace Is::nbla;
        using byte = unsigned char;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");
        Size_t array_size = 512;

        auto sync_array_1 = std::make_shared<SyncedArray>(array_size);
        sync_array_1->zero();
        sync_array_1->cast(dtypes::UBYTE, ctx_cpu);
        std::cout << "sync_array_1: head_array_class -> " << sync_array_1->head_array_class() << std::endl;
        std::cout << "sync_array_1: get_num_arrays -> " << sync_array_1->get_num_arrays() << std::endl;
        std::cout << "sync_array_1: size -> " << sync_array_1->size() << std::endl;
        std::cout << "sync_array_1: modification_count -> " << sync_array_1->modification_count() << std::endl;
    }


    TEST(nnabla_core, ndarray)
    {
        using namespace Is::nbla;
        using byte = unsigned char;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");
        Size_t array_size = 512;

        auto ndarr_1 = NdArray::create(Shape_t{3, 16, 16});
        ndarr_1->fill(128);
        ndarr_1->cast(dtypes::DOUBLE, ctx_cpu);
        show_ndarray_contents<double>(ctx_cpu, ndarr_1);

        std::cout << " test zeros ---------------" << std::endl;

        auto ndarray_zeros = zeros<byte>(ctx_cpu, Shape_t{3, 16, 16});
        show_ndarray_contents<double>(ctx_cpu, ndarray_zeros);

        std::cout << " test ones ---------------" << std::endl;

        auto ndarray_ones = ones<byte>(ctx_cpu, Shape_t{3, 16, 16});
        show_ndarray_contents<double>(ctx_cpu, ndarray_ones);

        std::cout << " test eye ---------------" << std::endl;

        auto ndarray_eye = eye<byte>(ctx_cpu, 3, 5);
        show_2d_array<double>(ctx_cpu, ndarray_eye);

        auto ndarray_eye2 = eye<byte>(ctx_cpu, 5, 3);
        show_2d_array<double>(ctx_cpu, ndarray_eye2);
    }


    TEST(nnabla_func, randn)
    {
        using namespace Is::nbla;
        using byte = unsigned char;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");

        float mu = 0.0;
        float sigma = 1.0;
        int seed = 2021;
        Shape_t shape = {3, 5, 5};
        auto ndarray_out_d = randn<double>(ctx_cpu, shape, mu, sigma, seed);
        show_ndarray_contents<double>(ctx_cpu, ndarray_out_d);
        std::cout << ndarray_out_d->get_data_pointer<double>(ctx_cpu)[0] << std::endl;

        auto ndarray_out_f = randn<float>(ctx_cpu, shape, mu, sigma, seed);
        show_ndarray_contents<float>(ctx_cpu, ndarray_out_f);
        std::cout << ndarray_out_f->get_data_pointer<float>(ctx_cpu)[0] << std::endl;
    }


    TEST(nnabla_func, add_scalar)
    {
        using namespace Is::nbla;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");

        // zero
        auto ndarray_zeros = zeros<float>(ctx_cpu, Shape_t{3, 32, 32});
        show_ndarray_contents<float>(ctx_cpu, ndarray_zeros);

        auto out_ndarray = add_scalar<float>(ctx_cpu, ndarray_zeros, 25.5); // 80ms (3, 2000, 2000)

        // 118ms (3, 2000, 2000)
        // auto shape = ndarray_zeros->shape();
        // auto strides = ndarray_zeros->strides();
        // float* data = ndarray_zeros->cast_data_and_get_pointer<float>(ctx_cpu);
        // for (int c = 0; c < shape[0]; ++c)
        // {
        //     for (int y = 0; y < shape[1]; ++y)
        //     {
        //         for (int x = 0; x < shape[2]; ++x)
        //         {
        //             data[c * strides[0] + y * strides[1] + x * strides[2]] = 25.5;
        //         }
        //     }
        // }

        show_ndarray_contents<float>(ctx_cpu, out_ndarray);
    }


    TEST(nnabla_func, mul_scalar)
    {
        using namespace Is::nbla;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");

        // ones
        auto ndarray_ones = ones<float>(ctx_cpu, Shape_t{3, 256, 256});
        // show_ndarray_contents<float>(ctx_cpu, ndarray_ones);

        auto out_ndarray = mul_scalar<float>(ctx_cpu, ndarray_ones, 63.2);
        // show_ndarray_contents<float>(ctx_cpu, out_ndarray);
    }


    TEST(dummy, naive_mul)
    {
        int channels = 3;
        int height = 256;
        int width = 256;
        size_t datasize = sizeof(float) * channels * height * width;
        float* data = new float[datasize];
        std::memset(data, 1, datasize);

        for (int c = 0; c < channels; ++c)
        {
            for (int y = 0; y < height; ++y)
            {
                for (int x = 0; x < width; ++x)
                {
                    data[c * channels + y * height + x * width] *= 55.5;
                }
            }
        }

        delete[] data;
    }


    TEST(nnabla_func, transpose)
    {
        using namespace Is::nbla;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");

        // ones
        auto ndarray_ones = ones<float>(ctx_cpu, Shape_t{3, 16, 16});
        show_ndarray_contents<float>(ctx_cpu, ndarray_ones);

        auto out_ndarray = transpose<float>(ctx_cpu, ndarray_ones, {1, 0 ,2});
        show_ndarray_contents<float>(ctx_cpu, out_ndarray);

        auto out_shape = out_ndarray->shape();
        std::printf("transposed: (%ld, %ld, %ld)\n",out_shape[0], out_shape[1], out_shape[2]);
    }

    TEST(nnabla_func, sum)
    {
        using namespace Is::nbla;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");

        // ones
        auto ndarray_ones = ones<float>(ctx_cpu, Shape_t{3, 16, 16});
        show_ndarray_contents<float>(ctx_cpu, ndarray_ones);

        auto out_ndarray = sum<float>(ctx_cpu, ndarray_ones, 0);
        auto out_shape = out_ndarray->shape();
        show_2d_array<float>(ctx_cpu, out_ndarray);
        std::printf("sumed: size:%ld (%ld, %ld)\n", out_shape.size(), out_shape[0], out_shape[1]);
    }


    TEST(nnabla_func, reshape)
    {
        using namespace Is::nbla;
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");

        // ones
        auto ndarray_ones = ones<float>(ctx_cpu, Shape_t{3, 16, 16});
        show_ndarray_contents<float>(ctx_cpu, ndarray_ones);

        
    }
}




int main(int, char**)
{
    std::cout << "TEST nnabla" << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}