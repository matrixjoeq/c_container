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
#include <list>
#include <algorithm>
#include "c_internal.h"
#include "c_list.h"
#include "c_algorithm.h"
#include "c_test_util.hpp"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 3, 3, 3, 4, 6, 6, 7, 9 };
const int default_length = __array_length(default_data);

#pragma GCC diagnostic ignored "-Weffc++"
class CBinarySearchTest : public ::testing::Test
{
public:
    CBinarySearchTest() : l_(0) {}
    ~CBinarySearchTest() { TearDown(); }

    void SetupList(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_list_push_back(l_, C_REF_T(&datas[i]));
        first_ = c_list_begin(l_);
        last_ = c_list_end(l_);
    }

    void SetupPerformance(void)
    {
        std_list_.clear();
        std_list_.resize(__PERF_SET_SIZE);
        srand(static_cast<unsigned int>(time(0)));
        int data = 0;
        for (auto iter = std_list_.begin(); iter != std_list_.end(); ++iter) {
            data = rand() % INT32_MAX;
            *iter = data;
            c_list_push_back(l_, C_REF_T(&data));
        }
        first_ = c_list_begin(l_);
        last_ = c_list_end(l_);
        std_first_ = std_list_.begin();
        std_last_ = std_list_.end();
    }

    void SetUp()
    {
        l_ = C_LIST_INT;
        first_ = c_list_begin(l_);
        last_ = c_list_end(l_);
        output_ = 0;
    }

    void TearDown()
    {
        __c_free(output_);
        c_list_destroy(l_);
        l_ = 0;
        output_ = 0;

        std_list_.clear();
    }

protected:
    c_list_t* l_;
    c_list_iterator_t first_;
    c_list_iterator_t last_;
    c_list_iterator_t* output_;
    std::list<int> std_list_;
    std::list<int>::iterator std_first_;
    std::list<int>::iterator std_last_;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CBinarySearchTest, Bound)
{
    SetupList(default_data, default_length);

    int value = -1;
    c_algo_lower_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(0, C_ITER_DISTANCE(&first_, output_));
    c_algo_upper_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(0, C_ITER_DISTANCE(&first_, output_));

    value = 0;
    c_algo_lower_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(0, C_ITER_DISTANCE(&first_, output_));
    c_algo_upper_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(1, C_ITER_DISTANCE(&first_, output_));

    value = 1;
    c_algo_lower_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(1, C_ITER_DISTANCE(&first_, output_));
    c_algo_upper_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first_, output_));

    value = 2;
    c_algo_lower_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first_, output_));
    c_algo_upper_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first_, output_));

    value = 3;
    c_algo_lower_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first_, output_));
    c_algo_upper_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(5, C_ITER_DISTANCE(&first_, output_));

    value = 9;
    c_algo_lower_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(9, C_ITER_DISTANCE(&first_, output_));
    c_algo_upper_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(10, C_ITER_DISTANCE(&first_, output_));

    value = 10;
    c_algo_lower_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(10, C_ITER_DISTANCE(&first_, output_));
    c_algo_upper_bound(&first_, &last_, &value, &output_);
    EXPECT_EQ(10, C_ITER_DISTANCE(&first_, output_));
}

TEST_F(CBinarySearchTest, BoundPerformance)
{
    SetupPerformance();

    __TEST_LOOP {
        int x = rand() % INT32_MAX;
        __c_measure(std::lower_bound(std_first_, std_last_, x));
        __c_measure(std::upper_bound(std_first_, std_last_, x));
        __c_measure(c_algo_lower_bound(&first_, &last_, &x, &output_));
        __c_measure(c_algo_upper_bound(&first_, &last_, &x, &output_));
    }
}

TEST_F(CBinarySearchTest, BinarySearch)
{
    SetupList(default_data, default_length);

    int value = -1;
    EXPECT_FALSE(c_algo_binary_search(&first_, &last_, &value));

    value = 0;
    EXPECT_TRUE(c_algo_binary_search(&first_, &last_, &value));

    value = 2;
    EXPECT_FALSE(c_algo_binary_search(&first_, &last_, &value));

    value = 3;
    EXPECT_TRUE(c_algo_binary_search(&first_, &last_, &value));

    value = 5;
    EXPECT_FALSE(c_algo_binary_search(&first_, &last_, &value));

    value = 6;
    EXPECT_TRUE(c_algo_binary_search(&first_, &last_, &value));

    value = 9;
    EXPECT_TRUE(c_algo_binary_search(&first_, &last_, &value));

    value = 10;
    EXPECT_FALSE(c_algo_binary_search(&first_, &last_, &value));
}

TEST_F(CBinarySearchTest, BinarySearchPerformance)
{
    SetupPerformance();

    bool std_ret = false;
    bool c_ret = false;
    __TEST_LOOP {
        int x = rand() % INT32_MAX;
        __c_measure(std_ret = std::binary_search(std_first_, std_last_, x));
        __c_measure(c_ret = c_algo_binary_search(&first_, &last_, &x));
        EXPECT_EQ(std_ret, c_ret);
    }
}

TEST_F(CBinarySearchTest, EqualRange)
{
    SetupList(default_data, default_length);

    int value = -1;
    c_iterator_t* lower = 0;
    c_iterator_t* upper = 0;

    c_algo_equal_range(&first_, &last_, &value, &lower, &upper);
    EXPECT_TRUE(C_ITER_EQ(lower, &first_));
    EXPECT_TRUE(C_ITER_EQ(upper, &first_));

    value = 0;
    c_algo_equal_range(&first_, &last_, &value, &lower, &upper);
    EXPECT_EQ(0, C_ITER_DISTANCE(&first_, lower));
    EXPECT_EQ(1, C_ITER_DISTANCE(&first_, upper));

    value = 2;
    c_algo_equal_range(&first_, &last_, &value, &lower, &upper);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first_, lower));
    EXPECT_EQ(2, C_ITER_DISTANCE(&first_, upper));

    value = 3;
    c_algo_equal_range(&first_, &last_, &value, &lower, &upper);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first_, lower));
    EXPECT_EQ(5, C_ITER_DISTANCE(&first_, upper));

    value = 5;
    c_algo_equal_range(&first_, &last_, &value, &lower, &upper);
    EXPECT_EQ(6, C_ITER_DISTANCE(&first_, lower));
    EXPECT_EQ(6, C_ITER_DISTANCE(&first_, upper));

    value = 6;
    c_algo_equal_range(&first_, &last_, &value, &lower, &upper);
    EXPECT_EQ(6, C_ITER_DISTANCE(&first_, lower));
    EXPECT_EQ(8, C_ITER_DISTANCE(&first_, upper));

    value = 9;
    c_algo_equal_range(&first_, &last_, &value, &lower, &upper);
    EXPECT_EQ(9, C_ITER_DISTANCE(&first_, lower));
    EXPECT_TRUE(C_ITER_EQ(upper, &last_));

    value = 10;
    c_algo_equal_range(&first_, &last_, &value, &lower, &upper);
    EXPECT_TRUE(C_ITER_EQ(lower, &last_));
    EXPECT_TRUE(C_ITER_EQ(upper, &last_));

    __c_free(lower);
    __c_free(upper);
}

} // namespace
} // namespace c_container
