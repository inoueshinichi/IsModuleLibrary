#pragma once

#include <ctime> // clock関数
#include <chrono>
#if defined(_MSC_VER)
    #include <Windows.h> // win32 : GetTickCount関数, timeGetTime関数, QueryPerformanceCounter関数
#endif

namespace Is
{
    namespace utils
    {
        /**
         * @brief C言語のclock関数 精度： 10ms程度
         * 単位: [ms]
         */
        auto invoke_tm_ms_c = [](auto&& func, auto&&... args) -> double {
            std::cout << "C clock(): ";
            std::clock_t start = std::clock();
            std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送
            std::clock_t end = std::clock();
            std::size_t duration = (double)(end - start);
            std::cout << duration << "[ms]" << std::endl;
            return duration;
        };

        auto invoke_tm_ms_c_ret = [](auto &&func, auto &&...args) {
            std::cout << "C clock(): ";
            std::clock_t start = std::clock();
            auto result = std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送
            std::clock_t end = std::clock();
            std::size_t duration = (double)(end - start);
            std::cout << duration << "[ms]" << std::endl;
            return result;
        };

        /**
         * @brief C++ std::chorono 精度： 1ms程度
         * 
         */
        auto invoke_tm_ms_chrono = [](auto&& func, auto&&... args) -> double {
            std::cout << "std::chrono(): ";
            auto start = std::chrono::high_resolution_clock::now(); // system_clock::now()
            std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = end - start;
            double duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            std::cout << duration_ms << "[ms]" << std::endl;
            return duration_ms;
        };

        auto invoke_tm_ms_chrono_ret = [](auto &&func, auto &&...args) {
            std::cout << "std::chrono(): ";
            auto start = std::chrono::system_clock::now();
            auto result = std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送
            auto end = std::chrono::system_clock::now();
            auto duration = end - start;
            double duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            std::cout << duration_ms << "[ms]" << std::endl;
            return result;
        };


        /**
         * @brief C++ std::chrono 精度: 1ns
         * 
         */
        auto invoke_tm_ns_chrono_ret = [](auto &&func, auto &&... args) {
            std::cout << "std::chrono(): ";
            auto start = std::chrono::high_resolution_clock::now();
            auto result = std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = end - start;
            double duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
            std::cout << duration_ns << "[ns]" << std::endl;
            return result;
        };

#if defined(_MSC_VER)
        /**
         * @brief  Win32 API GetTickCount 精度： 1ms程度
         * 単位: [ms]
         */
        auto invoke_tm_win32_1 = [](auto&& func, auto&&... args) -> double {
            std::cout << "Win32 GetTickCount64(): ";
            ULONGLONG start = ::GetTickCount64();
            std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送
            ULONGLONG end = ::GetTickCount64();
            double duration = (double(end - start);
            std::cout << duration << "[ms]" << std::endl;
            return duration;
        };


        /**
         * @brief  Win32 API timeGetTime 精度： 1ms程度
         * 
         */
        auto invoke_tm_win32_2 = [](auto&& func, auto&&... args) -> double {
            std::cout << "Win32 timeGetTime(): ";
            DWORD start = ::timeGetTime();
            std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送
            DWORD end = ::timeGetTime();
            double duration = (double)(end - start);
            std::cout << duration << "[ms]" << std::endl;
            return duration;
        };


        /**
         * @brief Win32 API QueryPerformanceCounter 精度： 1ns程度
         * 
         */
        auto invoke_tm_win32_3 = [](auto&& func, auto&&... args) -> double {
            std::cout << "Win32 QueryPerformanceCounter(): ";
            LARGE_INTEGER freq;
            if (!(::QueryPerformanceFrequency(&freq)))
            {
                std::cout << "fail to get time unit." << std::endl;
                return 0;
            }

            LARGE_INTEGER start, end;
            if (!::QueryPerformanceFrequency(&start))
            {
                std::cout << "fail to start." << std::endl;
                return 0;
            }

            std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送

            if (!::QueryPerformanceFrequency(&end))
            {
                std::cout << "fail to end." << std::endl;
                return 0;
            }

            double duration = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
            std::cout << duration << "[ms]" << std::endl;
            return duration;
        };
#endif
    }
}