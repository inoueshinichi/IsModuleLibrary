#ifndef IS_NBLA_FUNCTION_BASE_TRANSFORM_UNARY_HPP
#define IS_NBLA_FUNCTION_BASE_TRANSFORM_UNARY_HPP

#include "nbla/cpu.hpp"
#include "nbla/function.hpp"
#include "nbla/function_registry.hpp"

namespace Is
{
    namespace nbla
    {
        /*引数の個数が1個の場合の関数Commandパターンの雛形*/
        template <typename... Args>
        class BaseTransformUnary : public BaseFunction<Args...>
        {
        protected:
            bool inplace_;

        public:
            BaseTransformUnary(const Context& ctx, bool inplace, Args... args)
                : BaseFunction<Args...>(ctx, args...)
                , inplace_(inplace) {}

            virtual ~BaseTransformUnary() {}

            virtual int min_inputs() override { return 1; }

            virtual int min_outputs() override { return 1; }

            virtual int inplace_data(int i) const override
            {
                if (!inplace_)
                {
                    return Function::NOT_INPLACE;
                }
                return Function::INPLACE;
            }

            virtual int inplace_data_with(int i) const override
            {
                return 0;
            }

        protected:
            virtual void setup_impl(const IsNdArrays& inputs, const IsNdArrays& outputs) override
            {
                outputs[0]->reshape(inputs[0]->shape(), true);
                if (inplace_)
                {
                    outputs[0]->data()->set_array(inputs[0]->data()->array());
                }
            }
        };


        /*引数が1個の場合の関数用コマンドパターンクラス*/
        // 演算操作は状態を持つポリシークラス.
        template <typename T, typename UnaryOp, typename... Args>
        class TransformUnary : public BaseTransformUnary<Args...>
        {
        protected:
            UnaryOp unary_op_;

        public:
            TransformUnary(const Context& ctx, bool inplace, Args... args)
                : BaseTransformUnary<Args...>(ctx, inplace, args...)
                , unary_op_(args...) {}

            virtual ~TransformUnary() {}

            virtual vector<dtypes> in_types() override { return vector<dtypes>{get_dtype<T>()}; }

            virtual vector<dtypes> out_types() override { return vector<dtypes>{get_dtype<T>()}; }

            virtual vector<string> allowed_array_classes()
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

        protected:
            // virtual void forward_impl(const Variables& inputs, const Variables& outputs);
            // virtual void backward_impl(const Variables& inputs, const Variables& outputs,
            //                            const vector<bool>& propagete_down,
            //                            const vector<bool>& accum);
            virtual void forward_impl(const IsNdArrays& inputs, const IsNdArrays& outputs);
            virtual void backward_impl(const IsNdArrays& inputs, const IsNdArrays& outputs,
                                       const vector<bool>& propagete_down,
                                       const vector<bool>& accum);
        };


        /* 演算操作を行うポリシークラス */
        class BaseUnaryOp
        {
        public:
            inline BaseUnaryOp() {}

            template <typename T> inline
            T operator()(const T x)
            {
                NBLA_ERROR(error_code::not_implemented,
                           "Forward operation is not implemented.");
                // NBLA_ERROR(error_code::not_implemented,
                //             "Execute operation is not implemented.");
            }

            template <typename T> inline
            T g(const T dy, const T x, const T y, const bool inplace)
            {
                NBLA_ERROR(error_code::not_implemented,
                           "Backward operation is not implemented.");
            }
        };


        /* forward(execute)演算実行テンプレート関数 */
        template <typename T, typename UnaryOp>
        void transform_unary(Size_t size, const T* x, T* y, UnaryOp op)
        {
#if _OPENMP
    #pragma omp parallel for schedule(static)
#endif
            for (Size_t idx = 0; idx < size; ++idx)
            {
                y[idx] = op(x[idx]);
            }
        }


        /* backward演算実行テンプレート関数 */
        template <typename T, typename UnaryOp, bool accum>
        void transform_unary_grad(int size, const T* dy, const T* x, const T* y, T* g,
                                  const bool inplace, UnaryOp op)
        {
            for (int idx = 0; idx < size; ++idx)
            {
                g[idx] = (accum ? g[idx] : (T)0) + op.g(dy[idx], x[idx], y[idx], inplace);
            }
        }


        // /* execute_impl for TransformUnary */
        // template <typename T, typename UnaryOp, typename... Args>
        // void TransformUnary<T, UnaryOp, Args...>::execute_impl(
        //     const IsNdArrays& inputs, const IsNdArrays& outputs)
        // {
        //     const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
        //     T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, !this->inplace_); // 常にinplace?
        //     transform_unary(inputs[0]->size(), x, y, unary_op_);
        // }

        /* forward_impl for TransformUnary */
        template <typename T, typename UnaryOp, typename... Args>
        void TransformUnary<T, UnaryOp, Args...>::forward_impl(
            const IsNdArrays& inputs, const IsNdArrays& outputs)
        {
            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, !this->inplace_); // 常にinplace?
            transform_unary(inputs[0]->size(), x, y, unary_op_);
        }

        // /* forward_impl for TransformUnary */
        // template <typename T, typename UnaryOp, typename... Args>
        // void TransformUnary<T, UnaryOp, Args...>::forward_impl(
        //     const Variables& inputs, const Variables& outputs)
        // {
        //     const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
        //     T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, !this->inplace_); // 常にinplace?
        //     transform_unary(inputs[0]->size(), x, y, unary_op_);
        // }


        // /* backward_impl for TransformUnary */
        // template <typename T, typename UnaryOp, typename... Args>
        // void TransformUnary<T, UnaryOp, Args...>::backward_impl(
        //     const Variables& inputs, const Variables& outputs,
        //     const vector<bool>& propagate_down, const vector<bool>& accum)
        // {
        //     if (!propagate_down[0])
        //     {
        //         return;
        //     }

        //     const T* dy = outputs[0]->get_grad_pointer<T>(this->ctx_);
        //     const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
        //     const T* y = outputs[0]->get_grad_pointer<T>(this->ctx_);
        //     Size_t size = inputs[0]->size();
        //     T* g = inputs[0]->cast_grad_and_get_pointer<T>(this->ctx_, !accum[0]);
        //     if (accum[0])
        //     {
        //         transform_unary_grad<T, UnaryOp, true>(size, dy, x, y, g, this->inplace_, unary_op_);
        //     } 
        //     else 
        //     {
        //         transform_unary_grad<T, UnaryOp, false>(size, dy, x, y, g, this->inplace_, unary_op_);
        //     }
        // }

         /* backward_impl for TransformUnary */
        template <typename T, typename UnaryOp, typename... Args>
        void TransformUnary<T, UnaryOp, Args...>::backward_impl(
            const IsNdArrays& inputs, const IsNdArrays& outputs,
            const vector<bool>& propagate_down, const vector<bool>& accum)
        {
            if (!propagate_down[0])
            {
                return;
            }

            const T* dy = outputs[0]->get_grad_pointer<T>(this->ctx_);
            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            const T* y = outputs[0]->get_grad_pointer<T>(this->ctx_);
            Size_t size = inputs[0]->size();
            T* g = inputs[0]->cast_grad_and_get_pointer<T>(this->ctx_, !accum[0]);
            if (accum[0])
            {
                transform_unary_grad<T, UnaryOp, true>(size, dy, x, y, g, this->inplace_, unary_op_);
            } 
            else 
            {
                transform_unary_grad<T, UnaryOp, false>(size, dy, x, y, g, this->inplace_, unary_op_);
            }
        }

/********************** Transform-Unary(Op)系クラス作成用ヘルパーマクロ **********************************/

#define NBLA_DEFINE_UNARY_OP_CLASS(NAME)                                                              \
    class NAME##UnaryOp : public BaseUnaryOp

// #define NBLA_DEFINE_UNARY_OP_EXECUTE(OP)                                                              \
//     template <typename T> inline T operator()(const T x) { return OP; }

#define NBLA_DEFINE_UNARY_OP_FORWARD(OP)                                                              \
    template <typename T> inline T operator()(const T x) { return OP; }

#define NBLA_DEFINE_UNARY_OP_BACKWARD(GOP)                                                            \
    template <typename T> inline T g(const T dy, const T x, const T y, const bool inplace)            \
    { return GOP; }

#define NBLA_DEFINE_TRANSFORM_UNARY_CLASS_COMMON(NAME, DEP_Y)                                         \
public:                                                                                               \
    virtual ~NAME() {}                                                                                \
    virtual string name() { return #NAME; }                                                           \
    virtual bool grad_depends_output_data(int i, int o) const { return DEP_Y; }                       \

// ----------------------------------------------------------------------------------------------------
// Zero argument
// ----------------------------------------------------------------------------------------------------

#define NBLA_DEFINE_UNARY_OP_NO_GRAD(NAME, OP)                                                        \
    NBLA_DEFINE_UNARY_OP_CLASS(NAME)                                                                  \
    {                                                                                                 \
    public:                                                                                           \
        /*NBLA_DEFINE_UNARY_OP_EXECUTE(OP)*/                                                          \
        NBLA_DEFINE_UNARY_OP_FORWARD(OP)                                                              \
    }

#define NBLA_DEFINE_UNARY_OP(NAME, OP, GOP)                                                           \
    NBLA_DEFINE_UNARY_OP_CLASS(NAME)                                                                  \
    {                                                                                                 \
    public:                                                                                           \
        NBLA_DEFINE_UNARY_OP_FORWARD(OP)                                                              \
        NBLA_DEFINE_UNARY_BACKWARD(GOP)                                                               \
        /*NBLA_DEFINE_UNARY_EXECUTE(OP)*/                                                             \
    }

/* Transform Unary クラス */
#define NBLA_DEFINE_TRANSFORM_UNARY_CLASS(NAME, DEP_Y)                                                \
    template <typename T>                                                                             \
    class NAME : public TransformUnary<T, NAME##UnaryOp>                                              \
    {                                                                                                 \
        NBLA_DEFINE_TRANSFORM_UNARY_CLASS_COMMON(NAME, DEP_Y)                                         \
                                                                                                      \
        NAME(const Context& ctx)                                                                      \
            : TransformUnary<T, NAME##UnaryOp>(ctx, false) {}                                         \
                                                                                                      \
        virtual shared_ptr<Function> copy() const                                                     \
        {                                                                                             \
            return create_##NAME(this->ctx_);                                                         \
        }                                                                                             \
    }

/* Commandパターンに必要な機能3点セットでマクロ定義: NO GRAD */
#define NBLA_DEFINE_TRANSFORM_UNARY_NO_GRAD(NAME, OP)                                                 \
    NBLA_REGISTER_FUNCTION_HEADER(NAME);                                                              \
    NBLA_DEFINE_UNARY_OP_NO_GRAD(NAME, OP);                                                           \
    NBLA_DEFINE_TRANSFORM_UNARY_CLASS(NAME, false)

/**
 * @brief 
 * @note If DEP_Y is true, the gradient computation depends on output data.
 * 
 */
/* Commandパターンに必要な機能3点セットでマクロ定義: WITH GRAD */
#define NBLA_DEFINE_TRANSFORM_UNARY(NAME, OP, GOP, DEP_Y)                                             \
    NBLA_REGISTER_FUNCTION_HEADER(NAME);                                                              \
    NBLA_DEFINE_UNARY_OP(NAME, OP, GOP);                                                              \
    NBLA_DEFINE_TRANSFORM_UNARY_CLASS(NAME, DEP_Y)

// ----------------------------------------------------------------------------------------------------
// One argument
// ----------------------------------------------------------------------------------------------------

#define NBLA_DEFINE_TRANSFORM_UNARY_CLASS_1(NAME, DEP_Y, A0)                                          \
    template <typename T>                                                                             \
    class NAME : public TransformUnary<T, NAME##UnaryOp, A0>                                          \
    {                                                                                                 \
        NBLA_DEFINE_TRANSFORM_UNARY_CLASS_COMMON(NAME, DEP_Y)                                         \
                                                                                                      \
        NAME(const Context& ctx, const A0& a0)                                                        \
            : TransformUnary<T, NAME##UnaryOp, A0>(ctx, false, a0) {}                                 \
                                                                                                      \
        virtual shared_ptr<Function> copy() const                                                     \
        {                                                                                             \
            return create_##NAME(this->ctx_, std::get<0>(this->args_));                               \
        }                                                                                             \
    }

#define NBLA_DEFINE_TRANSFORM_UNARY_CLASS_1_INPLACE(NAME, DEP_Y, A0)                                  \
    template <typename T>                                                                             \
    class NAME : public TransformUnary<T, NAME##UnaryOp, A0>                                          \
    {                                                                                                 \
        NAME(const Context& ctx, const A0& a0, bool inplace)                                          \
            : TransformUnary<T, NAME##UnaryOp, A0>(ctx, inplace, a0) {}                               \
                                                                                                      \
        virtual shared_ptr<Function> copy() const                                                     \
        {                                                                                             \
            return create_##NAME(this->ctx_, std::get<0>(this->args_), this->inplace_);               \
        }                                                                                             \
    }

#define NBLA_DEFINE_UNARY_OP_1(NAME, OP, GOP, A0)                                                     \
    NBLA_DEFINE_UNARY_OP_CLASS(NAME)                                                                  \
    {                                                                                                 \
    public:                                                                                           \
        A0 a0;                                                                                        \
                                                                                                      \
        inline NAME##UnaryOp(A0 a0_)                                                                  \
            : a0(a0_) {}                                                                              \
                                                                                                      \
        NBLA_DEFINE_UNARY_OP_FORWARD(OP)                                                              \
        NBLA_DEFINE_UNARY_OP_BACKWARD(GOP)                                                            \
        /*NBLA_DEFINE_UNARY_OP_EXECUTE(OP)*/                                                          \
    }

/* Commandパターンに必要な機能3点セットでマクロ定義: WITH GRAD */
#define NBLA_DEFINE_TRANSFORM_UNARY_1(NAME, OP, GOP, DEP_Y, A0)                                       \
    NBLA_REGISTER_FUNCTION_HEADER(NAME, A0);                                                          \
    NBLA_DEFINE_UNARY_OP_1(NAME, OP, GOP, A0);                                                        \
    NBLA_DEFINE_TRANSFORM_UNARY_CLASS_1(NAME, DEP_Y, A0)

/* Commandパターンに必要な機能3点セットでマクロ定義: INPLACE */
#define NBLA_DEFINE_TRANSFORM_UNARY_1_INPLACE(NAME, OP, GOP, DEP_Y, A0)                               \
    NBLA_REGISTER_FUNCTION_HEADER(NAME, A0, bool);                                                    \
    NBLA_DEFINE_UNARY_OP_1(NAME, OP, GOP, A0);                                                        \
    NBLA_DEFINE_TRANSFORM_UNARY_CLASS_1_INPLACE(NAME, DEP_Y, A0)


#define NBLA_DEFINE_UNARY_OP_1_NO_GRAD(NAME, OP, A0)                                                  \
  NBLA_DEFINE_UNARY_OP_CLASS(NAME)                                                                    \
  {                                                                                                   \
  public:                                                                                             \
    A0 a0;                                                                                            \
                                                                                                      \
    inline NAME##UnaryOp(A0 a0_)                                                                      \
        : a0(a0_) {}                                                                                  \
                                                                                                      \
    NBLA_DEFINE_UNARY_OP_FORWARD(OP)                                                                  \
    /*NBLA_DEFINE_UNARY_OP_EXECUTE(OP)*/                                                              \
  }

/* Commandパターンに必要な機能3点セットでマクロ定義: NO GRAD */ 
#define NBLA_DEFINE_TRANSFORM_UNARY_1_NO_GRAD(NAME, OP, A0)                                           \
  NBLA_REGISTER_FUNCTION_HEADER(NAME, A0);                                                            \
  NBLA_DEFINE_UNARY_OP_1_NO_GRAD(NAME, OP, A0);                                                       \
  NBLA_DEFINE_TRANSFORM_UNARY_CLASS_1(NAME, false, A0)

    } // namespace nbla
}
#endif