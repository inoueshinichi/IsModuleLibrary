#ifndef IS_NBLA_ISNDARRAY_HPP
#define IS_NBLA_ISNDARRAY_HPP

/**
 * @file is_nd_array.hpp
 * @author Inoue Shinichi (inoue.shinichi.1800@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-02-21
 * 
 * @copyright Copyright (c) 2021
 * ※Sony nnablaがオープンソースであることを祈る。
 * https://github.com/sony/nnabla
 * https://github.com/sony/nnabla/blob/v1.16.0/include/nbla/variable.hpp
 * 
 * IsNdArrayは, 本家nnablaのVariableクラスの代替として使用する.
 * 画像ファイル読み込み機能などをこのクラスに実装する.
 * ----------------------------------------------------------
 * DLのbackpropagation機能は削除してある.
 * function.hppの定義もVariablesをIsNdArrayに変更し、backward()関数は無効化.
 * forward()関数はexecute()関数へ名前を変更.
 */

#include "nbla/common.hpp"
#include "nbla/nd_array.hpp"

// #include "nbla/dtypes_to_type.hpp"

#include <memory>
#include <string>
#include <sstream>


namespace Is
{
    namespace nbla
    {
        using std::shared_ptr;
        using std::make_shared;
        using std::string;
        using std::stringstream;

        class IsNdArray
        {
            Shape_t shape_;    // 多次元配列の形状
            Stride_t strides_; // C型のストライド
            Size_t size_;      // メモリサイズ
            Size_t ndim_;      // 次元数
            NdArrayPtr data_;  // 多次元配列インスタンスへのポインタ


            void update_shape_info();


        public:
            using Ptr = shared_ptr<IsNdArray>;

        private:
            // コンストラクタは公開しない
            NBLA_API IsNdArray(const Shape_t& shape = {});
            NBLA_API IsNdArray(NdArrayPtr data);


            /* IsNdArrayコンストラクタをprivateにすると使えない */
            // template <typename... Args>
            // static Ptr create(Args... args)
            // {
            //     return shared_ptr<IsNdArray>(args...);
            // }

            static Ptr create(const Shape_t& shape)
            {
                return shared_ptr<IsNdArray>(new IsNdArray(shape));
            }


            static Ptr create(NdArrayPtr data)
            {
                return shared_ptr<IsNdArray>(new IsNdArray(data));
            }

            
        public:
            /*********************************************/

            /**
             * @brief 画像ファイルからIsNdAraryインスタンスを生成.
             * 
             * @param filename 
             * @return Ptr 
             */
            static Ptr create_from(const Context& ctx, const string& filename)
            {
                // bmp, png, tiff, jpeg
            }


            /**
             * @brief ゼロで初期化してIsNdArrayインスタンスを生成.
             * 
             * @param ctx 
             * @param shape 
             * @param dtype 
             * @return Ptr 
             */
            template <typename T>
            static Ptr zeros(const Context& ctx, const Shape_t& shape)
            {
                auto ndarr = create(shape);
                ndarr->data_->zero();
                ndarr->cast_data_and_get_pointer<T>(ctx);
                return ndarr;
            }


            /**
             * @brief 1で初期化してIsNdArrayインスタンスを生成.
             * 
             * @param ctx 
             * @param shape 
             * @param dtype 
             * @return Ptr 
             */
            template <typename T>
            static Ptr ones(const Context& ctx, const Shape_t& shape)
            {
                auto ndarr = create(shape);
                ndarr->data_->fill(1.0);
                ndarr->cast_data_and_get_pointer<T>(ctx);
                return ndarr;
            }


            /**
             * @brief 指定値で埋めてIsNdArrayインスタンスを生成.
             * 
             * @tparam T 
             * @param ctx 
             * @param shape 
             * @param fill_value 
             * @return Ptr 
             */
            template <typename T>
            static Ptr fill(const Context& ctx, const Shape_t& shape, double fill_value)
            {
                auto ndarr = create(shape);
                ndarr->data_->fill(fill_value);
                ndarr->cast_data_and_get_pointer<T>(ctx);
                return ndarr;
            }


            template <typename T>
            static Ptr identity(const Context& ctx, const Shape_t& shape)
            {
                auto ndarr = create(shape);
                ndarr->data_->zero();
                T* data = ndarr->cast_data_and_get_pointer<T>(ctx);
                Stride_t strides = ndarr->strides();
                Size_t ndim = ndarr->ndim();
                Size_t size = ndarr->size();

                /* ここに処理を挿入 */
               

                return ndarr;
            }

            /*********************************************/

            NBLA_API void reshape(const vector<Size_t>& shape, bool force);


            NBLA_API Ptr view();


            NBLA_API Ptr view(const Shape_t &shape);


            inline Shape_t shape() const
            {
                return shape_;
            }


            inline Stride_t strides() const
            {
                return strides_;
            }


            NBLA_API Size_t size(Size_t axis = -1) const;


            inline Size_t ndim() const
            {
                return ndim_;
            }


            inline NdArrayPtr data()
            {
                return data_;
            }


            NBLA_API void set_data(NdArrayPtr data);


            template <typename T>
            T* cast_data_and_get_pointer(const Context&ctx, bool write_only = false)
            {
                Array* arr = data_->array()->cast(get_dtype<T>(), ctx, write_only);
                return arr->pointer<T>();
            }


            template <typename T>
            const T* get_data_pointer(const Context& ctx)
            {
                const Array* arr = data_->array()->get(get_dtype<T>(), ctx);
                return arr->const_pointer<T>();
            }


            DISABLE_COPY_AND_ASSIGN(IsNdArray);
        };


        // エイリアス
        using IsNdArrayPtr = IsNdArray::Ptr;
    }
}
#endif