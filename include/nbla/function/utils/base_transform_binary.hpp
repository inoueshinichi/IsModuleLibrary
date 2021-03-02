#ifndef IS_NBLA_FUNCTION_BASE_TRANSFORM_BINARY_HPP
#define IS_NBLA_FUNCTION_BASE_TRANSFORM_BINARY_HPP

#include "nbla/cpu.hpp"
#include "nbla/function.hpp"
#include "nbla/function/broadcast.hpp"
#include "nbla/function_registry.hpp"
#include "nbla/imperative.hpp"

namespace Is
{
    namespace nbla
    {
        template <typename... Args>
        class BaseTransformBinary : public BaseFunction<Args...>
        {
        protected:
            shared_ptr<Function> f_bc0_, fb1_;
            bool inplace_;

        public:
            BaseTransformBinary(const Context& ctx, bool inplace, Args... args)
                : BaseFunction<Args...>(ctx, args...)
                , inplace_(inplace) {}
            
            virtual int min_inputs() override { return 2; }

            virtual int min_outputs() override { return 2; }

            virtual int inplace_data(int i) override 
            {
                if (!inplace_ || i > 0)
                {
                    return Function::NOT_INPLACE;
                }
                return Function::INPLACE;
            }

            virtual int inplace_data_with(int i) const override
            {
                // 0 is okay because neber be called in the case of i != 0.
                return 0;
            }

        protected:
            virtual void setup_impl(const IsNdArrays& inputs, const IsNdArrays& outputs)
            {
                int ndim = inputs[0]->ndim();
                NBLA_CHECK(ndim == inputs[1]->ndim(), error_code::value,
                           "Dimensions of inputs must match. "
                           "inputs[0]: %d != inputs[1]: %d.",
                           ndim, inputs[1]->ndim());
                Shape_t s0 = inputs[0]->shape();
                Shape_t s1 = inputs[1]->shape();
                Shape_t oshape(ndim);
                bool bc0 = false;
                bool bc1 = false;
                for (int i = 0; i < ndim; ++i)
                {
                    if (s0[i] != s1[i])
                    {
                        NBLA_CHECK(std::min(s0[i], s1[i]) == 1, error_code::value,
                                   "Broadcast dimension must be one. shape[%d]: %d.",
                                   i, std::min(s[i], s1[i]));
                        if (s0[i] == 1)
                        {
                            bc0 = true;
                        }
                        if (s1[i] == 1)
                        {
                            bc1 = true;
                        }
                    }
                    oshape[i] = std::max(s0[i], s1[i]);
                }
                outputs[0]->reshape(oshape, true);

                // check in-place conditions
                if (inplace_)
                {
                    NBLA_CHECK(s0 == oshape, error_code::value,
                               "%s: Shapes of inputs[0] and output must match when "
                               "`inplace == true`.",
                               this->name().c_str());
                    outputs[0]->data()->set_array(inputs[0]->data()->array());
                }

                if (bc0)
                {
                    f_bc0_ = create_Broadcast(this->ctx_, vector<int>(oshape.cbegin(), oshape.cend()));
                }
                if (bc1)
                {
                    f_bc1_ = create_Broadcast(this->ctx_, vector<int>(oshape.cbegin(), oshape.cend());
                }
            }
        };


        /**
         * @brief 2入力1出力関数用コマンドパターンの雛形
         * 演算操作は状態を持つポリシークラス
         * 
         * @tparam T 
         * @tparam BinaryOp 
         * @tparam Args 
         */
        template <typename T, typename BinaryOp, typename... Args>
        class TransformBinary : public BaseTransformBinary<Args...>
        {
        protected:
            BinaryOp binary_op_;
        
        public:
            TransformBinary(const Context& ctx, bool inplace, Args... args)
                : BaseTransformBinary<Args...>(ctx, inplace, args...)
                , binary_op_(args...) {}

            virtual ~TransformBinary() {}

            virtual vector<dtypes> in_types() override
            {
                return vector<dtypes>{get_dtype<T>(), get_dtype<T>()};
            }

            virtual vector<dtypes> out_types() override
            {
                return vector<dtypes>{get_dtype<T>();}
            }

            virtual vector<string> allowed_array_classes()
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

        protected:
            // virtual void forward_impl(const Variables& inputs, const Variables& outputs);
            // virtual void backward_impl(const Variables& inputs, const Variables& outputs,
            //                            const vector<bool>& propagate_down,
            //                            const vector<bool>& accum);
            virtual void execute_impl(const IsNdArrays& inputs, const IsNdArrays& outputs);
        };


        /*引数2個の演算操作のポリシークラスのインターフェース*/
        class BaseBinaryOp
        {
        public:
            inline BaseBinaryOp() {}

            template <typename T> inline
            T operator()(const T x0, const T x1)
            {
                // NBLA_ERROR(error_code::not_implemented,
                //            "Forward operation is not implemented.");
                NBLA_ERROR(error_code::not_implemented,
                           "Execute operation is not implemented.");
            }

            // template <typename T> inline
            // T g0(const T dy, const T x0, const T x1, const T y, const bool inplace)
            // {
            //     NBLA_ERROR(error_code::not_implemented,
            //                "Backward operation for input 0 is not implemented.");
            // }

            // template <typename T> inline
            // T g1(const T dy, const T x0, const T x1, const T y, const bool inplace)
            // {
            //     NBLA_ERROR(error_code::not_implemented,
            //                "Backward operation for input 1 is not implemented.");
            // }
        };


        template <typename T, typename BinaryOp>
        void transform_binary(Size_t size, const T* x0, const T* x1, BinaryOp op)
        {
            for (Size_t idx = 0; idx < size; ++idx)
            {
                y[idx] = op(x0[idx], x1[idx]);
            }
        }


        template <typename T, typename BinaryOp, bool accum>
        void transform_binary_grad0(Size_t size, const T* dy, const T* x0, const T* x1,
                                    const T* y, T* g0, bool inplace, BinaryOp op)
        {
            for (Size_t idx = 0; idx < size; ++idx)
            {
                g0[idx] = (accum ? g0[idx] : (T)0) + op.g0(dy[idx], x0[idx], x1[idx], y[idx], inplace);
            }
        }


        template <typename T, typename BinaryOp, bool accum>
        void transform_binary_grad1(Size_t size, const T* dy, const T* x0, const T* x1,
                                    const T* y, T* g1, bool inplace, BinaryOp op)
        {
            for (Size_t idx = 0; idx < size; ++idx)
            {
                g1[idx] = (accum ? g1[idx] : (T)0) + op.g1(dy[idx], x0[idx], y[idx], inplace);
            }
        }


        template <typename T, typename BinaryOp, typename... Args>
        void TransformBinary<T, BinaryOp, Args...>::execute_impl(
            const Variables& inputs, const Variables& outputs)
        {
            auto _get = [this](Variable* v) {
                return v->get_data_pointer<T>(this->ctx_);
            };

            // Broadcast
            Variable o_bc0;
            Variable o_bc1;
            if (this->f_bc0_)
            {
                execute(this->f_bc0_, {inputs[0]}, {&o_bc0});
            }
            if (this->f_bc1_)
            {
                execute(this->f_bc1_, {inputs[1]}, {&o_bc1});
            }

            // Binary transform
            const T* x0 = _get((this->f_bc0_) ? (&o_bc0) : (inputs[0]));
            const T* x1 = _get((this->f_bc1_) ? (&o_bc1) : (inputs[1]));
            T* y = outputs[0]->cast_data_get_pointer<T>(this->ctx_, !this->inplace_);
            transform_binary(outputs[0]->size(), x0, x1, y, binary_op_);
        }

        // template <typename T, typename BinaryOp, typename... Args>
        // void TransformBinary<T, BinaryOp, Args...>::forward_impl(
        //     const Variables& inputs, const Variables& outputs)
        // {
        //     auto _get = [this](Variable* v) {
        //         return v->get_data_pointer<T>(this->ctx_);
        //     };

        //     // Broadcast
        //     Variable o_bc0;
        //     Variable o_bc1;
        //     if (this->f_bc0_)
        //     {
        //         execute(this->f_bc0_, {inputs[0]}, {&o_bc0});
        //     }
        //     if (this->f_bc1_)
        //     {
        //         execute(this->f_bc1_, {inputs[1]}, {&o_bc1});
        //     }

        //     // Binary transform
        //     const T* x0 = _get((this->f_bc0_) ? (&o_bc0) : (inputs[0]));
        //     const T* x1 = _get((this->f_bc1_) ? (&o_bc1) : (inputs[1]));
        //     T* y = outputs[0]->cast_data_get_pointer<T>(this->ctx_, !this->inplace_);
        //     transform_binary(outputs[0]->size(), x0, x1, y, binary_op_);
        // }


        // template <typename T, typename BinaryOp, typename... Args>
        // void TransformBinary<T, BinaryOp, Args...>::backward_impl(
        //     const Variables& inputs, const Variables& outputs,
        //     const vector<bool>& propagate_down, const vector<bool>& accum)
        // {
        //     if (!(propagate_down[0] || propagate_down[1]))
        //     {
        //         return;
        //     }

        //     auto _get_data = [this](Variable* v) {
        //         return v->get_data_pointer<T>(this->ctx_);
        //     };
        //     auto _cast_grad = [this](Variable* v, bool wo) {
        //         return v->cast_grad_and_get_pointer<T>(this->ctx_, wo);
        //     };
            
        //     const T* dy = outputs[0]->get_grad_pointer<T>(this->ctx_);
        //     const T* y = _get_data(outputs[0]);
        //     Size_t size = outputs[0]->size();
        //     if (propagate_down[0])
        //     {
        //         // Broadcast
        //         Variable o_bc0;
        //         Variable o_bc1;
        //         if (this->f_bc0_)
        //         {
        //             execute(this->f_bc0_, {inputs[0]}, {&o_bc0});
        //         }
        //         if (this->f_bc1_)
        //         {
        //             execute(this->f_bc1_, {inputs[1]}, {&o_bc1});
        //         }

        //         // Binary transform backward
        //         const T* x0 = _get_data((this->f_bc0_) ? (&o_bc0) : (inputs[0]));
        //         const T* x1 = _get_data((this->f_bc1_) ? (&o_bc1) : (inputs[1]));
        //         T* dx0 = (this->f_bc0_) ? _cast_grad(&o_bc0, true) : _cast_grad(inputs[0], !accum[0]);
        //         if ((!this->f_bc0_) && accum[0])
        //         {
        //             transform_binary_grad0<T, BinaryOp, true>(size, dy, x0, x1, y, dx0,
        //                                                       this->inplace_, binary_op_);
        //         }
        //         else
        //         {
        //             transform_binary_grad0<T, BinaryOp, false>(size, dy, x0, x1, y, dx0,
        //                                                        this->inplace_, binary_op_);
        //         }

        //         // Broadcast backward
        //         if (this->f_bc0_)
        //         {
        //             nbla::backward(this->f_bc0_, Variables{inputs[0]}, Variables{&o_bc0},
        //                            {true}, {accum[0]});
        //         }
        //     } // if (propagate_down[0])

        //     if (propagate_down[1])
        //     {
        //         // Broadcast
        //         Variable o_bc0;
        //         Variable o_bc1;
        //         if (this->f_bc0_)
        //         {
        //             execute(this->f_bc0_, {inputs[0]}, {&o_bc0});
        //         }
        //         if (this->f_bc1_)
        //         {
        //             execute(this->f_bc1_, {inputs[1]}, {&o_bc1});
        //         }

        //         // Binary transform backward
        //         const T* x0 = _get_data((this->f_bc0_) ? (&o_bc0) : (inputs[0]));
        //         const T* x1 = _get_data((this->f_bc1_) ? (&o_bc1) : (inputs[1]));
        //         T* dx1 = (this->f_bc1_) ? _cast_grad(&o_bc1, true) : _cast_grad(inputs[1], !accum[1]);
        //         if ((!this->f_bc1_) && accum[1])
        //         {
        //             transform_binary_grad1<T, BinaryOp, true>(size, dy, x0, x1, y, dx1, this->inplace_, binary_op);
        //         }
        //         else
        //         {
        //             transform_binary_grad1<T, BinaryOp, false>(size, dy, x0, x1, y, dx1, this->inplace_, binary_op);
        //         }

        //         // Broadcast backward
        //         if (this->f_bc1_)
        //         {
        //             nbla::backward(this->f_bc1_, Variables{inputs[1]}, Variables{&o_bc1},
        //                            {true}, {accum[1]});
        //         }
        //     } // if (propagate_down[1])
        // }

/********************** Transform-binary(Op)系クラス作成用ヘルパーマクロ **********************************/

#define NBLA_DEFINE_BINARY_OP_CLASS(NAME)                                                               \
    class NAME##BinaryOp : public BaseBinaryOp

#define NBLA_DEFINE_BINARY_OP_EXECUTE(OP)                                                               \
    template <typename T> inline T operator()(const T x0, const T x1)                                   \
    {                                                                                                   \
        return OP;                                                                                      \
    }

// #define NBLA_DEFINE_BINARY_OP_FORWARD(OP)                                                               \
//     template <typename T> inline T operator()(const T x0, const T x1)                                   \
//     {                                                                                                   \
//         return OP;                                                                                      \
//     }

// #define NBLA_DEFINE_BINARY_OP_BACKWARD(NUM, GOP)                                                        \
//     template <typename T> inline                                                                        \
//     T g##NUM(const T dy, const T x0, const T x1, const T y, const bool inplace)                         \
//     {                                                                                                   \
//         return GOP;                                                                                     \
//     }

#define NBLA_DEFINE_TRANSFORM_BINARY_CLASS_COMMON(NAME, DEP_Y_0, DEP_Y_1)                               \
public:                                                                                                 \
    virtual ~NAME() {}                                                                                  \
    virtual string name() { return #NAME; }                                                             \
    virtual bool grad_depends_output_data(int i, int o) const                                           \
    {                                                                                                   \
        if (i == 0)                                                                                     \
        {                                                                                               \
            return DEP_Y_0;                                                                             \
        }                                                                                               \
        return DEP_Y_1;                                                                                 \
    }

// ----------------------------------------------------------------------------------------------------
// Zero argument
// ----------------------------------------------------------------------------------------------------
#define NBLA_DEFINE_BINARY_OP_NO_GRAD(NAME, OP)                                                         \
    NBLA_DEFINE_BINARY_OP_CLASS(NAME)                                                                   \
    {                                                                                                   \
    public:                                                                                             \
        /*NBLA_DEFINE_BINARY_OP_FORWARD(OP)*/                                                           \
        NBLA_DEFINE_BINARY_OP_EXECUTE(OP)                                                               \
    }

#define NBLA_DEFINE_BINARY_OP(NAME, OP, GOP0, GOP1)                                                     \
    NBLA_DEFINE_BINARY_OP_CLASS(NAME)                                                                   \
    {                                                                                                   \
    public:
        NBLA_DEFINE_BINARY_OP_EXECUTE(OP)
        /*NBLA_DEFINE_BINARY_OP_FORWARD(OP)*/
        /*NBLA_DEFINE_BINARY_OP_BACKWARD(0, GOP0)*/
        /*NBLA_DEFINE_BINARY_OP_BACKWARD(1, GOP1)*/
    }

#define NBLA_DEFINE_TRANSFORM_BINARY_CLASS(NAME, DEP_Y_0, DEP_Y_1)                                      \
    template <typename T>                                                                               \
    class NAME : public TransformBinary<T, NAME##BinaryOp>                                              \
    {                                                                                                   \
        NBLA_DEFINE_TRANSFORM_BINARY_CLASS_COMMON(NAME, DEP_Y_0, DEP_Y_1)                               \
                                                                                                        \
        NAME(const Context& ctx)                                                                        \
            : TransformBinary<T, NAME##BinaryOp>(ctx, false) {}                                         \
                                                                                                        \
        virtual shared_ptr<Function> copy() const                                                       \
        {                                                                                               \
            return create_##NAME(this->ctx_);                                                           \
        }                                                                                               \
    }

#define NBLA_DEFINE_TRANSFORM_BINARY_CLASS_IMPLACE(NAME, DEP_Y_0, DEP_Y_1)                              \
    template <typename T>                                                                               \
    class NAME : public TransformBinary<T, NAME##BinaryOp>                                              \
    {                                                                                                   \
        NBLA_DEFINE_TRANSFORM_BINARY_CLASS_COMMON(NAME, DEP_Y_0, DEP_Y_1)                               \
                                                                                                        \
        NAME(const Context& ctx, bool inplace)                                                          \
            : TransformBinary<T, NAME##BinaryOp>(ctx, inplace) {}                                       \
                                                                                                        \
        virtual shared_ptr<Function> copy() const                                                       \
        {                                                                                               \
            return create_##NAME(this->ctx_, this->inplace_);                                           \
        }                                                                                               \
    }

#define NBLA_DEFINE_TRANSFORM_BINARY_NO_GRAD(NAME, OP)                                                  \
    NBLA_REGISTER_FUNCTION_HEADER(NAME);                                                                \
    NBLA_DEFINE_BINARY_OP_NO_GRAD(NAME, OP);                                                            \
    NBLA_DEFINE_TRANSFORM_BINARY_CLASS(NAME, false, false)

#define NBLA_DEFINE_TRANSFORM_BINARY(NAME, OP, GOP0, GOP1, DEP_Y_0, DEP_Y_1)                            \
    NBLA_REGISTER_FUNCTION_HEADER(NAME);                                                                \
    NBLA_DEFINE_BINARY_OP(NAME, OP, GOP0, GOP1);                                                        \
    NBLA_DEFINE_TRANSFORM_BINARY_CLASS(NAME, DEP_Y_0, DEP_Y_1)

#define NBLA_DEFINE_TRANSFORM_BINARY_INPLACE(NAME, OP, GOP0, GOP1, DEP_Y_0, DEP_Y_1)                    \
    NBLA_REGISTER_FUNCTION_HEADER(NAME, bool);                                                          \
    NBLA_DEFINE_BINARY_OP(NAME, OP, GOP0, GOP1);                                                        \
    NBLA_DEFINE_TRANSFORM_BINARY_CLASS_INPLACE(NAME, DEP_Y_0, DEP_Y_1)

// ----------------------------------------------------------------------------------------------------
// One argument
// ----------------------------------------------------------------------------------------------------

#define NBLA_DEFINE_BINARY_OP_1(NAME, OP, GOP0, GOP1, A0)                                               \
    NBLA_DEFINE_BINARY_OP_CLASS(NAME)                                                                   \
    {                                                                                                   \
    public:                                                                                             \
        A0 a0;                                                                                          \
                                                                                                        \
        inline NAME##BinaryOp(const A0& a0_) : a0(a0_) {}                                               \
        NBLA_DEFINE_BINARY_OP_EXECUTE(OP)                                                               \
        /*NBLA_DEFINE_BINARY_OP_FORWARD(OP)*/                                                           \
        /*NBLA_DEFINE_BINARY_OP_BACKWARD(0, GOP0)*/                                                     \
        /*NBLA_DEFINE_BINARY_OP_BACKWARD(1, GOP1)*/                                                     \
    }

#define NBLA_DEFINE_TRANSFORM_BINARY_CLASS_1(NAME, DEP_Y_0, DEP_Y_1, A0)                                \
    template <typename T>                                                                               \
    class NAME : public TransformBinary<T, NAME##BinaryOp, A0)                                          \
    {                                                                                                   \
        NBLA_DEFINE_TRANSFORM_BINARY_CLASS_COMMON(NAME, DEP_Y_0, DEP_Y_1)                               \
                                                                                                        \
        NAME(const Context& ctx, const A0& a0)                                                          \
            : TransformBinary<T, NAME##BinaryOp, A0>(ctx, flase, a0) {}                                 \
                                                                                                        \
        virtual shared_ptr<Function> copy() const                                                       \
        {                                                                                               \
            return create_##NAME(this->ctx_, std::get<0>(this->args_));                                 \
        }
    }

#define NBLA_DEFINE_TRANSFORM_BINARY_1(NAME, OP, GOP0, GOP1, DEP_Y_0, DEP_Y_1, A0)                      \
    NBLA_REGISTER_FUNCTION_HEADER(NAME, A0);                                                            \
    NBLA_DEFINE_BINARY_OP_1(NAME, OP, GOP0, GOP1, A0);                                                  \
    NBLA_DEFINE_TRANSFORM_BINARY_CLASS_1(NAME, DEP_Y_0, DEP_Y_1, A0)

    } // namespace nbla
}
#endif