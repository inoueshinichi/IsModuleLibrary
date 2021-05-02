#pragma once

#include <string>
#include <vector>
#include <sstream>

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
    inline std::string string_join(const std::vector<T>& vector, const std::string& delimiter)
    {
        std::stringstream ss;
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