#pragma once

#include <imgproc/common.hpp>
#include <imgproc/saturate_cast.hpp>

namespace Is
{
    namespace imgproc
    {
        using namespace nbla;

        /**
         * @brief `reflect`でダンプ中....
         * 
         * @tparam T 
         * @param src 
         * @param ex 
         * @param ey 
         * @param style 
         * @return NdArrayPtr 
         */
        template <typename T>
        NdArrayPtr padding(NdArrayPtr src, int ex, int ey, const std::string& style = "zero")
        {
            IMGPROC_CHECK_IMAGE_SHAPE(src);

            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto ndim = src->ndim();
            auto shape = src->shape();
            auto strides = src->strides();
            int channels = shape[0];
            int height = shape[1];
            int width = shape[2];
            T *data = src->template cast_data_and_get_pointer<T>(ctx);
            // -----------------------------

            int index_ys = ey;
            int index_xs = ex;
            int index_ye = index_ys + height;
            int index_xe = index_xs + width;

            auto ex_src = zeros<T>(Shape_t{channels, height + 2 * ey, width + 2 * ex});
            auto ex_strides = ex_src->strides();
            auto ex_channels = ex_strides[0];
            auto ex_height = ex_strides[1];
            auto ex_width = ex_strides[2];
            T *data_ex_src = ex_src->template cast_data_and_get_pointer<T>(ctx);

            for (int c = 0; c < channels; ++c)
            {
                for (int y = 0; y < height; ++y) 
                {
                    for (int x = 0; x < width; ++x) 
                    {
                        data_ex_src[c * ex_channels + (y + ey) * ex_height + (x + ex) * ex_width] =
                            data[c * strides[0] + y * strides[1] + x * strides[2]];
                    }
                }
            }

            if (style == "zero")
            {
                // 000|abcdefg|000
                return ex_src;
            }
            else if (style == "mean")
            {

                for (int c = 0; c < channels; ++c)
                {
                    // 左端, 右端
                    double mean_left, mean_right;
                    for (int y = 0; y < height; ++y)
                    {
                        mean_left = mean_right = 0.0f;

                        for (int x = 0; x < ex; ++x)
                        {
                            mean_left = data_ex_src[c * ex_channels + (y + index_ys) * ex_height + (x + index_xs) * ex_width];
                            mean_right = data_ex_src[c * ex_channels + (y + index_ys) * ex_height + (x + index_xe - ex) * ex_width];
                        }
                        mean_left /= ex;
                        mean_right /= ex;

                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex_src[c * ex_channels + (y + index_ys) * ex_height + x * ex_width] = saturate_cast<T>(mean_left);
                            data_ex_src[c * ex_channels + (y + index_ys) * ex_height + (x + index_xe) * ex_width] = saturate_cast<T>(mean_right);
                        }
                    }

                    // 上端, 下端
                    double mean_top, mean_bottom;
                    for (int x = 0; x < width; ++x)
                    {
                        mean_top = mean_bottom = 0.0f;

                        for (int y = 0; y < ey; ++y)
                        {
                            mean_top = data_ex_src[c * ex_channels + (y + index_ys) * ex_height + (x + index_xs) * ex_width];
                            mean_bottom = data_ex_src[c * ex_channels + (y + index_ye - ey) * ex_height + (x + index_xs) * ex_width];
                        }
                        mean_top /= ey;
                        mean_bottom /= ey;

                        for (int y = 0; y < ey; ++y)
                        {
                            data_ex_src[c * ex_channels + y * ex_height + (x + index_xs) * ex_width] = saturate_cast<T>(mean_top);
                            data_ex_src[c * ex_channels + (y + index_ye) * ex_height + (x + index_xs) * ex_width] = saturate_cast<T>(mean_bottom);
                        }
                    }

                    // 四隅
                    float mean_tl, mean_tr, mean_bl, mean_br;
                    mean_tl = mean_tr = mean_bl = mean_br = 0.0f;

                    // 左上
                    mean_tl += data_ex_src[c * ex_channels + (index_ys - 1) * ex_height + index_xs * ex_width];
                    mean_tl += data_ex_src[c * ex_channels + index_ys * ex_height + (index_xs - 1) * ex_width];
                    mean_tl /= 2;
                    // 右上
                    mean_tr += data_ex_src[c * ex_channels + (index_ys - 1) * ex_height + (index_xe - 1) * ex_width];
                    mean_tr += data_ex_src[c * ex_channels + index_ys * ex_height + index_xe * ex_width];
                    mean_tr /= 2;
                    // 左下
                    mean_bl += data_ex_src[c * ex_channels + (index_ye - 1) * ex_height + (index_xs - 1) * ex_width];
                    mean_bl += data_ex_src[c * ex_channels + index_ye * ex_height + index_xs * ex_width];
                    mean_bl /= 2;
                    // 右下
                    mean_br += data_ex_src[c * ex_channels + (index_ye - 1) * ex_height + index_xe * ex_width];
                    mean_br += data_ex_src[c * ex_channels + index_ye * ex_height + (index_xe - 1) * ex_width];
                    mean_br /= 2;

                    for (int y = 0; y < ey; ++y) 
                    {
                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex_src[c * ex_channels + (y + 0) * ex_height + (x + 0) * ex_width] = saturate_cast<T>(mean_tl);
                            data_ex_src[c * ex_channels + (y + 0) * ex_height + (x + index_xe) * ex_width] = saturate_cast<T>(mean_tr);
                            data_ex_src[c * ex_channels + (y + index_ye) * ex_height + (x + 0) * ex_width] = saturate_cast<T>(mean_bl);
                            data_ex_src[c * ex_channels + (y + index_ye) * ex_height + (x + index_xe) * ex_width] = saturate_cast<T>(mean_br);
                        }
                    }
                }

                return ex_src;
            }
            else if (style == "reflect")
            {
                // cba|abcdefg|gfe

                for (int c = 0; c < channels; ++c)
                {
                    // 左端, 右端
                    for (int y = 0; y < height; ++y)
                    {
                        for (int x = 0; x < ex; ++x)
                        {
                            data_ex_src[c * ex_channels + index_ys * ex_height + (index_xs - 1 - x) * ex_width] =
                                data_ex_src[c * ex_channels + index_ys * ex_height + (x + index_xs) * ex_width];

                            data_ex_src[c * ex_channels + index_ys * ex_height + (x + index_xe) * ex_width] =
                                data_ex_src[c * ex_channels + index_ys * ex_height + (index_xe - 1 - x) * ex_width];
                        }
                    }

                    // 上端, 下端
                    for (int x = 0; x < width; ++x)
                    {
                        for (int y = 0; y < ey; ++y)
                        {
                            data_ex_src[c * ex_channels + (index_ys - 1 - y) * ex_height + index_xs * ex_width] =
                                data_ex_src[c * ex_channels + (y + index_ys) * ex_height + index_xs * ex_width];

                            data_ex_src[c * ex_channels + (y + index_ye) * ex_height + index_xs * ex_width] =
                                data_ex_src[c * ex_channels * (index_ye - 1 - y) * ex_height + index_xs * ex_width];
                        }
                    }

                    // 四隅
                    for (int y = 0; y < ey; ++y)
                    {
                        for (int x = 0; x < ex; ++x)
                        {
                            // 左上
                            data_ex_src[c * ex_channels + (index_ys - 1 - y) * ex_height + (index_xs - 1 - x) * ex_width] =
                                data_ex_src[c * ex_channels + (y + index_ys) * ex_height + (x + index_xs) * ex_width];

                            // 右上
                            data_ex_src[c * ex_channels + (index_ys - 1 - y) * ex_height + (x + index_xs) * ex_width] =
                                data_ex_src[c * ex_channels + (y + index_ys) * ex_height + (index_xe - 1 - x) * ex_width];

                            // 左下
                            data_ex_src[c * ex_channels + (y + index_ye) * ex_height + (index_xs - 1 - x) * ex_width] =
                                data_ex_src[c * ex_channels + (index_ye - 1 - y) * ex_height + (x + index_xs) * ex_width];
                            
                            // 右下
                            data_ex_src[c * ex_channels + (y + index_ye) * ex_height + (x + index_xe) * ex_width] =
                                data_ex_src[c * ex_channels + (index_ye - 1 - y) * ex_height + (index_xe - 1 - x) * ex_width];
                        }
                    }
                }
            }
            else if (style == "replicate")
            {
                // aaa|abcdefg|ggg
            }
            else if (style == "warp")
            {
                // efg|abcdefg|abc
            }
            else
            {
                std::string msg = utils::format_string(
                    "No matching `zero`, `mean`, `reflect`, `replicate` or `warp`. Given is %s", style.c_str());
                throw std::runtime_error(msg);
            }
        }

        // template <typename T>
        // NdArrayPtr linear_filter(NdArrayPtr src, int ksize_x, int ksize_y, double *kernel)
        // {
        //     IMGPROC_CHECK_IMAGE_SHAPE(src);

        //     const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
        //     auto ndim = src->ndim();
        //     auto sh = src->shape();
        //     auto st = src->strides();
        //     int ch = shape[0];
        //     int h = shape[1];
        //     int w = shape[2];
        //     T *data = src->template cast_data_and_get_pointer<T>(ctx);
        //     // -----------------------------

        //     if (!kernel)
        //     {
        //         throw std::runtime_error("kernel is nullptr.");
        //     }

        //     if (ksize_x % 2 == 0)
        //         ksize_x += 1;
        //     if (ksize_y % 2 == 0)
        //         ksize_y += 1;

        //     int hlf_ksx = (int)(ksize_x / 2);
        //     int hlf_ksy = (int)(ksize_y / 2);

        //     dst = zeros<float>({ch, h + 2 * hlf_ksy, w + 2 * hlf_ksx});

        // }
    }
}