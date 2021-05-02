#include "nbla/context.hpp"
#include "nbla/memory/cpu_memory.hpp"
#include "nbla/memory/allocator.hpp"
#include "nbla/memory/naive_allocator.hpp"
#include "nbla/memory/caching_allocator_with_buckets.hpp"
#include "nbla/array.hpp"
#include "nbla/array_registry.hpp"
#include "nbla/nd_array.hpp"

#include <iostream>
#include <string>
#include <cstdio>

auto property = [](const std::string& test_name, auto&& func, auto&&... args) {
    std::cout << "[ENTER] " << test_name << "---------------" << std::endl;

    std::forward<decltype(func)>(func)(std::forward<args>(args)...);

    std::cout << "[EXIT] " << test_name << "---------------" << std::endl;
    std::cout << std::endl;
};

void test_context()
{
    using namespace Is::nbla;
    using byte = unsigned char;
    Context ctx_cpu;

    std::cout << ctx_cpu.array_class << std::endl;
    std::cout << ctx_cpu.device_id << std::endl;
    std::cout << ctx_cpu.to_string() << std::endl;
    std::cout << get_array_key_from_context(ctx_cpu) << std::endl;
}


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
}


void test_memory()
{
    using namespace Is::nbla;
    using byte = unsigned char;

    std::string device_id {"cpu"};
    size_t bytes {64};
    size_t second_start_offset = 40;

    // 空メモリ
    CpuMemory mem = CpuMemory(bytes, device_id);
    show_status_memory(&mem);
    std::cout << std::endl;

    // メモリ確保
    mem.alloc();
    show_status_memory(&mem);
    std::cout << std::endl;

    // メモリ分割
    auto sub_mem = mem.divide(second_start_offset);
    printf("Memory ->\n");
    show_status_memory(&mem);
    printf("SubMemory ->\n");
    show_status_memory(sub_mem.get());
    std::cout << std::endl;

    // メモリ統合
    mem.try_merge(sub_mem.get());
    printf("SubMemory ->\n");
    show_status_memory(sub_mem.get());
    printf("Memory ->\n");
    show_status_memory(&mem);
    std::cout << std::endl;
}


void test_allocator()
{
    using namespace Is::nbla;
    using byte = unsigned char;
    using memory_type = CpuMemory;
    string device_id{ "cpu" };
    size_t bytes{ 64 };

    AllocatorMemory alloc_mem;
    std::shared_ptr<NativeAllocator<memory_type>> 
        allocator(new NativeAllocator<memory_type>());
    
    alloc_mem = allocator->alloc(bytes, device_id);
}


void test_caching_allocator()
{
    using namespace Is::nbla;
    using byte = unsigned char;
    using memory_type = CpuMemory;
    string device_id{ "cpu" };
    size_t bytes{ 64 };

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


void test_array()
{
    using namespace Is::nbla;
    using byte = unsigned char;
    string device_id{ "cpu" };
    Context ctx_cpu({"cpu:float"}, "CpuArray", "0");
    Size_t array_size = 512;

    shared_ptr<Array> cpu_array_1 = shared_ptr<Array>(
            ArrayCreator::create(array_size, dtypes::FLOAT, ctx_cpu));

    std::printf("cpu_array_1: pointer -> %p\n", cpu_array_1->pointer<float>());
    std::printf("cpu_array_1: const_pointer -> %p\n", cpu_array_1->const_pointer<float>());
    std::printf("cpu_array_1: dtype -> %d\n", cpu_array_1->dtype());
    std::printf("cpu_array_1: size -> %d\n", cpu_array_1->size());
    std::cout << "cpu_array_1: context -> " << cpu_array_1->context().to_string() << std::endl;

    Context ctx_cpu_cache({ "cpu:float" }, "CpuCachedArray", "0");
        shared_ptr<Array> cpu_cached_array_1 = shared_ptr<Array>(
            ArrayCreator::create(array_size, dtypes::FLOAT, ctx_cpu_cache));

    std::cout << "cpu_cached_array_1: context -> " << cpu_cached_array_1->context().to_string() << std::endl;

    cpu_cached_array_1->zero();
}


void test_synced_array()
{
    using namespace Is::nbla;
    using byte = unsigned char;
    string device_id{ "cpu" };
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


template <typename T>
void show_ndarray_contents(const Is::nbla::Context& ctx, 
                           std::shared_ptr<Is::nbla::NdArray> ndarray)
{
    using namespace Is::nbla;
    Shape_t shape = ndarray->shape();
    Stride_t strides = ndarray->strides();
    Size_t size = ndarray->size();
    Size_t ndim = ndarray->ndim();
    auto synced_array = ndarray->array();
    const T* data = static_cast<const T*>(synced_array->data_ptr(get_dtype<T>(), ctx));
    

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



void test_ndarray()
{
    using namespace Is::nbla;
    using byte = unsigned char;
    string device_id{ "cpu" };
    Context ctx_cpu({"cpu:float"}, "CpuArray", "0");
    Size_t array_size = 512;

    auto ndarr_1 = NdArray::create(Shape_t{3,16,16});
    ndarr_1->fill(128);
    ndarr_1->cast(dtypes::DOUBLE, ctx_cpu);
    show_ndarray_contents<double>(ctx_cpu, ndarr_1);
}


void test_isndarray()
{
    using namespace Is::nbla;
    using byte = unsigned char;
    Context ctx_cpu;

    auto isnd_ones= IsNdArray::ones<byte>(ctx_cpu, Shape_t{3, 8, 8});
    show_ndarray_contents<int>(ctx_cpu, isnd_ones->data());

    auto isnd_identity = IsNdArray::identity<byte>(ctx_cpu, Shape_t{3, 5, 8});
    show_ndarray_contents<int>(ctx_cpu, isnd_identity->data());
}


int main(int argc, char** argv)
{
    std::cout << "Hello, from IsNdarrayLibrary!\n";
    
    property("test context", test_context);
    property("test memory", test_memory);
    property("test_allocator", test_allocator);
    property("test_caching_allocator", test_caching_allocator);
    property("test_array", test_array);
    property("test_synced_array", test_synced_array);
    property("test_ndarray", test_ndarray);
    property("test_isndarray", test_isndarray);

    return 0;
}
