#pragma once

#include <nbla/cpu.hpp>
#include <nbla/function.hpp>
#include <nbla/function_registry.hpp>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Arange, double, double, double)
        // start
        // stop
        // step


        /**
         * @brief Generate a range of values within the half-open interval 
         * `[start, stop)`` (the interval including start but excluding stop) with `step` increments.
         * 
         * Inputs:
         * - None
         * 
         * Outputs:
         * - 1-D Array.
         * 
         * @param T Data type for computation.
         * @param start Start value.
         * @param stop End value.
         * @param step Step value.
         */
        template <typename T>
        class Arange : public BaseFunction<double, double, double>
        {
        protected:
            double start_;
            double stop_;
            double step_;
        public:
            Arange(const Context &ctx, double start, double stop, double step)
                : BaseFunction(ctx, start, stop, step)
                , start_(start)
                , stop_(stop)
                , step_(step) {}

            virtual ~Arange() {}
            virtual shared_ptr<Function> copy() const 
            {
                return create_Arange(ctx_, start_, stop_, step_);
            }

            virtual int min_inputs() { return 0; }
            virtual int min_outputs() { return 1; }
            virtual vector<dtypes> in_types() { return vector<dtypes>{}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<string> allowed_array_classes()
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }
            virtual string name() { return "Arange"; }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}