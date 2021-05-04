#include <nbla/array.hpp>
#include <nbla/function/randn.hpp>
#include <nbla/random_manager.hpp>
#include <nbla/nd_array.hpp>

namespace Is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_SOURCE(Randn, float, float, const Shape_t&, int)
        // float: Mean
        // float: Sigma
        // const Shape_t&: Shape
        // int: Seed

        template <typename T>
        void Randn<T>::setup_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            outputs[0]->reshape(Shape_t(shape_.cbegin(), shape_.cend()), true);
            rgen_ = std::mt19937((seed_ == -1 ? std::random_device()() : seed_));
        }


        template <typename T>
        void Randn<T>::execute_impl(const NdArrays& inputs, const NdArrays& outputs)
        {
            // ガウス分布
            std::normal_distribution<typename force_float<T>::type> rdist(mu_, sigma_);

            std::mt19937& rgen = 
                seed_ == -1 ? SingletonManager::get<RandomManager>()->get_rand_generator() // Globalなrgen
                            : rgen_;

            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, true);
            for (Size_t s = 0; s < outputs[0]->size(); ++s)
            {
                y[s] = (T)rdist(rgen);
            } 
        }


        // dll export
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Randn, float)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Randn, double)
        NBLA_INSTANTIATE_FUNCTION(NBLA_API, Randn, long double)
        
    }
}