
#include "nbla/exception.hpp"
#include "nbla/memory/memory.hpp"

#include <algorithm>
#include <typeinfo>

namespace Is
{
    namespace Nbla
    {
        Memory::Memory(size_t bytes, const string& device_id)
            : bytes_(std::max(bytes, (size_t)1)), device_id_(device_id) 
        {}

        Memory::~Memory()
        {}

        void Memory::alloc()
        {
            NBLA_CHECK(!ptr_, error_code::value, "Allocation called twice.");
            NBLA_CHECK(this->alloc_impl(), error_code::memory, "%s allocation failed.", 
                       typeid(*this).name());
        }

        size_t Memory::bytes_active()
        {
            if (!ptr_ || next_ || prev_)
            {
                return 0;
            }
            return bytes_;
        }

        // static func
        void Memory::associate_consecutive(Memory* left, Memory* right)
        {
            if (left)
            {
                left->next_ = right;
            }

            if (right)
            {
                right->prev_ = left;
            }
        }

        shared_ptr<Memory> Memory::divide(size_t second_start)
        {
            NBLA_CHECK(second_start > 0, error_code::value,
                "`second_start` must be > 0. Given %zd.", second_start);
            NBLA_CHECK(second_start < bytes_, error_code::value,
                "`second_start` must be < bytes_. Given %zd was not %zd.",
                second_start, bytes_);
            
            auto new_mem = this->devide_impl(second_start);
            bytes_ = second_start;

            // thisのnext_をnew_memのnext_にコピー
			// thisのnext_とnew_memのprev_が互いを指すようにする.
            Memory::associate_consecutive(new_mem.get(), this->next_);
            Memory::associate_consecutive(this, new_mem.get());
            return new_mem;
        }

        void Memory::try_merge(Memory* from)
        {
            if (!from || from->locked())
            {
                // 使用中のメモリ. 
                // CacheAllocatorがMemoryインスタンスをラップするAllocatorMemoryを作成したときにセットされる.
                return;
            }

            if (from == next_)
            {
                this->merge_next_impl(from);
                Memory::associate_consecutive(this, from->next_);  // fromインスタンスをリストから除外
            }
            else if (from == prev_)
            {
                this->merge_prev_impl(prev_);
                Memory::associate_consecutive(from->prev_, this); // fromインスタンスをリストから除外
            }

            byte_ += from->bytes_;
            from->disable();  // ptr_はどこでメモリ解放される? shinichi inoue 21/01/24.
        }
    }
}
