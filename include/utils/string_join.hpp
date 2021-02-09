#ifndef IS_UTILS_STRING_JOIN_HPP
#define IS_UTILS_STRING_JOIN_HPP

namespace Is
{
    /**
     * @brief Pythonの" ".join(['A', 'B', 'C'])と同じ文字連結処理
     * 
     * @tparam T 
     * @param vector 
     * @param delimiter 
     * @return string 
     */
    template <typename T>
    inline string string_join(const vector<T>& vector, const string& delimiter)
    {
        stringstream ss;
        if (vector.empty())
        {
            return "";
        }

        for (int i = 0; i < vector.size() - 1; ++i)
        {
            ss << vector[i] << delimiter;
        }
        ss << vector[vector.size() - 1];
        return ss.str();
    }
}

#endif