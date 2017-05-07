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
#include "c_test_util.hpp"
#include "c_internal.h"
#include "c_list.h"
#include "c_algorithm.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

bool is_even(c_ref_t value)
{
    return (*(int*)value) % 2 == 0;
}

#pragma GCC diagnostic ignored "-Weffc++"
class CPartitionTest : public ::testing::Test
{
public:
    CPartitionTest() : l_(0) {}
    ~CPartitionTest() { TearDown(); }

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
        srandom(static_cast<unsigned int>(time(0)));
        int data = 0;
        for (auto iter = std_list_.begin(); iter != std_list_.end(); ++iter) {
            data = random() % INT32_MAX;
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

TEST_F(CPartitionTest, IsPartitioned)
{
    SetupList(default_data, default_length);
    EXPECT_FALSE(c_algo_is_partitioned(&first_, &last_, is_even));

    c_list_clear(l_);
    int numbers[] = { 0, 2, 4, 6, 8, 3, 5, 9, 1, 7 };
    SetupList(numbers, __array_length(numbers));
    first_ = c_list_begin(l_);
    last_ = c_list_end(l_);
    EXPECT_TRUE(c_algo_is_partitioned(&first_, &last_, is_even));
}

TEST_F(CPartitionTest, Partition)
{
    SetupList(default_data, default_length);

    EXPECT_FALSE(c_algo_is_partitioned(&first_, &last_, is_even));
    c_algo_partition(&first_, &last_, &output_, is_even);
    EXPECT_TRUE(c_algo_is_partitioned(&first_, &last_, is_even));
}

TEST_F(CPartitionTest, PartitionPerformance)
{
    SetupPerformance();

    __c_measure(std::partition(std_first_, std_last_, [](int i){return i % 2 == 0;}));
    __c_measure(c_algo_partition(&first_, &last_, &output_, is_even));

    bool std_ret = false;
    bool c_ret = false;
    __c_measure(std_ret = std::is_partitioned(std_first_, std_last_, [](int i){return i % 2 == 0;}));
    __c_measure(c_ret = c_algo_is_partitioned(&first_, &last_, is_even));
    EXPECT_EQ(std_ret, c_ret);

    std::list<int>::iterator point;
    __c_measure(point = std::partition_point(std_first_, std_last_, [](int i){return i % 2 == 0;}));
    __c_measure(c_algo_partition_point(&first_, &last_, &output_, is_even));
    EXPECT_FALSE(is_even(C_ITER_DEREF(output_)));
}

TEST_F(CPartitionTest, PartitionCopy)
{
    SetupList(default_data, default_length);

    c_list_t* even = C_LIST_INT;
    c_list_t* odd = C_LIST_INT;
    c_list_resize(even, default_length);
    c_list_resize(odd, default_length);
    c_list_iterator_t e_first = c_list_begin(even);
    c_list_iterator_t* e_end = 0;
    c_list_iterator_t o_first = c_list_begin(odd);
    c_list_iterator_t* o_end = 0;

    int even_numbers[] = { 0, 2, 4, 6, 8 };
    int odd_numbers[] = { 1, 3, 5, 7, 9 };
    c_list_t* expected_even = c_list_create_from(c_get_int_type_info(), even_numbers, __array_length(even_numbers));
    c_list_t* expected_odd = c_list_create_from(c_get_int_type_info(), odd_numbers, __array_length(odd_numbers));
    c_list_iterator_t ee_first = c_list_begin(expected_even);
    c_list_iterator_t eo_first = c_list_begin(expected_odd);

    EXPECT_EQ(default_length, c_algo_partition_copy(&first_, &last_, &e_first, &o_first, &e_end, &o_end, is_even));
    EXPECT_TRUE(c_algo_equal(&e_first, e_end, &ee_first));
    EXPECT_TRUE(c_algo_equal(&o_first, o_end, &eo_first));

    c_list_destroy(expected_even);
    c_list_destroy(expected_odd);
    c_list_destroy(even);
    c_list_destroy(odd);
}

TEST_F(CPartitionTest, PartitionCopyPerformance)
{
    SetupPerformance();

    c_list_t* dest_true = C_LIST_INT;
    c_list_t* dest_false = C_LIST_INT;
    c_list_resize(dest_true, __PERF_SET_SIZE);
    c_list_resize(dest_false, __PERF_SET_SIZE);
    c_list_iterator_t t_first = c_list_begin(dest_true);
    c_list_iterator_t f_first = c_list_begin(dest_false);

    __c_measure(c_algo_partition_copy(&first_, &last_, &t_first, &f_first, C_IGNORED, C_IGNORED, is_even));

    c_list_destroy(dest_true);
    c_list_destroy(dest_false);
}

TEST_F(CPartitionTest, PartitionPoint)
{
    SetupList(default_data, default_length);
    c_algo_partition(&first_, &last_, &output_, is_even);
    c_algo_partition_point(&first_, &last_, &output_, is_even);
    EXPECT_TRUE(c_algo_all_of(&first_, output_, is_even));
    EXPECT_TRUE(c_algo_none_of(output_, &last_, is_even));
}

} // namespace
} // namespace c_container
