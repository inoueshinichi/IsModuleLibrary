#pragma once

#ifdef _MSC_VER
    // https://qiita.com/Chironian/items/462a3bdf271d5f0b00b6#%EF%BC%92%EF%BC%93c4251%E8%AD%A6%E5%91%8A%E3%81%8C%E5%87%BA%E3%82%8B
    #pragma warning(disable:4251)
#endif

#if defined(_MSC_VER) && !defined(__CUDACC__)
    #if defined(is_imgproc_EXPORTS) || defined(is_imgproc_dbg_EXPORTS)
        #define IMGPROC_API __declspec(dllexport)
    #else
        #define IMGPROC_API __declspec(dllimport)
    #endif
#else
    #define IMGPROC_API
#endif


// C++11 gives alignas as standard
// http://en.cppreference.com/w/cpp/language/alignas
#define IMGPROC_ALIGN(N) alignas(N)
