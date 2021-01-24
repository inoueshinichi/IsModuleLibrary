#ifndef NBLA_DEFS_HPP
#define NBLA_DEFS_HPP

#if defined(_MSC_VER) && !defined(__CUDACC__)
#   if defined(nnabla_EXPORTS) || defined(nnabla_dbg_EXPORTS)
#       define NBLA_API __declspec(dllexport)
#   else
#       define NBLA_API __declspec(dllimport)
#   endif
#else
#   define NBLA_API
#endif

// C++11 gives alignas as standard
// http://en.cppreference.com/w/cpp/language/alignas
#define NBLA_ALIGN(N) alignas(N)

// クラス型を取得するためのヘルパーマクロ
// これを使用するために，このマクロの前に<type_traits>をインクルードしなければならない.
#define NBLA_THIS_TYPE std::remove_pointer<decltype(this)>::type

#endif