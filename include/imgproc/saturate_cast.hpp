#pragma once

#include <cmath>
#include <limits.h>
// #include <algorithm>

namespace Is
{
    namespace imgproc
    {
        using uchar = unsigned char;
        using ushort = unsigned short;
        using uint = unsigned int;
        using ulong = unsigned long;
        using longlong = long long;
        using ulonglong = unsigned long long;
        using longdouble = long double;

        // プライマリーテンプレート------------------------------------

        /*1byte*/
        // uchar
        template <typename T>
        static inline T saturate_cast(uchar v) { return T(v); }
        // char
        template <typename T>
        static inline T saturate_cast(char v) { return T(v); }

        /*2byte*/
        // ushort
        template <typename T>
        static inline T saturate_cast(ushort v) { return T(v);  }
        // short
        template <typename T>
        static inline T saturate_cast(short v) { return T(v); }

        /*4byte*/
        // uint
        template <typename T>
        static inline T saturate_cast(uint v) { return T(v); }
        // int
        template <typename T>
        static inline T saturate_cast(int v) { return T(v); }
        // float
        template <typename T>
        static inline T saturate_cast(float v) { return T(v); }

        /*4byte or 8byte*/
        // ulong
        template <typename T>
        static inline T saturate_cast(ulong v) { return T(v); }
        // long
        template <typename T>
        static inline T saturate_cast(long v) { return T(v); }

        /*8byte*/
        // ulonglong
        template <typename T>
        static inline T saturate_cast(ulonglong v) { return T(v); }
        // longlong
        template <typename T>
        static inline T saturate_cast(longlong v) { return T(v); }
        // double
        template <typename T>
        static inline T saturate_cast(double v) { return T(v); }

        /*16byte*/
        // long double
        template <typename T>
        static inline T saturate_cast(longdouble v) { return T(v); }


        // 完全特殊化テンプレート------------------------------------

        // ----- uchar -----
        // uchar -> uchar
        template <>
        inline uchar saturate_cast<uchar>(uchar v) { return v; }
        // char -> uchar
        template <>
        inline uchar saturate_cast<uchar>(char v) { return (uchar)(std::max)((int)v, 0); }
        // ushort -> uchar
        template <>
        inline uchar saturate_cast<uchar>(ushort v) { return (uchar)((std::max)((uchar)v, (uchar)UCHAR_MAX)); }
        // int -> uchar
        template <>
        inline uchar saturate_cast<uchar>(int v) { return (uchar)((uint)v <= (uint)UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
        // short -> uchar
        template <>
        inline uchar saturate_cast<uchar>(short v) { return saturate_cast<uchar>((int)v); }
        // uint -> uchar
        template <>
        inline uchar saturate_cast<uchar>(uint v) { return (uchar)((std::min)(v, (uint)UCHAR_MAX)); }
        // ulong -> uchar
        template <>
        inline uchar saturate_cast<uchar>(ulong v) { return saturate_cast<uchar>((uint)v); }
        // long -> uchar
        template <>
        inline uchar saturate_cast<uchar>(long v) { return saturate_cast<uchar>((int)v); }
        // ulonglong -> uchar
        template <>
        inline uchar saturate_cast<uchar>(ulonglong v) { return (uchar)((std::min)(v, (ulonglong)UCHAR_MAX)); }
        // longlong -> uchar
        template <>
        inline uchar saturate_cast<uchar>(longlong v) { return (uchar)((ulonglong)v <= (ulonglong)UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
        // float -> uchar
        template <>
        inline uchar saturate_cast<uchar>(float v) { 
            int iv = (int)std::roundf(v);
            return saturate_cast<uchar>(iv);
        }
        // double -> uchar
        template <>
        inline uchar saturate_cast<uchar>(double v) {
            int iv = (int)std::round(v);
            return saturate_cast<uchar>(iv);
        }
        // longdouble -> uchar
        template <>
        inline uchar saturate_cast<uchar>(longdouble v) {
            return saturate_cast<uchar>((double)v);
        }


        // ----- char -----
        // char -> char
        template <>
        inline char saturate_cast<char>(char v) { return v; }
        // uchar -> char
        template <>
        inline char saturate_cast<char>(uchar v) { return (char)((std::min)((int)v, SCHAR_MAX)); }
        // ushort -> char
        template <>
        inline char saturate_cast<char>(ushort v) { return (char)((std::min)((uint)v, (uint)SCHAR_MAX)); }
        // int -> char
        template <>
        inline char saturate_cast<char>(int v) { return (char)((uint)(v - SCHAR_MIN) <= (uint)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN); }
        // short -> char
        template <>
        inline char saturate_cast<char>(short v) { return saturate_cast<char>((int)v); }
        // uint -> char
        template <>
        inline char saturate_cast<char>(uint v) { return (char)((std::min)(v, (uint)SCHAR_MAX)); }
        // long -> char
        template <>
        inline char saturate_cast<char>(long v) { return saturate_cast<char>((int)v); }
        // ulong -> char
        template <>
        inline char saturate_cast<char>(ulong v) { return saturate_cast<char>((uint)v); }
        // longlong -> char
        template <>
        inline char saturate_cast<char>(longlong v) { return (char)((ulonglong)(v - SCHAR_MIN) <= (ulonglong)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN); }
        // ulonglong -> char
        template <>
        inline char saturate_cast<char>(ulonglong v) { return (char)((std::min)(v, (ulonglong)SCHAR_MAX)); }
        // float -> char
        template <>
        inline char saturate_cast<char>(float v) {
            int iv = (int)std::roundf(v);
            return saturate_cast<char>(iv);
        }
        // double -> char
        template <>
        inline char saturate_cast<char>(double v) {
            int iv = (int)std::round(v);
            return saturate_cast<char>(iv);
        }
        // longdouble -> char
        template <>
        inline char saturate_cast<char>(longdouble v) {
            return saturate_cast<char>((double)v);
        }


        // ----- ushort -----
        // char -> ushort
        template <>
        inline ushort saturate_cast<ushort>(char v) { return (ushort)((std::max)((int)v, 0)); }
        // uchar -> ushort
        template <>
        inline ushort saturate_cast<ushort>(uchar v) { return (ushort)(v); }
        // short -> ushort
        template <>
        inline ushort saturate_cast<ushort>(short v) { return (ushort)((std::max)((int)v, 0)); }
        // ushort -> ushort
        template <>
        inline ushort saturate_cast<ushort>(ushort v) { return v; }
        // int -> ushort
        template <>
        inline ushort saturate_cast<ushort>(int v) { return (ushort)((uint)v <= (uint)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
        // uint -> ushort
        template <>
        inline ushort saturate_cast<ushort>(uint v) { return (ushort)((std::min)(v, (uint)USHRT_MAX)); }
        // long -> ushort
        template <>
        inline ushort saturate_cast<ushort>(long v) { return saturate_cast<ushort>((int)v); }
        // ulong -> ushort
        template <>
        inline ushort saturate_cast<ushort>(ulong v) { return saturate_cast<ushort>((uint)v); }
        // longlong -> ushort
        template <>
        inline ushort saturate_cast<ushort>(longlong v) { return (ushort)((ulonglong)v <= (ulonglong)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
        // ulonglong -> ushort
        template <>
        inline ushort saturate_cast<ushort>(ulonglong v) { return (ushort)((std::min)(v, (ulonglong)USHRT_MAX)); }
        // float -> ushort
        template <>
        inline ushort saturate_cast<ushort>(float v) {
            int iv = (int)std::roundf(v);
            return saturate_cast<ushort>(iv);
        }
        // double -> ushort
        template <>
        inline ushort saturate_cast<ushort>(double v) {
            int iv = (int)std::round(v);
            return saturate_cast<ushort>(iv);
        }
        // longdouble -> ushort
        template <>
        inline ushort saturate_cast<ushort>(longdouble v) {
            return saturate_cast<ushort>((double)v);
        }


        // ----- short -----
        // char -> short
        template <>
        inline short saturate_cast<short>(char v) { return (short)v; }
        // uchar -> short
        template <>
        inline short saturate_cast<short>(uchar v) { return (short)v; }
        // short -> short
        template <>
        inline short saturate_cast<short>(short v) { return v; }
        // ushort -> short
        template <>
        inline short saturate_cast<short>(ushort v) { return (short)((std::min)((int)v, SHRT_MAX)); }
        // int -> short
        template <>
        inline short saturate_cast<short>(int v) { return (short)((uint)(v - SHRT_MIN) <= (uint)SHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN); }
        // uint -> short
        template <>
        inline short saturate_cast<short>(uint v) { return (short)((std::min)(v, (uint)SHRT_MAX)); }
        // long -> short
        template <>
        inline short saturate_cast<short>(long v) { return saturate_cast<short>((int)v); }
        // ulong -> short
        template <>
        inline short saturate_cast<short>(ulong v) { return saturate_cast<short>((uint)v); }
        // longlong -> short
        template <>
        inline short saturate_cast<short>(longlong v) { return (short)((ulonglong)(v - SHRT_MIN) <= (ulonglong)SHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN); }
        // ulonglong -> short
        template <>
        inline short saturate_cast<short>(ulonglong v) { return (short)((std::min)(v, (ulonglong)SHRT_MAX)); }
        // float -> short
        template <>
        inline short saturate_cast<short>(float v) {
            int iv = (int)std::roundf(v);
            return saturate_cast<short>(iv);
        }
        // double -> short
        template <>
        inline short saturate_cast<short>(double v) {
            int iv = (int)std::round(v);
            return saturate_cast<short>(iv);
        }
        // longdouble -> short
        template <>
        inline short saturate_cast<short>(longdouble v) {
            return saturate_cast<short>((double)v);
        }


        // ----- uint -----
        // char -> uint
        template <>
        inline uint saturate_cast<uint>(char v) { return (uint)((std::max)(v, (char)0)); }
        // uchar -> uint
        template <>
        inline uint saturate_cast<uint>(uchar v) { return (uint)v; }
        // short -> uint
        template <>
        inline uint saturate_cast<uint>(short v) { return (uint)((std::max)(v, (short)0)); }
        // ushort -> uint
        template <>
        inline uint saturate_cast<uint>(ushort v) { return (uint)v; }
        // int -> uint
        template <>
        inline uint saturate_cast<uint>(int v) { return (uint)((std::max)(v, (int)0)); }
        // uint -> uint
        template <>
        inline uint saturate_cast<uint>(uint v) { return v; }
        // long -> uint
        template <>
        inline uint saturate_cast<uint>(long v) { return saturate_cast<uint>((int)v); }
        // ulong -> uint
        template <>
        inline uint saturate_cast<uint>(ulong v) { return (uint)v; }
        // longlong -> uint
        template <>
        inline uint saturate_cast<uint>(longlong v) { return (uint)((ulonglong)v <= (ulonglong)UINT_MAX ? v : v > 0 ? UINT_MAX : 0); }
        // ulonglong -> uint
        template <>
        inline uint saturate_cast<uint>(ulonglong v) { return (uint)((std::min)(v, (ulonglong)UINT_MAX)); }
        // float -> uint
        template <>
        inline uint saturate_cast<uint>(float v) {
            int iv = (int)std::roundf(v);
            return saturate_cast<uint>(iv);
        }
        // double -> uint
        template <>
        inline uint saturate_cast<uint>(double v) {
            int iv = (int)std::round(v);
            return saturate_cast<uint>(iv);
        }
        // longdouble -> uint
        template <>
        inline uint saturate_cast<uint>(longdouble v) {
            return saturate_cast<uint>((double)v);
        }


        // ----- int -----
        // char -> int
        template <>
        inline int saturate_cast<int>(char v) { return (int)v; }
        // uchar -> int
        template <>
        inline int saturate_cast<int>(uchar v) { return (int)v; }
        // short -> int
        template <>
        inline int saturate_cast<int>(short v) { return (int)v; }
        // ushort -> int
        template <>
        inline int saturate_cast<int>(ushort v) { return (int)v; }
        // int -> int
        template <>
        inline int saturate_cast<int>(int v) { return (int)v; }
        // uint -> int
        template <>
        inline int saturate_cast<int>(uint v) { return (int)((std::min)(v, (uint)INT_MAX)); }
        // long -> int
        template <>
        inline int saturate_cast<int>(long v) { return (int)v; }
        // ulong -> int
        template <>
        inline int saturate_cast<int>(ulong v) { return saturate_cast<int>((uint)v); }
        // longlong -> int
        template <>
        inline int saturate_cast<int>(longlong v) { return saturate_cast<int>((ulonglong)(v - INT_MIN) <= (ulonglong)UINT_MAX ? v : v > 0 ? INT_MAX : INT_MIN); }
        // ulonglong -> int
        template <>
        inline int saturate_cast<int>(ulonglong v) { return (int)((std::min)(v, (ulonglong)INT_MAX)); }
        // float -> int
        template <>
        inline int saturate_cast<int>(float v) {
            int iv = (int)std::roundf(v);
            return iv;
        }
        // double -> int
        template <>
        inline int saturate_cast<int>(double v) {
            int iv = (int)std::round(v);
            return iv;
        }
        // longdouble -> int
        template <>
        inline int saturate_cast<int>(longdouble v) {
            return saturate_cast<int>((double)v);
        }


        // ----- ulong -----
        // char -> ulong
        template <>
        inline ulong saturate_cast<ulong>(char v) { return (ulong)saturate_cast<uint>(v); }
        // uchar -> ulong
        template <>
        inline ulong saturate_cast<ulong>(uchar v) { return (ulong)saturate_cast<uint>(v); }
        // short -> ulong
        template <>
        inline ulong saturate_cast<ulong>(short v) { return (ulong)saturate_cast<uint>(v); }
        // ushort -> ulong
        template <>
        inline ulong saturate_cast<ulong>(ushort v) { return (ulong)saturate_cast<uint>(v); }
        // int -> ulong
        template <>
        inline ulong saturate_cast<ulong>(int v) { return (ulong)saturate_cast<uint>(v); }
        // uint -> ulong
        template <>
        inline ulong saturate_cast<ulong>(uint v) { return (ulong)v; }
        // long -> ulong
        template <>
        inline ulong saturate_cast<ulong>(long v) { return (ulong)saturate_cast<uint>(v); }
        // ulong -> ulong
        template <>
        inline ulong saturate_cast<ulong>(ulong v) { return v; }
        // longlong -> ulong
        template <>
        inline ulong saturate_cast<ulong>(longlong v) { return (ulong)saturate_cast<uint>(v); }
        // float -> ulong
        template <>
        inline ulong saturate_cast<ulong>(float v) { return (ulong)saturate_cast<uint>(v); }
        // double -> ulong
        template <>
        inline ulong saturate_cast<ulong>(double v) { return (ulong)saturate_cast<uint>(v); }
        // longdouble -> ulong
        template <>
        inline ulong saturate_cast<ulong>(longdouble v) { return saturate_cast<ulong>((double)v); }


        // ----- long -----
        // char -> long
        template <>
        inline long saturate_cast<long>(char v) { return (long)saturate_cast<int>(v); }
        // uchar -> long
        template <>
        inline long saturate_cast<long>(uchar v) { return (long)saturate_cast<int>(v); }
        // short -> long
        template <>
        inline long saturate_cast<long>(short v) { return (long)saturate_cast<int>(v); }
        // ushort -> long
        template <>
        inline long saturate_cast<long>(ushort v) { return (long)saturate_cast<int>(v); }
        // int -> long
        template <>
        inline long saturate_cast<long>(int v) { return (long)v; }
        // uint -> long
        template <>
        inline long saturate_cast<long>(uint v) { return (long)saturate_cast<int>(v); }
        // long -> long
        template <>
        inline long saturate_cast<long>(long v) { return v; }
        // ulong -> long
        template <>
        inline long saturate_cast<long>(ulong v) { return (long)saturate_cast<int>(v); }
        // longlong -> long
        template <>
        inline long saturate_cast<long>(longlong v) { return (long)saturate_cast<int>(v); }
        // ulonglong -> long
        template <>
        inline long saturate_cast<long>(ulonglong v) { return (long)saturate_cast<int>(v); }
        // float -> long
        template <>
        inline long saturate_cast<long>(float v) { return (long)saturate_cast<int>(v); }
        // double -> long
        template <>
        inline long saturate_cast<long>(double v) { return (long)saturate_cast<int>(v); }


        // ----- ulonglong -----
        // char -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(char v) { return (ulonglong)((std::max)(v, (char)v)); }
        // uchar -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(uchar v) { return (ulonglong)v; }
        // short -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(short v) { return (ulonglong)((std::max)(v, (short)0)); }
        // ushort -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(ushort v) { return (ulonglong)v; }
        // int -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(int v) { return (ulonglong)((std::max)(v, (int)0)); }
        // uint -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(uint v) { return (ulonglong)v; }
        // long -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(long v) { return saturate_cast<ulonglong>((int)v); }
        // ulong -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(ulong v) { return saturate_cast<ulonglong>((uint)v); }
        // longlong -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(longlong v) { return (ulonglong)((std::max)(v, (longlong)0)); }
        // ulonglong -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(ulonglong v) { return v; }
        // float -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(float v) {
            longlong llv = (longlong)std::roundf(v);
            return saturate_cast<ulonglong>(llv);
        }
        // double -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(double v) {
            longlong llv = (longlong)std::round(v);
            return saturate_cast<ulonglong>(llv);
        }
        // longdouble -> ulonglong
        template <>
        inline ulonglong saturate_cast<ulonglong>(longdouble v) {
            return saturate_cast<ulonglong>((double)v);
        }


        // ----- longlong -----
        // char -> longlong
        template <>
        inline longlong saturate_cast<longlong>(char v) { return (longlong)v; }
        // uchar -> longlong
        template <>
        inline longlong saturate_cast<longlong>(uchar v) { return (longlong)v; }
        // short -> longlong
        template <>
        inline longlong saturate_cast<longlong>(short v) { return (longlong)v; }
        // ushort -> longlong
        template <>
        inline longlong saturate_cast<longlong>(ushort v) { return (longlong)v; }
        // int -> longlong
        template <>
        inline longlong saturate_cast<longlong>(int v) { return (longlong)v; }
        // uint -> longlong
        template <>
        inline longlong saturate_cast<longlong>(uint v) { return (longlong)v; }
        // long -> longlong
        template <>
        inline longlong saturate_cast<longlong>(long v) { return (longlong)v; }
        // ulong -> longlong
        template <>
        inline longlong saturate_cast<longlong>(ulong v) { return (longlong)v; }
        // longlong -> longlong
        template <>
        inline longlong saturate_cast<longlong>(longlong v) { return v; }
        // ulonglong -> longlong
        template <>
        inline longlong saturate_cast<longlong>(ulonglong v) { return (longlong)((std::min)(v, (ulonglong)LONG_LONG_MAX)); }
        // float -> longlong
        template <>
        inline longlong saturate_cast<longlong>(float v) { return (longlong)(std::roundf(v)); }
        // double -> longlong
        template <>
        inline longlong saturate_cast<longlong>(double v) { return (longlong)(std::round(v)); }
        // longdouble -> longlong
        template <>
        inline longlong saturate_cast<longlong>(longdouble v) { return saturate_cast<longlong>((double)v); }


        // ----- float -----
        // char -> float
        template <>
        inline float saturate_cast<float>(char v) { return (float)v; }
        // uchar -> float
        template <>
        inline float saturate_cast<float>(uchar v) { return (float)v; }
        // short -> float
        template <>
        inline float saturate_cast<float>(short v) { return (float)v; }
        // ushort -> float
        template <>
        inline float saturate_cast<float>(ushort v) { return (float)v; }
        // int -> float
        template <>
        inline float saturate_cast<float>(int v) { return (float)v; }
        // uint -> float
        template <>
        inline float saturate_cast<float>(uint v) { return (float)v; }
        // long -> float
        template <>
        inline float saturate_cast<float>(long v) { return (float)v; }
        // ulong -> float
        template <>
        inline float saturate_cast<float>(ulong v) { return (float)v; }
        // longlong -> float
        template <>
        inline float saturate_cast<float>(longlong v) { return (float)v; }
        // ulonglong -> float
        template <>
        inline float saturate_cast<float>(ulonglong v) { return (float)v; }
        // float -> float
        template <>
        inline float saturate_cast<float>(float v) { return v; }
        // double -> float
        template <>
        inline float saturate_cast<float>(double v) { return (float)v; }


        // ----- double -----
        // char -> double
        template <>
        inline double saturate_cast<double>(char v) { return (double)v; }
        // uchar -> double
        template <>
        inline double saturate_cast<double>(uchar v) { return (double)v; }
        // short -> double
        template <>
        inline double saturate_cast<double>(short v) { return (double)v; }
        // ushort -> double
        template <>
        inline double saturate_cast<double>(ushort v) { return (double)v; }
        // int -> double
        template <>
        inline double saturate_cast<double>(int v) { return (double)v; }
        // uint -> double
        template <>
        inline double saturate_cast<double>(uint v) { return (double)v; }
        // long -> double
        template <>
        inline double saturate_cast<double>(long v) { return (double)v; }
        // ulong -> double
        template <>
        inline double saturate_cast<double>(ulong v) { return (double)v; }
        // longlong -> double
        template <>
        inline double saturate_cast<double>(longlong v) { return (double)v; }
        // ulonglong -> double
        template <>
        inline double saturate_cast<double>(ulonglong v) { return (double)v; }
        // float -> double
        template <>
        inline double saturate_cast<double>(float v) { return (double)v; }
        // double -> double
        template <>
        inline double saturate_cast<double>(double v) { return v; }
        // longdougle -> double
        template <>
        inline double saturate_cast<double>(longdouble v) { return (double)v; }


        // ----- longdouble -----
        // char -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(char v) { return (longdouble)v; }
        // uchar -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(uchar v) { return (longdouble)v; }
        // short -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(short v) { return (longdouble)v; }
        // ushort -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(ushort v) { return (longdouble)v; }
        // int -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(int v) { return (longdouble)v; }
        // uint -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(uint v) { return (longdouble)v; }
        // long -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(long v) { return (longdouble)v; }
        // ulong -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(ulong v) { return (longdouble)v; }
        // longlong -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(longlong v) { return (longdouble)v; }
        // ulonglong -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(ulonglong v) { return (longdouble)v; }
        // float -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(float v) { return (longdouble)v; }
        // double -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(double v) { return (longdouble)v; }
        // longdouble -> longdouble
        template <>
        inline longdouble saturate_cast<longdouble>(longdouble v) { return v; }
    }
}