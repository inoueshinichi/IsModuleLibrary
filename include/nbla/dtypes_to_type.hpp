#ifndef IS_NBLA_DTYPES_TO_TYPE_HPP
#define IS_NBLA_DTYPES_TO_TYPE_HPP

#include "nbla/dtypes.hpp"
// #include "nbla/half.hpp"

#include <type_traits>

namespace Is
{
    namespace nbla
    {
        /* bool */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::BOOL>* = nullptr>
        struct type_from
        {
            using type = bool;
        };

        /* char */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::BYTE>* = nullptr>
        struct type_from
        {
            using type = char;
        };

        /* unsigned char */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::UBYTE>* = nullptr>
        struct type_from
        {
            using type = unsigned char;
        };

        /* short */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::SHORT>* = nullptr>
        struct type_from
        {
            using type = short;
        };

        /* unsigned short */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::USHORT>* = nullptr>
        struct type_from
        {
            using type = unsigned short;
        };

        /* int */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::INT>* = nullptr>
        struct type_from
        {
            using type = int;
        };
        
        /* unsigned int */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::UINT>* = nullptr>
        struct type_from
        {
            using type = unsigned int;
        };

        /* long */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::LONG>* = nullptr>
        struct type_from
        {
            using type = long;
        };

        /* unsigned long */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::ULONG>* = nullptr>
        struct type_from
        {
            using type = unsigned long;
        };

        /* long long */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::LONGLONG>* = nullptr>
        struct type_from
        {
            using type = long long;
        };

        /* unsigned long long */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::ULONGLONG>* = nullptr>
        struct type_from
        {
            using type = unsigned long long;
        };

        /* float */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::FLOAT>* = nullptr>
        struct type_from
        {
            using type = float;
        };

        /* double */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::DOUBLE>* = nullptr>
        struct type_from
        {
            using type = double;
        };

        /* long double */
        template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::LONGDOUBLE>* = nullptr>
        struct type_from
        {
            using type = long dobule;
        };

        // /* half */
        // template <size_t DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == dtypes::HALF>* = nullptr>
        // struct type_from
        // {
        //     using type = Half;
        // };

        // エイリアス
        template <size_t DTYPES_ENUM>
        using type_from_t = type_from<DTYPES_ENUM>::type;        
    } // namespace nbla
}
#endif