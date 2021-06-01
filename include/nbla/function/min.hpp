#pragma once

#include <nbla/cpu.hpp>
#include <nbla/function.hpp>
#include <nbla/function/max.hpp>
#include <nbla/function_registry.hpp>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Min, const vector<int>&, bool, bool, bool)
        // axes
        // keep_dims
        // with_index
        // only_index


        /**
         * @brief Reduction along axes with min operation.
         * 
         * Inputs:
         * - N-D array
         * 
         * Outputs:
         * - one or two N-D arrays
         * 
         * @param T Data type for computation.
         * @param axes A list of axes to be reduced.
         * @param keep_dims Flag whether the reduced axes are kept.
         * @param with_index Return sorted values and index.
         * @param only_index Return only the sort index.
         * 
         */
        template <typename T> 
        class Min : public Max<T> 
        {
        public:
            Min(const Context &ctx, const vector<int> &axes, bool keep_dims, bool with_index, bool only_index)
                : Max<T>(ctx, axes, keep_dims, with_index, only_index) {}
            
            virtual ~Min() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Min(this->ctx_, this->axes_, this->keep_dims_,
                                this->with_index_, this->only_index_);
            }

            virtual string name() { return "Min"; }

        protected:
            NBLA_API virtual void execute_impl_reduce(const T* x, T* y, int outer_size, int reduction_size);
        };
    }
}