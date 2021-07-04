#pragma once

#include <nbla/cpu.hpp>
#include <nbla/function.hpp>
#include <nbla/function_registry.hpp>

#include <random>


namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(Randn, float, float, const vector<int>&, int)
        // float: Mean
        // float: Sigma
        // const vector<int>&: Shape
        // int: Seed
        
        template <typename T>
        class Randn : public BaseFunction<float, float, const vector<int>&, int>
        {
        protected:
            float mu_;
            float sigma_;
            const vector<int> shape_;
            int seed_;
            std::mt19937 rgen_;

        public:
            Randn(const Context& ctx, float mu, float sigma, const vector<int>& shape, int seed)
                : BaseFunction<float, float, const vector<int>&, int>(ctx, mu, sigma, shape, seed)
                , mu_(mu)
                , sigma_(sigma)
                , shape_(shape)
                , seed_(seed)
            {
                NBLA_CHECK(sigma != 0,
                           error_code::value,
                           "`sigma` must not be zero. (given %f).",
                           sigma);
            }

            virtual ~Randn() {}

            virtual shared_ptr<Function> copy() const 
            {
                return create_Randn(ctx_, mu_, sigma_, shape_, seed_);
            }

            virtual vector<dtypes> in_types() { return vector<dtypes>{}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{ get_dtype<T>() }; }
            virtual int min_inputs() { return 0; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "Randn"; }
            
            virtual vector<string> allowed_array_classes()
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs, const NdArrays& outputs);
            NBLA_API virtual void execute_impl(const NdArrays& inputs, const NdArrays& outputs);

        };
    }
}