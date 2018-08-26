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
#include <vector>
#include <algorithm>
#include "c_internal.h"
#include "c_vector.h"
#include "c_algorithm.h"
#include "c_test_util.hpp"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

void print_value(c_ref_t data)
{
    printf("%d ", C_DEREF_INT(data));
}

#pragma GCC diagnostic ignored "-Weffc++"
class CHeapTest : public ::testing::Test
{
public:
    CHeapTest() : v_(0) {}
    ~CHeapTest() { c_vector_destroy(v_); }

    void SetupVector(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_vector_push_back(v_, C_REF_T(&datas[i]));
        v_first_ = c_vector_begin(v_);
        v_last_ = c_vector_end(v_);
    }

    void SetupPerformance(void)
    {
        std_v_.clear();
        std_v_.reserve(__PERF_SET_SIZE);
        c_vector_clear(v_);
        c_vector_reserve(v_, __PERF_SET_SIZE);
        srandom(static_cast<unsigned int>(time(0)));
        int data = 0;
        for (auto iter = std_v_.begin(); iter != std_v_.end(); ++iter) {
            data = random() % INT32_MAX;
            *iter = data;
            c_vector_push_back(v_, C_REF_T(&data));
        }
        v_first_ = c_vector_begin(v_);
        v_last_ = c_vector_end(v_);
        std_first_ = std_v_.begin();
        std_last_ = std_v_.end();
    }

    void SetUp()
    {
        v_ = C_VECTOR_INT;
        v_first_ = c_vector_begin(v_);
        v_last_ = c_vector_end(v_);
    }

    void TearDown()
    {
        c_vector_destroy(v_);
        v_ = 0;

        std_v_.clear();
        std_v_.shrink_to_fit();
    }

protected:
    c_vector_t* v_;
    c_vector_iterator_t v_first_;
    c_vector_iterator_t v_last_;
    std::vector<int> std_v_;
    std::vector<int>::iterator std_first_;
    std::vector<int>::iterator std_last_;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CHeapTest, IsHeap)
{
    SetupVector(default_data, default_length);
    EXPECT_FALSE(c_algo_is_heap(&v_first_, &v_last_));

    int max_heap[] = { 68, 50, 65, 21, 31, 32, 26, 19, 16, 13, 24 };
    c_vector_clear(v_);
    SetupVector(max_heap, __array_length(max_heap));
    EXPECT_TRUE(c_algo_is_heap(&v_first_, &v_last_));
}

TEST_F(CHeapTest, PushHeap)
{
    EXPECT_TRUE(c_algo_is_heap(&v_first_, &v_last_));

    for (int i : default_data) {
        c_vector_push_back(v_, C_REF_T(&i));
        v_first_ = c_vector_begin(v_);
        v_last_ = c_vector_end(v_);
        c_algo_push_heap(&v_first_, &v_last_);
        c_algo_for_each(&v_first_, &v_last_, print_value);
        printf("\n");
        EXPECT_TRUE(c_algo_is_heap(&v_first_, &v_last_));
    }
}

TEST_F(CHeapTest, PopHeap)
{
    SetupVector(default_data, default_length);
    c_algo_make_heap(&v_first_, &v_last_);
    EXPECT_TRUE(c_algo_is_heap(&v_first_, &v_last_));
    c_algo_for_each(&v_first_, &v_last_, print_value);
    printf("\n");

    while (!c_vector_empty(v_)) {
        c_algo_pop_heap(&v_first_, &v_last_);
        c_algo_for_each(&v_first_, &v_last_, print_value);
        printf("\n");
        c_vector_pop_back(v_);
        v_last_ = c_vector_end(v_);
        EXPECT_TRUE(c_algo_is_heap(&v_first_, &v_last_));
    }
}

TEST_F(CHeapTest, MakeHeap)
{
    SetupVector(default_data, default_length);
    c_algo_make_heap(&v_first_, &v_last_);
    EXPECT_TRUE(c_algo_is_heap(&v_first_, &v_last_));
    c_algo_for_each(&v_first_, &v_last_, print_value);
    printf("\n");
}

TEST_F(CHeapTest, SortHeap)
{
    SetupVector(default_data, default_length);
    c_algo_make_heap(&v_first_, &v_last_);
    EXPECT_TRUE(c_algo_is_heap(&v_first_, &v_last_));
    c_algo_for_each(&v_first_, &v_last_, print_value);
    printf("\n");

    c_algo_sort_heap(&v_first_, &v_last_);
    c_algo_for_each(&v_first_, &v_last_, print_value);
    printf("\n");
    EXPECT_TRUE(c_algo_is_sorted(&v_first_, &v_last_));
}

TEST_F(CHeapTest, HeapPerformance)
{
    bool std_ret = false;
    bool c_ret = false;
    __TEST_LOOP {
        SetupPerformance();
        __c_measure(std_ret = std::is_heap(std_first_, std_last_));
        __c_measure(c_ret = c_algo_is_heap(&v_first_, &v_last_));
        EXPECT_EQ(std_ret, c_ret);

        __c_measure(std::make_heap(std_first_, std_last_));
        __c_measure(c_algo_make_heap(&v_first_, &v_last_));

        __c_measure(std_ret = std::is_heap(std_first_, std_last_));
        __c_measure(c_ret = c_algo_is_heap(&v_first_, &v_last_));
        EXPECT_EQ(std_ret, c_ret);

        __c_measure(std::sort_heap(std_first_, std_last_));
        __c_measure(c_algo_sort_heap(&v_first_, &v_last_));

        __c_measure(std_ret = std::is_heap(std_first_, std_last_));
        __c_measure(c_ret = c_algo_is_heap(&v_first_, &v_last_));
        EXPECT_EQ(std_ret, c_ret);
    }
}

} // namespace
} // namespace c_container
