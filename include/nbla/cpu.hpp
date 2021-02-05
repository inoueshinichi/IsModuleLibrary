#ifndef IS_NBLA_CPU_HPP
#define IS_NBLA_CPU_HPP

#include "nbla/backend_base.hpp"
#include "nbla/defs.hpp"
#include "nbla/memory/allocator.hpp"
#include "nbla/singleton_manager.hpp"

#include <memory>
#include <string>

namespace Is
{
    namespace nbla
    {
        using std::vector;
		using std::string;
		using std::unique_ptr;

        /**
         * @brief CPU計算のためのいくつかのハンドルと設定を保持するシングルトンクラス
         * 1) ~Cpu()はprivateにする
         * 2) SingletonManagerをfriendにする
         */
        class NBLA_API Cpu : public BackendBase
        {
        public:
            ~Cpu();

            /**
             * @brief CPU上での関数実装で使用できる配列クラスのリスト
             * 
             * @return vector<string> 
             */
            vector<string> array_classes() const;


            /**
             * @brief  配列クラスを設定
             * エンドユーザーは呼び出してはいけない
             * @param a 
             */
            void _set_array_classes(const vector<string>& a);


            /**
             * @brief 名前によって使用可能な配列クラスを登録
             * 
             * @param name 
             */
            void register_array_class(const string& name);


            /**
             * @brief cachingアロケータを取得
             * 
             * @return shared_ptr<Allocator> 
             */
            shared_ptr<Allocator> caching_allocator();


            /**
             * @brief nativeアロケータを取得
             * 
             * @return shared_ptr<Allocator> 
             */
            shared_ptr<Allocator> native_allocator();


            /**
             * @brief Free all unused host memory caches
             * 
             */
            void free_unused_host_caches();


            /**
             * @brief Synchronize host to device.
             * 
             * @param device 
             */
            void device_synchronize(const string &device);


            /**
             * @brief Synchronize host to default stream of device.
             * 
             * @param device 
             */
            void default_stream_synchronize(const string &device);


            /**
             * @brief Create non blockuing streams for data transfer.
             * Noting to do in CPU backend.
             * @param device 
             */
            void create_lms_streams(int device = -1) {}

        protected:
            vector<string> array_classes_; // Available array classes

            /*
				AllocatorMemoryのデストラクタが呼ばれる前にAllocatorのデストラクタが呼ばれるのを
				防ぐことを目的に、AllocatorMemoryクラスにAllocatorのポインタをメンバ変数にする.
				そのため、Allocatorはshared_ptrにする必要がある.
			*/
			shared_ptr<Allocator> native_allocator_;
			shared_ptr<Allocator> caching_allocator_;

        private:
            friend SingletonManager;

            // ユーザーによってCpuコンストラクタは呼ばれない
			Cpu();

            DISABLE_COPY_AND_ASSIGN(Cpu);
        };
    }
}
#endif