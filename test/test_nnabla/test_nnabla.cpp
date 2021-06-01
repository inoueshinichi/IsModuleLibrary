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

// nnabla func
#include <nbla/function/randn.hpp>



#include <iostream>
#include <string>
#include <cstdio>
#include <iomanip>


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
}




int main(int, char**)
{
    std::cout << "TEST nnabla" << std::endl;
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}