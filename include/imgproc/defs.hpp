#pragma once

#ifdef _MSC_VER
    // https://qiita.com/Chironian/items/462a3bdf271d5f0b00b6#%EF%BC%92%EF%BC%93c4251%E8%AD%A6%E5%91%8A%E3%81%8C%E5%87%BA%E3%82%8B
    #pragma warning(disable:4251)
#endif

#if defined(_MSC_VER) && !defined(__CUDACC__)
    #if defined(imgproc_EXPORTS) || defined(imgproc_dbg_EXPORTS)
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

#include <imgproc/saturate_cast.hpp>

#include <cmath>

namespace Is
{
    namespace imgproc
    {
        // // 前方宣言
        // template <typename T>
        // class Rect<T>;

        // /*Point2*/
        // template <typename T>
        // class Point2
        // {
        // public:
        //     T x_;
        //     T y_;

        // public:
        //     Point2() = default;

        //     Point2(T x, T y)
        //         : x_(x), y_(y) {}
            
        //     virtual ~Point2() {}

        //     Point2(const Point2<T> &other) = default;
        //     Point2<T> &operator=(const Point2<T> &other) = default;
        //     Point2(Point2<T> &&other) = default;
        //     Point2<T> &operator=(Point2<T> &&other) = default;

        //     template <typename U>
        //     operator Point2<U>() const
        //     {
        //         return Point2<U>(saturate_cast<U>(x_), saturate_cast<U>(y_));
        //     }

        //     T x() const { return x_; }
        //     T y() const { return y_; }
        //     double norm() const { return std::sqrt(x_ * x_ + y_ * y_); }
        //     bool inside(const Rect<T> &rect) const
        //     {
        //         return rect.contain(*this);
        //     }

        //     /*static operator*/
        //     // 1) Point2<T> &operator+=(Point2<T> &a, const Point2<T> &b);
        //     // 2) Point2<T> &operator-=(Point2<T> &a, const Point2<T> &b);
        //     // 3) Point2<T> &operator*=(Point2<T> &a, U b);
        //     // 4) Point2<T> &operatpr/=(Point2<T> &a, U b);
        //     // 5) bool operator==(const Point2<T> &a, const Point2<T> &b);
        //     // 6) bool operator!=(const Point2<T> &a, const Point2<T> &b);
        //     // 7) Point2<T> operator+(const Point2<T> &a, const Point2<T> &b);
        //     // 8) Point2<T> operator-(const Point2<T> &a, const Point2<T> &b);
        //     // 9) Point2<T> operator*(const Point2<T> &a, const Point2<T> &b);
        //     // 10) Point2<T> operator*(const Point2<T> &a, U b);
        //     // 11) POint2<T> operator*(U a, const Point2<T> &b);
        //     // 12) Point2<T> operator/(const Point2<T> &a, const Point2<T> &b);
        //     // 13) Point2<T> operator/(const Point2<T> &a, U b);
        //     // 14) Point2<T> operator/(U a, const Point2<T> &b);
        // };

        // // 1) Point2<T> &operator+=(Point2<T> &a, const Point2<T> &b);
        // template <typename T>
        // static inline Point2<T> &operator+=(Point2<T> &a, const Point2<T> &b)
        // {
        //     a.x_ = saturate_cast<T>(a.x_ + b.x_);
        //     a.y_ = saturate_cast<T>(a.y_ + b.y_);
        //     return a;
        // }

        // // 2) Point2<T> &operator-=(Point2<T> &a, const Point2<T> &b);
        // template <typename T>
        // static inline Point2<T> &operator-=(Point2<T> &a, const Point2<T> &b)
        // {
        //     a.x_ = saturate_cast<T>(a.x_ - b.x_);
        //     a.y_ = saturate_cast<T>(a.y_ - b.y_);
        //     return a;
        // }

        // // 3) Point2<T> &operator*=(Point2<T> &a, U b);
        // template <typename T, typename U>
        // static inline Point2<T> &operator*=(Point2<T> &a, U b)
        // {
        //     a.x_ = saturate_cast<T>(a.x_ * b);
        //     a.y_ = saturate_cast<T>(a.y_ * b);
        //     return a;
        // }

        // // 4) Point2<T> &operatpr/=(Point2<T> &a, U b);
        // template <typename T, typename U>
        // static inline Point2<T> &operator/=(Point2<T> &a, U b)
        // {
        //     a.x_ = saturate_cast<T>(a.x_ / b);
        //     a.y_ = saturate_cast<T>(a.y_ / b);
        //     return a;
        // }

        // // 5) bool operator==(const Point2<T> &a, const Point2<T> &b);
        // template <typename T>
        // bool operator==(const Point2<T> &a, const Point2<T> &b)
        // {
        //     return a.x_ == b.x_ && a.y_ == b.y_;
        // }

        // // 6) bool operator!=(const Point2<T> &a, const Point2<T> &b);
        // template <typename T>
        // bool operator!=(const Point2<T> &a, const Point2<T> &b)
        // {
        //     return !operator==(a, b);
        // }






        // // ----------------------------------------------------------------

        // /*Rect*/
        // template <typename T>
        // class Rect
        // {
        // public:
        //     T top_;
        //     T left_;
        //     T bottom_;
        //     T right_;

        // public:
        //     Rect() = default;
        //     Rect(T top, T left, T bottom, T right)
        //         : top_(top), left_(left), bottom_(bottom), right_(right) {}

        //     Rect(const Rect<T> &other) = default;
        //     Rect<T> &operator=(const Rect<T> &other) = default;
        //     Rect(Rect<T> &&other) = default;
        //     Rect<T> &operator=(Rect<T> &&other) = default;

        //     template <typename U>
        //     operator Rect<U>() const {

        //     }


        // }
    }
}
