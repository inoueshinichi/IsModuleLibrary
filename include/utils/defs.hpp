#ifndef IS_UTILS_DEFS_HPP
#define IS_UTILS_DEFS_HPP

#if defined(_MSC_VER)
// __func__の定義
#	define __func__ __FUNCTION__
#endif

namespace Is
{
    namespace utils
    {

#if defined(_MSC_VER) && !defined(__CUDACC__)
#   if defined(utils_EXPORTS) || defined(utils_dbg_EXPORTS)
#       define UTILS_API __declspec(dllexport)
#   else
#       define UTILS_API __declspec(dllimport)
#   endif
#else
#   define UTILS_API
#endif

    } // utils
}
#endif