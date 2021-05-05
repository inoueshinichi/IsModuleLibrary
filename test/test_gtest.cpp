// #include <gtest/gtest.h>


// int test_target_sum(int input)
// {
//     if (input == 0)
//         return 0;

//     return input + test_target_sum(input - 1);
// }

// namespace 
// {
//     TEST(TutorialTest, SumTest)
//     {
//         EXPECT_EQ(6, test_target_sum(3));
//     }
// }

#include <iostream>

int main(int, char**)
{
    std::cout << "Hello test_gtest" << std::endl;
    return 0;
    // return RUN_ALL_TESTS();
}