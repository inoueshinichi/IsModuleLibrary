#include "utils/win32/win32_char_traits.hpp"

namespace Is
{
    namespace utils
    {
        namespace win32
        {
            //////////////////////////////////
            /* Convert from UTF-8 to UTF-16 */
            //////////////////////////////////
            /**
             * @brief UTF-8 -> UTF-16
             * 
             * @param utf8_char 
             * @return std::wstring 
             */
            std::wstring CvtUtf8ToUtf16(char const* utf8_char)
            {
                if (!utf8_char)
                    return std::wstring();

                /* C++17よりstd::codecvtヘッダーが非推奨になったので、下記2行は使えない */
                // std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
                // return converter.from_bytes(utf8_char);

                /* 代替案(プラットフォーム毎の変換APIを使うこと. 主にWindows OS) */
                // https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte/3999597#3999597
                size_t necessary_size = ::MultiByteToWideChar(CP_UTF8, 0, utf8_char, -1, NULL, 0);
                std::wstring utf16_wstr(necessary_size, 0);
                ::MultiByteToWideChar(CP_UTF8, 0, utf8_char, -1, &utf16_wstr[0], necessary_size);
                utf16_wstr.resize(necessary_size - 1); // '\0'を消す
                return utf16_wstr;
            }


            /**
             * @brief UTF-8 -> UTF-16
             * 
             * @param utf8_string 
             * @return std::wstring 
             */
            std::wstring CvtUtf8ToUtf16(const std::string& utf8_string)
            {
                return CvtUtf8ToUtf16(utf8_string.c_str());
            }


            //////////////////////////////////
            /* Convert from UTF-16 to UTF-8 */
            //////////////////////////////////
            /**
             * @brief UTF-16 -> UTF-8
             * 
             * @param utf16_wchar 
             * @return std::string 
             */
            std::string CvtUtf16ToUtf8(const wchar_t* utf16_wchar)
            {
                if (!utf16_wchar)
                    return std::string();

                /* C++17よりstd::codecvtヘッダーが非推奨になったので、下記2行は使えない */
                // std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
                // return converter.to_bytes(utf16_wchar);

                /* 代替案(プラットフォーム毎の変換APIを使うこと. 主にWindows OS) */
                // https://stackoverflow.com/questions/215963/how-do-you-properly-use-widechartomultibyte/3999597#3999597
                size_t necessary_size = ::WideCharToMultiByte(CP_UTF8, 0, utf16_wchar, -1, NULL, 0, NULL, NULL);
                std::string utf8_str(necessary_size, 0);
                ::WideCharToMultiByte(CP_UTF8, 0, utf16_wchar, -1, &utf8_str[0], necessary_size, NULL, NULL);
                utf8_str.resize(necessary_size - 1); // '\0'を消す
                return utf8_str;
            }


            /**
             * @brief UTF-16 -> UTF-8
             * 
             * @param utf16_wstring 
             * @return std::string 
             */
            std::string CvtUtf16ToUtf8(const std::wstring& utf16_wstring)
            {
                return CvtUtf16ToUtf8(utf16_wstring.c_str());
            }


            /**
             * @brief UTF-16 -> UTF-8
             * 
             * @param utf16_char16 
             * @return std::string 
             */
            std::string CvtUtf16ToUtf8(const char16_t* utf16_char16)
            {
                return CvtUtf16ToUtf8(reinterpret_cast<const wchar_t*>(utf16_char16));
            }


            /**
             * @brief UTF-16 -> UTF-8
             * 
             * @param utf16_string 
             * @return std::string 
             */
            std::string CvtUtf16ToUtf8(const std::u16string& utf16_string)
            {
                return CvtUtf16ToUtf8(utf16_string.c_str());
            }


            //////////////////////////////////////
            /* Convert from Shift-JIS to UTF-16 */
            //////////////////////////////////////
            /**
             * @brief Shift-JIS -> UTF-16
             * 
             * @param shift_jis_char 
             * @return std::wstring 
             */
            std::wstring CvtShiftJISToUtf16(const char* shift_jis_char)
            {
                if (!shift_jis_char)
                    return std::wstring();

                size_t necessary_size = ::MultiByteToWideChar(CP_ACP, 0, shift_jis_char, -1, NULL, 0);
                std::wstring utf16_wstr(necessary_size, 0);
                ::MultiByteToWideChar(CP_ACP, 0, shift_jis_char, -1, &utf16_wstr[0], necessary_size);
                utf16_wstr.resize(necessary_size - 1); // '\0'を消す
                return utf16_wstr;
            }


            /**
             * @brief Shift-JIS -> UTF-16
             * 
             * @param shift_jis_string 
             * @return std::wstring 
             */
            std::wstring CvtShiftJISToUtf16(const std::string& shift_jis_string)
            {
                return CvtShiftJISToUtf16(shift_jis_string.c_str());
            }


            //////////////////////////////////////
            /* Convert from UTF-16 to Shift-JIS */
            //////////////////////////////////////
            /**
             * @brief UTF-16 -> Shift-JIS
             * 
             * @param utf16_wchar 
             * @return std::string 
             */
            std::string CvtUtf16ToShiftJIS(const wchar_t* utf16_wchar)
            {
                if (!utf16_wchar)
                    return std::string();

                size_t necessary_size = ::WideCharToMultiByte(CP_ACP, 0, utf16_wchar, -1, NULL, 0, NULL, NULL);
                std::string shift_jis_str(necessary_size, 0);
                ::WideCharToMultiByte(CP_ACP, 0, utf16_wchar, -1, &shift_jis_str[0], necessary_size, NULL, NULL);
                shift_jis_str.resize(necessary_size - 1); // '\0'を消す
                return shift_jis_str;
            }


            /**
             * @brief UTF-16 -> Shift-JIS
             * 
             * @param utf16_wstring 
             * @return std::string 
             */
            std::string CvtUtf16ToShiftJIS(const std::wstring& utf16_wstring)
            {
                return CvtUtf16ToShiftJIS(utf16_wstring.c_str());
            }


            /**
             * @brief UTF-16 -> Shift-JIS
             * 
             * @param utf16_char16 
             * @return std::string 
             */
            std::string CvtUtf16ToShiftJIS(const char16_t* utf16_char16)
            {
                return CvtUtf16ToShiftJIS(reinterpret_cast<const wchar_t*>(utf16_char16));
            }


            /**
             * @brief UTF-16 -> Shift-JIS
             * 
             * @param utf16_string 
             * @return std::string 
             */
            std::string CvtUtf16ToShiftJIS(const std::u16string& utf16_string)
            {
                return CvtUtf16ToShiftJIS(utf16_string.c_str());
            }


            //////////////////////////////////////
            /* Convert from UTT-8 to Shift-JIS  */
            //////////////////////////////////////
            /**
             * @brief UTF-8 -> Shift-JIS
             * 
             * @param utf8_char 
             * @return std::string 
             */
            std::string CvtUtf8ToShiftJIS(const char* utf8_char)
            {
                /* UTF-8 -> UTF-16 -> Shift-JIS */
                std::wstring utf16_wstr = CvtUtf8ToUtf16(utf8_char);
                return CvtUtf16ToShiftJIS(utf16_wstr);
            }


            /**
             * @brief UTF-8 -> Shift-JIS
             * 
             * @param utf8_string 
             * @return std::string 
             */
            std::string CvtUtf8ToShiftJIS(const std::string& utf8_string)
            {
                /* UTF-8 -> UTF-16 -> Shift-JIS */
                std::wstring utf16_wstr = CvtUtf8ToUtf16(utf8_string);
                return CvtUtf16ToShiftJIS(utf16_wstr);
            }


            //////////////////////////////////////
            /* Convert from Shift-JIS to UTF-8  */
            //////////////////////////////////////
            /**
             * @brief Shift-JIS -> UTF-8
             * 
             * @param shift_jis_char 
             * @return std::string 
             */
            std::string CvtShiftJISToUtf8(const char* shift_jis_char)
            {
                /* Shift-JIS -> UTF-16 -> UTF-8 */
                std::wstring utf16_wstr = CvtShiftJISToUtf16(shift_jis_char);
                return CvtUtf16ToShiftJIS(utf16_wstr);
            }


            /**
             * @brief Shift-JIS -> UTF-8
             * 
             * @param shift_jis_string 
             * @return std::string 
             */
            std::string CvtShiftJISToUtf8(const std::string& shift_jis_string)
            {
                /* Shift-JIS -> UTF-16 -> UTF-8 */
                std::wstring utf16_wstr = CvtShiftJISToUtf16(shift_jis_string);
                return CvtUtf16ToShiftJIS(utf16_wstr);
            }
        }
    }
}