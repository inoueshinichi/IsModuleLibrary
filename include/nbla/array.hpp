#ifndef IS_NBLA_ARRAY_HPP
#define IS_NBLA_ARRAY_HPP

#include "nbla/common.hpp"
#include "nbla/dtypes.hpp"
#include "nbla/event.hpp"
#include "nbla/exception.hpp"
// #include "nbla/half.hpp"
#include "nbla/memory/allocator.hpp"

#include <memory>
#include <type_traits>

namespace Is
{
    namespace nbla
    {
        /**
		* CpuArrayやCudaArrayのような実装クラスのための配列インターフェイスクラス.
		* Arrayクラス達はユーザーに直接触らせない
		*/

        // 配列インターフェースの基底クラス
		// このクラスはCpuArrayやCudaArrayなどの実装クラスによって拡張される.
        class Array : public std::enable_shared_from_this<Array>
        {
        protected:
            Size_t size_;         // 配列のサイズ
            dtypes dtype_;        // float32, int32, uint8のような型
            Context ctx_;         // 何のデバイス配列が使用されているかを識別するための情報を保持する
            AllocatorMemory mem_; // Memoryインスタンスを保持する
            EventPtr event_;      // 非同期でのメモリコピーを待つためのEventインスタンスを保持する


            /**
             * @brief Construct a new Array object
             * コンストラクタは派生クラスのみから呼ばれなければならない.
             * AllocatorMemoryはnbla::Allocatorの実現インスタンスによって作成され，右辺値参照として渡されるはず.
             * 
             * @param size 
             * @param dtype 
             * @param ctx 
             * @param mem 
             */
            NBLA_API Array(const Size_t size, dtypes dtype, const Context& ctx, AllocatorMemory&& mem);


            /**
             * @brief 多次元配列のサイズから使用している総バイト数を計算する.
             * 
             * @param size 
             * @param dtype 
             * @return std::size_t 
             */
            static NBLA_API std::size_t size_as_bytes(Size_t size, dtypes dtype);


            /**
             * @brief Memoryインスタンスが保持する先頭の生ポインタを取得する.
             * このメソッドはArray::pointer()をオーバーライドするために使われる.
             * 
             * @return void* 
             */
            virtual NBLA_API void* mem_pointer() { return mem_.pointer(); }


            /**
             * @brief Memoryインスタンスが保持するconstな先頭の生ポインタを取得する.
             * このメソッドはArray::conset_pointer()をオーバーライドするために使われる.
             * 
             * @return const void* 
             */
            virtual NBLA_API const void* mem_const_pointer() const { return mem_.const_pointer(); }


        public:
            using Ptr = shared_ptr<Array>;


            virtual NBLA_API ~Array() = 0;


            /**
             * @brief Array系インスタンスが持つ先頭の生ポインタを取得する.
             * 
             * @tparam T 
             * @return T* 
             */
            template <typename T = void>
            T* pointer()
            {
                return reinterpret_cast<T*>(mem_pointer());
            }

            
            /**
             * @brief Array系インスタンスが持つconstな先頭の生ポインタを取得する.
             * 
             * @tparam T 
             * @return const T* 
             */
            template <typename T = void>
            const T* const_pointer() const
            {
                return reinterpret_cast<const T*>(mem_const_pointer());
            }

            
            /**
             * @brief dtypesを取得する.
             * 
             * @return dtypes 
             */
            inline dtypes dtype() const { return dtype_; }


            /**
             * @brief 多次元配列のサイズを取得する.
             * 
             * 例えば，
			 * [[[1,2,3]
			 *   [4,5,6]
			 *   [7,8,9]]
			 *  [[9,8,7]
			 *   [6,5,4]
			 *   [3,2,1]]]
			 * Size_t = (2,3,3)の多次元配列(tensor)
             * 
             * @return Size_t 
             */
            inline Size_t size() const { return size_; }


            /**
             * @brief Contextを取得する.
             * 
             * @return Context 
             */
            inline Context context() const { return ctx_; }


            /**
             * @brief 他のArray系インスタンスから中身をコピーする.
             * 
             * @param src_array 
             */
            virtual void copy_from(const Array* src_array) = 0;


            /**
             * @brief zero(0)ですべての配列要素を埋める.
             * 
             * @return void 
             */
            virtual NBLA_API void zero() = 0;


            /**
             * @brief 与えた数値ですべての配列要素を埋める.
             * 
             * @param value 
             */
            virtual NBLA_API void fill(float value) = 0;


            /**
             * @brief Array系インスタンスを記述する最小情報でフィルタしたContextを取得する.
             * 
             * @param ctx 
             * @return Context 
             */
            static Context filter_context(const Context& ctx);


            /**
             * @brief Set the event object
             * 
             * @param eptr 
             */
            virtual NBLA_API void set_event(EventPtr eptr);


            /**
             * @brief イベントの終了を待つ.
             * 
             * 
             * @param ctx イベントの終了を待っているContext.
             * @param async_flags 
             * @return void 
             */
            virtual NBLA_API void wait_event(const Context ctx, const int async_flags = AysincFlag::NONE);


            /**
             * @brief イベントを持っているかチェックする.
             * 
             * @return bool 
             */
            virtual NBLA_API bool have_event();


            /**
             * @brief Array系インスタンスのshared_ptrを取得する.
             * 
             * @return Ptr 
             */
            virtual NBLA_API Ptr getptr();

        protected:
            DISABLE_COPY_AND_ASSIGN(Array);
        };

        // エイリアス
        using ArrayPtr = Array::Ptr;
        using ConstArray = const Array;
        using ConstArrayPtr = shared_ptr<ConstArray>;

        /**
		* 以下，マクロによるヘルパー関数群.
		*/
    
    }
}

#endif