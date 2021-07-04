#pragma once

#ifndef NBLA_BROADCAST_MAX_DIM
    #define NBLA_BROADCAST_MAX_DIM 8
#endif

#include <nbla/cpu.hpp>
#include <nbla/function.hpp>
#include <nbla/function_registry.hpp>


namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Broadcast, const vector<int>&)

        /**
         * @brief Broadcast N-D array to shape.
         * @param shape
         * The shape input array broadcasted to.
         * Dimensions broadcasted in input array must be size one.
         */
        template <typename T>
        class Broadcast : public BaseFunction<const vector<int> &>
        {
        protected:
            const vector<int> shape_;

            NdArray stride_x_;
            NdArray shape_y_;

        public:
            Broadcast(const Context& ctx, const vector<int>& shape)
                : BaseFunction<const vector<int>&>(ctx, shape)
                , shape_(shape) {}

            virtual ~Broadcast() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Broadcast(ctx_, shape_);
            }

            virtual vector<dtypes> in_types() 
            {
                return vector<dtypes>{ get_dtype<T>() };
            }

            virtual vector<dtypes> out_types()
            {
                return vector<dtypes>{ get_dtype<T>() };
            }

            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Broadcast"; }

            virtual vector<string> allowed_array_classes()
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };   
    }
}