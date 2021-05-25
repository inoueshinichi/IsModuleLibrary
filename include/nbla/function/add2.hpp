#pragma once

#include <nbla/cpu.hpp>
#include <nbla/function.hpp>
#include <nbla/function_registry.hpp>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Add2, bool)

        /**
         * @brief Elementwise Add
         * y_i = x0_i + x1_i
         * 
         * Inputs:
         *   - N-D array.
         *   - N-D array.
         * Outputs:
         *   - N-D array.
         */
        template <typename T>
        class Add2 : public BaseFunction<bool>
        {
        protected:
            bool inplace_;

        public:
            Add2(const Context& ctx, bool inplace)
                : BaseFunction(ctx, inplace)
                , inplace_(inplace) {}

            virtual ~Add2() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Add2(ctx_, inplace_);
            }

            virtual vector<dtypes> in_types() 
            {
                return vector<dtypes>{get_dtype<T>(), get_dtype<T>()};
            }

            virtual vector<dtypes> out_types() 
            { 
                return vector<dtypes>{get_dtype<T>()}; 
            }

            virtual int min_inputs() { return 2; }

            virtual int min_outputs() { return 1; }

            virtual string name() { return "Add2"; }

            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

            virtual int inplace_data(int i) const 
            {
                if (this->fall_back_func_ || !inplace_ || i > 0)
                    return Function::NOT_INPLACE;
                return Function::INPLACE;
            }

            virtual int inplace_data_with(int i) const 
            {
                // 0 is okay because never be called in the case of i != 0.
                return 0;
            }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}