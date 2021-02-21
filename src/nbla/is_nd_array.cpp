#include "nbla/is_nd_array.hpp"

#include <functional>
#include <memory>
#include <string>

namespace Is
{
    namespace nbla
    {
        void IsNdArray::update_shape_info()
        {
            size_ = compute_size_by_shape(shape_);
            strides_ = get_c_contiguous_strides(shape_);
            ndim_ = shape_.size();
        }


        IsNdArray::IsNdArray(const Shape_t& shape)
            : shape_(shape)
        {
            update_shape_info();
            data_.reset(new NdArray(shape_));
        }


        IsNdArray::IsNdArray(NdArrayPtr data)
        {
            shape_ = data->shape();
            update_shape_info();
            data_ = data;
        }


        void IsNdArray::reshape(const vector<Size_t>& shape, bool force)
        {
            if (shape_ == shape)
                return;

            const Size_t size = compute_size_by_shape(shape);
            if (size_ == size)
            {
                shape_ = shape;
                update_shape_info();
                data_->reshape(shape);
                return;
            }

            NBLA_CHECK(force, error_code::value,
                       "Total dimensions not match. Set force=true if you want to "
                       "resize array (clearing data). Given: %d != current: %d.",
                       size, size_);
            
            shape_ = shape;
            update_shape_info();
            data_->reshape(shape_, true);
        }


        IsNdArrayPtr IsNdArray::view()
        {
            shared_ptr<IsNdArray> v(new IsNdArray(shape_)); // privatekコンストラクタのためstd::make_shared使えない
            v->set_data(data_);
            return v;
        }


        IsNdArrayPtr IsNdArray::view(const Shape_t& shape)
        {
            const Size_t size = compute_size_by_shape(shape);
            NBLA_CHECK(size == size_, error_code::value,
                       "The total size must be the same as the variable. "
                       "Given: %d != current: %d.",
                       size, size_);

            shared_ptr<IsNdArray> v(new IsNdArray(shape));
            v->set_data(data_->view(shape));
            return v;
        }


        Size_t IsNdArray::size(Size_t axis) const
        {
            if (axis <= 0)
                return size_;
            
            return compute_size_by_shape(shape_, axis);
        }


        void IsNdArray::set_data(NdArrayPtr data)
        {
            NBLA_CHECK(data->shape() == shape_, error_code::value, "Shape must match.");
            data_ = data;
        }
    }
}