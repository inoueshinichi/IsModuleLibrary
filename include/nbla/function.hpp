#ifndef IS_NBLA_FUNCTION_HPP
#define IS_NBLA_FUNCTION_HPP

#include "nbla/array.hpp"
#include "nbla/context.hpp"
#include "nbla/is_nd_array.hpp"

#include <memory>
#include <string>
#include <tuple>

namespace Is
{
    using std::string;
    using std::vector;
    using std::shared_ptr;
    using std::make_shared;
    using std::tuple;
    using std::get;

    namespace nbla
    {
        using IsNdArrays = vector<IsNdArray*>;

        /**
         * @brief 計算ユニットのインターフェース
         * forward()関数を実装する実装関数クラスに派生する
         * 
         * forward:
         *  y = f(x)
         * 
         * backward: バックプロパゲーション誤差
         *  dx += dy dot dx/dy 
         * https://en.wikipedia.org/wiki/Jacobian_matrix_and_determinant
         * 
         */
        // 入出力情報・Inplace/Not・演算操作のAPIを定義するクラス
        class NBLA_API Function
        {
            bool used_{false};

        public:
            using Ptr = shared_ptr<Function>;

        protected:
            Context ctx_;
            vector<shared_ptr<Shape_t>> in_shapes;
            vector<shared_ptr<Shape_t>> out_shapes;

            /* フォールバック関数 */
            // 関数のインスタンス化時にセットされると関数の振る舞いが
            // フォールバック関数に置き換わる
            Ptr fall_back_func_;

        public:
            // Inplace level used in inplace_data function.
            static const int NOT_INPLACE = 0;
            static const int INPLACE_NOT_MODIFY = 1;
            static const int INPLACE = 2;

            // Copying and storing Context.
            explicit Function(const Context& ctx);
            virtual ~Function() = 0;

            /**
             * @brief Setting up function.
             * Functionのインスタンスが使用される前に、呼び出す必要がある.
             * 
             * - Determin Array class used according to the return of
			 *   Function::allowed_array_classes and the given context.
             * 
             * - Type and shape check.
             * 
             * - Calling Function::setup_impl.
             * 
             * - Pre-allocate memory to prevent locking in asynchronous execution in CUDA etc.
             * 
             * @param inputs 
             * @param outputs 
             */
            // void setup(const Variables& inputs, const Variables& outputs);
            void setup(const IsNdArrays& inputs, const IsNdArrays& outputs);


            /**
             * @brief 順伝搬を計算し、結果をoutputのデータに格納する
             * 
             * Checking shapes before calling forward_impl() which
             * be implemented in a derived function.
             * 
             * @param inputs 
             * @param outputs 
             */
            // void forward(const Variables& inputs, const Variables& outputs);
            void execute(const IsNdArrays& inputs, const IsNdArrays& outputs);


            // /**
            //  * @brief 出力の勾配によって掛け算された逆誤差を計算する. そして、入力に格納する.
            //  * 
            //  * Checking shapes before calling backward_impl() which must be implemented in
            //  * a derived function.
            //  * 
            //  * @param inputs 
            //  * @param outputs 
            //  * @param propagate_down 
            //  * @param accum 
            //  */
            // void backward(const Variables& inputs, const Variables& outputs, 
            //               const vector<bool>& propagate_down, const vector<bool>& accum);

            
            /**
             * @brief この関数で使われているContextを取得する.
             * 
             * @return Context 
             */
            Context context() const;


            /**
             * @brief 入力のDtypesを取得
             * 
             * @return vector<dtypes> 
             */
            virtual vector<dtypes> in_types() = 0;


            /**
             * @brief 出力のDtypesを取得
             * 
             * @return vector<dtypes> 
             */
            virtual vector<dtypes> out_types() = 0;


            /**
             * @brief  Get minimum number of inputs.
             * This is meant to be used in setup fuction with in_types which is used
             * to get maximum number of inputs.
             * @return int
             */
            virtual int min_inputs() = 0;

            /**
             * @brief Get minimum number of outputs.
             * This is meant to be used in setup function with out_types which is used to get
             * max number of outputs.
             * @return int
             */
            virtual int min_outputs() = 0;


            /**
             * @brief Get function name in string
             * 
             * @return string 
             */
            virtual string name() = 0;


            /**
             * @brief Get array classes that are allowed to be specified by Context
             * 
             * @return vector<string> 
             */
            virtual vector<string> allowed_array_classes() = 0;


            // /**
            //  * @brief Dependency flag for checking if in-grad depends on out-data.
            //  * 
            //  * If i=1 and o=0, checking if i-th input' gradient
            //  * computation requires o-th output's data or not.
            //  * 
            //  * @param i Input variable index.
            //  * @param o Output variable index.
            //  * @return true 
            //  * @return false 
            //  * 
            //  * @note If any of inputs requires an output variable data when computing
            //  *       its gradient, this function must be overridden to return appropriate
            //  *       boolean value. Otherwise, backward computation will be incorrect.
            //  */
            // virtual bool grad_depends_output_data(int i, int o) const 
            // {
            //     return false;
            // }


            // /**
            //  * @brief Dependency flag for checking if in-grad depends on in-data.
            //  * If i=1 and j=0, checking checking if i-th input' gradient
            //  * computation requires j-th input's data or not.
            //  * 
            //  * By default, always returns true. If override this in a sub-class, the
            //  * computation graph engine will optimize memory usage.
            //  * 
            //  * @param i Input variable index.
            //  * @param j Input variable index.
            //  * @return true 
            //  * @return false 
            //  */
            // virtual bool grad_depends_input_data(int i, int j) const
            // {
            //     return true;
            // }


            /**
             * @brief  Get in-place-level of i-th input variable's data (see below).
             * 0 (NOT_INPLACE): Not in-placed
             * 1 (INPLACE_NOT_MODIFY): In-placed but not modified.
             * 2 (INPLACE): In-placed and modified.
             * 
             * @param i Input variable index.
             * @return int 0 by default.
             * 
             * @note If a subclass uses in-place computation, the function must override
             *       this function.
             */
            virtual int inplace_data(int i) const
            {
                return NOT_INPLACE;
            }


            /**
             * @brief Get the output variable index where i-th variables' data in-placed to.
             * 
             * @param i Input variable index.
             * @return int 
             * 
             * @note This is only valid if the i-th variable is in-placed.
             *       The maintainer of a sub-class function must override
             *       this function.
             */
            virtual int inplace_data_with(int i) const
            {
                NBLA_ERROR(error_code::not_implemented,
                           "This must be implemented for in-place support of this function.");
            }


            // /**
            //  * @brief A flag for preventing that the graph engine clears buffers of
            //  *        input variables even if clear_buffer is true and condition mets.
            //  * 
            //  * @return true 
            //  * @return false 
            //  */
            // virtual bool prohibit_clear_input_buffers() const
            // {
            //     return false;
            // }


            // /**
            //  * @brief A flag for preventing that the graph engine sets input gradient buffer as
            //  *        0 even when accum is true.
            //  * 
            //  * @return true 
            //  * @return false 
            //  */
            // virtual bool prohibit_zero_input_grad() const
            // {
            //     return false;
            // }


            /**
             * @brief Copy another instance of Function with the same context.
             * 
             * @return shared_ptr<Function> 
             */
            virtual shared_ptr<Function> copy() const = 0;


            /**
             * @brief Check whether this was already used, and turn it used.
             * 
             * @return true 
             * @return false 
             */
            inline bool ask_if_used_and_use()
            {
                bool r = used_;
                used_ = true;
                return r;
            }


        protected:
            /**
             * @brief Implementation part of setup().
             * It must do:
             * - Reshape output Variables.
             * - Allocate resources used in forward/backward computation if necessary.
             * - Checking shapes and dtypes etc.
             * 
             * @param inputs 
             * @param outputs 
             */
            // virtual void setup_impl(const Variables& inputs, const Variables& outputs) = 0;
            virtual void setup_impl(const IsNdArrays& inputs, const IsNdArrays& outputs) = 0;


            /**
             * @brief Implementation part of forward().
             * It must do:
             * - Take data in inputs and store results into data in outputs.
             * 
             * @param inputs 
             * @param outputs 
             */
            // virtual forward_impl(const Variables& inputs, const Variables& outputs) = 0;
            virtual void execute_impl(const IsNdArrays& inputs, const IsNdArrays& outputs) = 0;


            // /**
            //  * @brief Implementation part of backward().
            //  * It must do:
            //  * - Take grad in outputs (backpropagated error from children of a computational
            //  *   graph) and compute Jacobian multiplication of this function with grad.
            //  * - Store backprop error into grad in inputs.
            //  * 
            //  * @param propagate_down Boolean array that indicates whether backprop is needed
            //  *                       for an input corresponding to its index.
            //  * 
            //  * @param inputs 
            //  * @param outputs 
            //  * @param propagate_down 
            //  * @param accum 
            //  */
            // virtual void backward_impl(const Variables& inputs, const Variables& outputs,
            //                            const vector<bool>& propagate_down, 
            //                            const vector<bool>& accum) = 0;

            DISABLE_COPY_AND_ASSIGN(Function);
        };

        // エイリアス
        using FunctionPtr = Function::Ptr;


        /**
         * @brief Base function.
         * 
         * Keep arguments
         * 
         */
        // 演算操作に必要な仮引数の型情報を保持するクラス
        template <typename... Args>
        class BaseFunction : public Function
        {
        protected:
            tuple<typename std::remove_reference_t<Args>...> args_;

        public:
            using base_function_type = BaseFunction<Args...>;

            BaseFunction(const Context& ctx, Args... args)
                : Function(ctx), args_(args...) {}

            /**
             * @brief コンストラクタに与えた引数の数を取得.
             * 
             * @return int 
             */
            int num_args()
            {
                return sizeof...(Args);
            }


            /**
             * @brief タプルとして引数を取得.
             * 
             * @return * const tuple<Args...>& 
             */
            const tuple<Args...>& get_args()
            {
                return args_;
            }


            template <int Index>
            auto get_arg() -> decltype(std::get<Index>(args_))
            {
                return std::get<Index>(args_);
            }

        };
    } // namespace nbla
}
#endif