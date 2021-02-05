#ifndef IS_NBLA_MEMORY_HPP
#define IS_NBLA_MEMORY_HPP

#include "nbla/defs.hpp"

#include <cstddef>
#include <memory>
#include <string>

namespace Is
{
    namespace nbla
    {
        using std::string;
        using std::size_t;
        using std::shared_ptr;

        class NBLA_API Memory
        {
            size_t bytes_{0};
            string device_id_;
            bool locked_{false};
            Memory* next_{nullptr};
            Memory* prev_{nullptr};

            inline void disable() { ptr_ = nullptr; }

        protected:
            void* ptr_{nullptr};

        public:
            Memory(size_t bytes, const string& device_id);
            virtual ~Memory();

            inline size_t bytes() const { return bytes_; }

            inline string device_id() const { return device_id_; }

            inline void* pointer() { return ptr_; }

            inline const void* const_pointer() const { return ptr_; }

            inline bool locked() const { return locked_; }

            inline bool disabled() { return !ptr_; }

            inline Memory* next() const { return next_; }

            inline Memory* prev() const { return prev_; }

            inline void lock() { locked_ = true; }

            inline void release() { locked = false; }

            void alloc();

            size_t bytes_active();

            shared_ptr<Memory> divide(size_t second_start);

            void try_merge(Memory* from);

            static void associate_consecutive(Memory* left, Memory* right);

        protected:
            virtual bool alloc_impl() = 0;
            
            virtual shared_ptr<Memory> divide_impl(size_t second_start) = 0;

            virtual void merge_next_impl(Memory* from) = 0;

            virtual void merge_prev_impl(Memory* from) = 0;
        };
    }
}
#endif