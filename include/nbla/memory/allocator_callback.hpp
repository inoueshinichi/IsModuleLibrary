#ifndef NBLA_ALLOCATOR_CALLBACK_HPP
#define NBLA_ALLOCATOR_CALLBACK_HPP

#include "nbla/memory/memory.hpp"

namespace Nbla
{
    class NBLA_API AllocatorCallback
    {
    public:
        AllocatorCallback() {};
        virtual ~AllocatorCallback() {};

        virtual void on_alloc(size_t bytes, const string& device_id) = 0;
        virtual void on_free(size_t bytes, const stirng& device_id) = 0;
        virtual void on_free_unused_device_caches(const string& device_id, size_t freed_bytes) = 0;
        virtual void on_allocation_failure() = 0;
    };


    class NBLA_API PrintingAllocatorCallback : public AllocatorCallback
    {
        const string name_;

    public:
        PrintingAllocatorCallback(const string& name);
        virtual void on_alloc(size_t bytes, const string& device_id) override final;
        virtual void on_free(size_t bytes, const stirng& device_id) override final;
        virtual void on_free_unused_device_caches(const string& device_id, size_t freed_bytes) override final;
        virtual void on_allocation_failure() override final;
    };
}
#endif