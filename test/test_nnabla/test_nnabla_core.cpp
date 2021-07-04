// googletest
#include <gtest/gtest.h>

// nnabla core
#include <nbla/context.hpp>
#include <nbla/global_context.hpp>
#include <nbla/memory/cpu_memory.hpp>
#include <nbla/memory/allocator.hpp>
#include <nbla/memory/naive_allocator.hpp>
#include <nbla/memory/caching_allocator_with_buckets.hpp>
#include <nbla/array.hpp>
#include <nbla/array_registry.hpp>
#include <nbla/synced_array.hpp>
#include <nbla/nd_array.hpp>
#include <nbla/nd_array_extra.hpp>

// utility
#include <test_utils.hpp>

#include <iostream>
#include <string>
#include <cstdio>
#include <iomanip>
#include <cstring>

using namespace Is::nbla;


namespace 
{
    TEST(nnabla_core, context)
    {
        Context ctx_cpu;
        std::cout << ctx_cpu.array_class << std::endl;
        std::cout << ctx_cpu.device_id << std::endl;
        std::cout << ctx_cpu.to_string() << std::endl;
        std::cout << get_array_key_from_context(ctx_cpu) << std::endl;
    }


    TEST(nnabla_core, memory)
    {
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
        string device_id{"cpu"};
        Context ctx_cpu({"cpu:float"}, "CpuArray", "0");
        Size_t array_size = 512;

        shared_ptr<Array> cpu_array_1 = shared_ptr<Array>(
            ArrayCreator::create(array_size, dtypes::FLOAT, ctx_cpu));

        std::printf("cpu_array_1: pointer -> %p\n", cpu_array_1->pointer<float>());
        std::printf("cpu_array_1: const_pointer -> %p\n", cpu_array_1->const_pointer<float>());
        std::printf("cpu_array_1: dtype -> %d\n", cpu_array_1->dtype());
        std::printf("cpu_array_1: size -> %ld\n", cpu_array_1->size());
        std::cout << "cpu_array_1: context -> " << cpu_array_1->context().to_string() << std::endl;

        Context ctx_cpu_cache({"cpu:float"}, "CpuCachedArray", "0");
        shared_ptr<Array> cpu_cached_array_1 = shared_ptr<Array>(
            ArrayCreator::create(array_size, dtypes::FLOAT, ctx_cpu_cache));

        std::cout << "cpu_cached_array_1: context -> " << cpu_cached_array_1->context().to_string() << std::endl;

        cpu_cached_array_1->zero();
    }


    TEST(nnabla_core, synced_array)
    {
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
        using byte = unsigned char;
        string device_id{"cpu"};
        const auto& ctx_cpu = SingletonManager::get<GlobalContext>()->get_current_context();

        auto ndarr_1 = NdArray::create(Shape_t{3, 16, 16});
        ndarr_1->fill(128);
        ndarr_1->cast(dtypes::DOUBLE, ctx_cpu);
        show_ndarray_contents<float>(ndarr_1);

        std::cout << " test zeros ---------------" << std::endl;

        auto ndarray_zeros = zeros<byte>(Shape_t{3, 16, 16});
        show_ndarray_contents<float>(ndarray_zeros);

        std::cout << " test ones ---------------" << std::endl;

        auto ndarray_ones = ones<byte>(Shape_t{3, 16, 16});
        show_ndarray_contents<float>(ndarray_ones);

        std::cout << " test eye ---------------" << std::endl;

        auto ndarray_eye = eye<byte>(3, 5);
        show_ndarray_contents<float>(ndarray_eye);

        auto ndarray_eye2 = eye<byte>(5, 3);
        show_ndarray_contents<float>(ndarray_eye2);
    }
}


int main(int, char**)
{
    std::cout << "TEST nnabla core" << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}