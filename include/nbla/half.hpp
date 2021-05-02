#pragma once

#include <nbla/defs.hpp>
#include <nbla/dtypes.hpp>
#include <nbla/exception.hpp>

#include <nbla/float_bits.hpp>

#include <cmath>
#include <limits>


namespace Is
{
    namespace nbla
    {
        inline uint16_t float2halfbits(float fvalue)
        {
            // unionでビット単位で共有
            union {
                uint32_t bits;
                float value_;
            };

            value_ = fvalue;
            return float_bits<float>::downconvert_to<Half>(bits);
        }


        inline float halfbits2float(uint16_t hbits)
        {
            union {
                uint32_t fbits;
                float fvalue;
            };

            fbits = float_bits<Half>::upconvert_to<float>(hbits);
            return fvalue;
        }


        /**
         * @brief Half (half floating point)クラス
         * float 4bytes
         * half  2bytes
         * 
         */
        struct NBLA_ALIGN(2) Half
        {
            uint16_t bits;

            // Constructors
            Half();
        #define CST(TYPE) NBLA_API Half(const TYPE& rhs)
            CST(bool);
            CST(unsigned char);
            CST(char);
            CST(unsigned short);
            CST(short);
            CST(unsigned int);
            CST(int);
            CST(unsigned long);
            CST(long);
            CST(unsigned long long);
            CST(long long);
            CST(float);
            CST(double);
            CST(long double);
            CST(Half);
         #undef CST

            // Assignment Operators
            NBLA_API Half& operator+=(const Half& rhs);
            NBLA_API Half& operator-=(const Half& rhs);
            NBLA_API Half& operator*=(const Half& rhs);
            NBLA_API Half& operator/=(const Half& rhs);
            NBLA_API Half& operator=(const Half& rhs);
        #define CST(TYPE) NBLA_API operator TYPE() const
            CST(bool);
            CST(unsigned char);
            CST(char);
            CST(unsigned short);
            CST(short);
            CST(unsigned int);
            CST(int);
            CST(unsigned long);
            CST(long);
            CST(unsigned long long);
            CST(long long);
            CST(float);
            CST(double);
            CST(long double);
        #undef CTS

            // Arithmetic operators
            NBLA_API Half operator+() const;
            NBLA_API Half operator-() const;
        #define AOP(OP, TYPE) NBLA_API Half operator OP(TYPE o) const
        #define AOPR(OP, TYPE) NBLA_API TYPE operator OP(TYPE o) const
        #define AOP_TYPE(OP)                                           \
            AOP(OP, bool);                                             \
            AOP(OP, unsigned char);                                    \
            AOP(OP, char);                                             \
            AOP(OP, unsigned short);                                   \
            AOP(OP, short);                                            \
            AOP(OP, unsigned int);                                     \
            AOP(OP, int);                                              \
            AOP(OP, unsigned long);                                    \
            AOP(OP, long);                                             \
            AOP(OP, unsigned long long);                               \
            AOP(OP, long long);                                        \
            AOPR(OP, float);                                           \
            AOPR(OP, double);                                          \
            AOPR(OP, long double);                                     \
            AOP(OP, Half)

            AOP_TYPE(+);
            AOP_TYPE(-);
            AOP_TYPE(*);
            AOP_TYPE(/);

        #undef AOP_TYPE
        #undef AOP
        #undef AOPR
        };


        // Inverse arithmetic operators
        #define AOP(OP, TYPE)                                       \
            NBLA_API Half operator OP(const TYPE &lhs, const Half &rhs)
        #define AOPR(OP, TYPE)                                      \
            NBLA_API TYPE operator OP(const TYPE &lhs, const Half &rhs)
        #define AOP_TYPE(OP)                                        \
            AOP(OP, bool);                                          \
            AOP(OP, unsigned char);                                 \
            AOP(OP, char);                                          \
            AOP(OP, unsigned short);                                \
            AOP(OP, short);                                         \
            AOP(OP, unsigned int);                                  \
            AOP(OP, int);                                           \
            AOP(OP, unsigned long);                                 \
            AOP(OP, long);                                          \
            AOP(OP, unsigned long long);                            \
            AOP(OP, long long);                                     \
            AOPR(OP, float);                                        \
            AOPR(OP, double);                                       \
            AOPR(OP, long double)

            AOP_TYPE(+);
            AOP_TYPE(-);
            AOP_TYPE(*);
            AOP_TYPE(/);

        #undef AOP_TYPE
        #undef AOP
        #undef AOPR


        // Relational operators
        #define ROP_TYPE(OP, TYPE)                                 \
        NBLA_API bool operator OP(const Half &lhs, const TYPE &rhs)
        #define IROP_TYPE(OP, TYPE)                                \
        NBLA_API bool operator OP(const TYPE &lhs, const Half &rhs)
        #define ROP(TYPE)                                          \
            ROP_TYPE(<, TYPE);                                     \
            ROP_TYPE(>, TYPE);                                     \
            ROP_TYPE(<=, TYPE);                                    \
            ROP_TYPE(>=, TYPE);                                    \
            ROP_TYPE(==, TYPE);                                    \
            ROP_TYPE(!=, TYPE);                                    \
            IROP_TYPE(<, TYPE);                                    \
            IROP_TYPE(>, TYPE);                                    \
            IROP_TYPE(<=, TYPE);                                   \
            IROP_TYPE(>=, TYPE)

            ROP(bool);
            ROP(unsigned char);
            ROP(char);
            ROP(unsigned short);
            ROP(short);
            ROP(unsigned int);
            ROP(int);
            ROP(unsigned long);
            ROP(long);
            ROP(unsigned long long);
            ROP(long long);
            ROP(float);
            ROP(double);
            ROP(long double);
            ROP_TYPE(<, Half);
            ROP_TYPE(>, Half);
            ROP_TYPE(<=, Half);
            ROP_TYPE(>=, Half);
            ROP_TYPE(==, Half);
            ROP_TYPE(!=, Half);

        #undef ROP_TYPE
        #undef IROP_TYPE
        #undef ROP


        template <typename T> struct force_float { using type = T; };
        template <> struct force_float<Half> { using type = float; };
    }
}


// cmath function

namespace std
{
    using namespace Is::nbla;
#define MATHF(FUNC) NBLA_API Half FUNC(const Half& h)
    MATHF(exp);
    MATHF(log);
    MATHF(log2);
    MATHF(log1p);
    MATHF(sqrt);
    MATHF(sin);
    MATHF(cos);
    MATHF(tan);
    MATHF(sinh);
    MATHF(cosh);
    MATHF(tanh);
    MATHF(asin);
    MATHF(acos);
    MATHF(atan);
    MATHF(asinh);
    MATHF(acosh);
    MATHF(atanh);
    MATHF(fabs);
    MATHF(abs);
    MATHF(floor);
    MATHF(ceil);
    MATHF(round);
    MATHF(isnan);
    MATHF(isinf);
    NBLA_API Half pow(const Half& a, const Half& b);
    NBLA_API Half pow(const Half& a, const int& b);
    NBLA_API Half max(const Half& a, const int& b);
    NBLA_API Half atan2(const Half& a, const Half& b);
    NBLA_API Half ldexp(const Half& a, const int& b);
    #undef MATHF

    template <> class numeric_limits<Half>
    {
    public:
        inline static Half min() { return 6.10352e-5; }
        inline static Half max() { return 3.2768e+4; }
        static constexpr bool is_integer = false;
        static constexpr bool is_signed = true;
    };
}