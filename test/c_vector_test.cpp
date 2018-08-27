/**
 * MIT License
 *
 * Copyright (c) 2017 MatrixJoeQ
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include "c_internal.h"
#include "c_vector.h"
#include "c_test_util.hpp"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

#pragma GCC diagnostic ignored "-Weffc++"
class CVectorTest : public ::testing::Test
{
public:
    CVectorTest() : vector_(0) {}
    ~CVectorTest() { c_vector_destroy(vector_); }

    void SetupVector(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_vector_push_back(vector_, C_REF_T(&datas[i]));

        ExpectNotEmpty();
        EXPECT_EQ(length, c_vector_size(vector_));
    }

    void SetupPerformance()
    {
        perf_data_.clear();
        perf_data_.resize(__PERF_SET_SIZE);
        srandom(static_cast<unsigned int>(time(0)));
        for (auto& data : perf_data_) {
            data = random() % INT32_MAX;
        }
    }

    void ExpectEqualToArray(const int* datas, int length)
    {
        EXPECT_EQ(length, c_vector_size(vector_));
        c_vector_iterator_t first = c_vector_begin(vector_);
        c_ref_t data = 0;
        for (int i = 0; i < length; ++i) {
            data = C_ITER_DEREF(&first);
            EXPECT_EQ(datas[i], C_DEREF_INT(data));
            C_ITER_INC(&first);
        }
    }

    void ExpectEmpty()
    {
        c_vector_iterator_t first = c_vector_begin(vector_);
        c_vector_iterator_t last = c_vector_end(vector_);
        EXPECT_TRUE(c_vector_empty(vector_));
        EXPECT_EQ(0, c_vector_size(vector_));
        EXPECT_TRUE(C_ITER_EQ(&first, &last));
    }

    void ExpectNotEmpty()
    {
        c_vector_iterator_t first = c_vector_begin(vector_);
        c_vector_iterator_t last = c_vector_end(vector_);
        EXPECT_FALSE(c_vector_empty(vector_));
        EXPECT_NE(0, c_vector_size(vector_));
        EXPECT_TRUE(C_ITER_NE(&first, &last));
    }

    void SetUp()
    {
        vector_ = C_VECTOR_INT;
        ExpectEmpty();
    }

    void TearDown()
    {
        c_vector_destroy(vector_);
        vector_ = 0;
        EXPECT_TRUE(c_vector_empty(vector_));
        EXPECT_EQ(0, c_vector_size(vector_));
    }

protected:
    c_vector_t* vector_;
    std::vector<int> perf_data_;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CVectorTest, Create)
{
    // create vector from large array
    SetupPerformance();
    c_vector_t* v = c_vector_create_from_array(
        c_get_int_type_info(), perf_data_.data(), __PERF_SET_SIZE);
    c_vector_assign(vector_, v);
    ExpectEqualToArray(perf_data_.data(), __PERF_SET_SIZE);
    c_vector_destroy(v);

    int i = 100;
    v = c_vector_create_n(c_get_int_type_info(), __PERF_SET_SIZE, C_REF_T(&i));
    auto v_copy = c_vector_copy(v);

    auto expect_vector_equal_to_i = [i](c_vector_t* vec) {
        auto first = c_vector_begin(vec);
        auto last = c_vector_end(vec);
        while (C_ITER_NE(&first, &last)) {
            EXPECT_EQ(i, C_DEREF_INT(C_ITER_DEREF(&first)));
            C_ITER_INC(&first);
        }
    };

    expect_vector_equal_to_i(v);
    expect_vector_equal_to_i(v_copy);

    c_vector_destroy(v);
    c_vector_destroy(v_copy);
    v = 0;
    v_copy = 0;
}

TEST_F(CVectorTest, Clear)
{
    // clear a non-empty vector
    SetupVector(default_data, default_length);
    c_vector_clear(vector_);
    ExpectEmpty();

    // clear an empty vector
    c_vector_clear(vector_);
    ExpectEmpty();
}

TEST_F(CVectorTest, BeginEnd)
{
    // iterate a non-empty vector
    SetupVector(default_data, default_length);
    c_vector_iterator_t first = c_vector_begin(vector_);
    c_vector_iterator_t last = c_vector_end(vector_);
    c_vector_iterator_t rfirst = c_vector_rbegin(vector_);
    c_vector_iterator_t rlast = c_vector_rend(vector_);
    C_ITER_DEC(&rlast);
    while (C_ITER_NE(&first, &last)) {
        EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&first)), C_DEREF_INT(C_ITER_DEREF(&rlast)));
        C_ITER_INC(&first);
        C_ITER_DEC(&rlast);
    }

    first = c_vector_begin(vector_);
    last = c_vector_end(vector_);
    rfirst = c_vector_rbegin(vector_);
    rlast = c_vector_rend(vector_);
    C_ITER_DEC(&last);
    while (C_ITER_NE(&rfirst, &rlast)) {
        EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&rfirst)), C_DEREF_INT(C_ITER_DEREF(&last)));
        C_ITER_INC(&rfirst);
        C_ITER_DEC(&last);
    }

    // iterate an empty vector
    c_vector_clear(vector_);
    first = c_vector_begin(vector_);
    last = c_vector_end(vector_);
    rfirst = c_vector_rbegin(vector_);
    rlast = c_vector_rend(vector_);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));
    EXPECT_TRUE(C_ITER_EQ(&rfirst, &rlast));
}

TEST_F(CVectorTest, At)
{
    SetupVector(default_data, default_length);

    for (int i = 0; i < default_length; ++i) {
        EXPECT_EQ(i, C_DEREF_INT(c_vector_at(vector_, i)));
    }
}

TEST_F(CVectorTest, BackOperations)
{
    // back operations on a non-empty vector
    SetupVector(default_data, default_length);

    c_vector_iterator_t first, last;
    size_t size = 0;
    c_ref_t data = 0;
    while (!c_vector_empty(vector_)) {
        size = c_vector_size(vector_);
        first = c_vector_begin(vector_);
        last = c_vector_end(vector_);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_vector_back(vector_);
        EXPECT_EQ(size - 1, C_DEREF_INT(data));
        c_vector_pop_back(vector_);
        EXPECT_EQ(size - 1, c_vector_size(vector_));
    }

    // back operations on an empty vector
    ExpectEmpty();
    data = c_vector_back(vector_);
    EXPECT_TRUE(data == 0);
    c_vector_pop_back(vector_); // nothing should happen
}

TEST_F(CVectorTest, FrontOperations)
{
    // front operations on a non-empty vector
    SetupVector(default_data, default_length);

    c_vector_iterator_t first, last;
    size_t index = 0;
    c_ref_t data = 0;
    while (!c_vector_empty(vector_)) {
        first = c_vector_begin(vector_);
        last = c_vector_end(vector_);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_vector_front(vector_);
        EXPECT_EQ(default_data[index++], C_DEREF_INT(data));
        c_vector_erase(vector_, first);
    }

    // front operations on an empty vector
    ExpectEmpty();
    data = c_vector_front(vector_);
    EXPECT_TRUE(data == 0);
}

TEST_F(CVectorTest, Data)
{
    SetupVector(default_data, default_length);
    c_ref_t data = c_vector_data(vector_);
    EXPECT_EQ(data, c_vector_front(vector_));
}

TEST_F(CVectorTest, Reserve)
{
    SetupVector(default_data, default_length);
    size_t old_cap = c_vector_capacity(vector_);

    c_vector_iterator_t first = c_vector_begin(vector_);
    c_vector_iterator_t last = c_vector_end(vector_);
    c_vector_reserve(vector_, 0);
    c_vector_iterator_t new_first = c_vector_begin(vector_);
    c_vector_iterator_t new_last = c_vector_end(vector_);
    ExpectEqualToArray(default_data, default_length);
    EXPECT_TRUE(C_ITER_EQ(&first, &new_first));
    EXPECT_TRUE(C_ITER_EQ(&last, &new_last));

    c_vector_reserve(vector_, old_cap);
    new_first = c_vector_begin(vector_);
    new_last = c_vector_end(vector_);
    ExpectEqualToArray(default_data, default_length);
    EXPECT_TRUE(C_ITER_EQ(&first, &new_first));
    EXPECT_TRUE(C_ITER_EQ(&last, &new_last));

    c_vector_reserve(vector_, old_cap + 1);
    new_first = c_vector_begin(vector_);
    new_last = c_vector_end(vector_);
    EXPECT_TRUE(C_ITER_NE(&first, &new_first));
    EXPECT_TRUE(C_ITER_NE(&last, &new_last));
    EXPECT_EQ(old_cap * 2, c_vector_capacity(vector_));

    old_cap = c_vector_capacity(vector_);
    first = c_vector_begin(vector_);
    last = c_vector_end(vector_);
    c_vector_reserve(vector_, old_cap * 2 + 1);
    new_first = c_vector_begin(vector_);
    new_last = c_vector_end(vector_);
    EXPECT_TRUE(C_ITER_NE(&first, &new_first));
    EXPECT_TRUE(C_ITER_NE(&last, &new_last));
    EXPECT_EQ(old_cap * 2 + 1, c_vector_capacity(vector_));
}

TEST_F(CVectorTest, Shrink)
{
    SetupVector(default_data, default_length);
    EXPECT_NE(c_vector_size(vector_), c_vector_capacity(vector_));
    c_vector_shrink_to_fit(vector_);
    EXPECT_EQ(c_vector_size(vector_), c_vector_capacity(vector_));

    c_vector_clear(vector_);
    EXPECT_TRUE(0 == c_vector_front(vector_));
    c_vector_shrink_to_fit(vector_);
    EXPECT_TRUE(0 == c_vector_front(vector_));
    EXPECT_TRUE(0 == c_vector_back(vector_));
    EXPECT_EQ(0, c_vector_capacity(vector_));
}

TEST_F(CVectorTest, Resize)
{
    SetupVector(default_data, default_length);

    int bigger[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0 };
    c_vector_resize(vector_, c_vector_size(vector_) + 3);
    ExpectEqualToArray(bigger, __array_length(bigger));

    int bigger_with_value[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 6, 6, 6 };
    int value = 6;
    c_vector_resize_with_value(vector_, c_vector_size(vector_) + 3, C_REF_T(&value));
    ExpectEqualToArray(bigger_with_value, __array_length(bigger_with_value));

    int smaller[] = { 0, 1, 2, 3, 4, 5 };
    c_vector_resize(vector_, 6);
    ExpectEqualToArray(smaller, __array_length(smaller));

    int even_smaller[] = { 0, 1, 2, 3, 4 };
    c_vector_resize(vector_, 5);
    ExpectEqualToArray(even_smaller, __array_length(even_smaller));
}

TEST_F(CVectorTest, Swap)
{
    SetupVector(default_data, default_length);

    c_vector_t* other = C_VECTOR_INT;

    c_vector_swap(vector_, other);
    ExpectEmpty();
    c_vector_swap(vector_, other);
    ExpectEqualToArray(default_data, default_length);
    EXPECT_TRUE(c_vector_empty(other));

    c_vector_destroy(other);
}

TEST_F(CVectorTest, InsertErase)
{
    c_vector_iterator_t first, last, iter;
    c_ref_t data = 0;

    for (int i = 0; i < 9; ++i) {
        last = c_vector_end(vector_);
        iter = c_vector_insert(vector_, last, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        EXPECT_EQ(i, C_DEREF_INT(data));
        EXPECT_EQ(i + 1, c_vector_size(vector_));
    }

    ExpectNotEmpty();

    first = c_vector_begin(vector_);
    last = c_vector_end(vector_);
    while (C_ITER_NE(&first, &last)) {
        size_t size = c_vector_size(vector_);
        first = c_vector_erase(vector_, first);
        last = c_vector_end(vector_);
        EXPECT_EQ(size - 1, c_vector_size(vector_));
    }

    first = c_vector_erase(vector_, first);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));

    for (int i = 0; i < 9; ++i) {
        first = c_vector_begin(vector_);
        iter = c_vector_insert(vector_, first, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        first = c_vector_begin(vector_);
        EXPECT_TRUE(C_ITER_EQ(&first, &iter));
        EXPECT_EQ(i, C_DEREF_INT(data));
    }

    first = c_vector_begin(vector_);
    last = c_vector_end(vector_);
    first = c_vector_erase_range(vector_, first, last);
    ExpectEmpty();
}

} // namespace
} // namespace c_container
