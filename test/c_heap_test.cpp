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
#include "c_internal.h"
#include "c_vector.h"
#include "c_algorithm.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

#pragma GCC diagnostic ignored "-Weffc++"
class CHeapTest : public ::testing::Test
{
public:
    CHeapTest() : __v(0) {}
    ~CHeapTest() { c_vector_destroy(__v); }

    void SetupVector(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_vector_push_back(__v, C_REF_T(&datas[i]));
        __first = c_vector_begin(__v);
        __last = c_vector_end(__v);
    }

    void SetUp()
    {
        __v = C_VECTOR_INT;
        __first = c_vector_begin(__v);
        __last = c_vector_end(__v);
    }

    void TearDown()
    {
        c_vector_destroy(__v);
        __v = 0;
    }

protected:
    c_vector_t* __v;
    c_vector_iterator_t __first;
    c_vector_iterator_t __last;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CHeapTest, IsHeap)
{
    SetupVector(default_data, default_length);
    EXPECT_FALSE(c_algo_is_heap(&__first, &__last));

    int max_heap[] = { 68, 50, 65, 21, 31, 32, 26, 19, 16, 13, 24 };
    c_vector_clear(__v);
    SetupVector(max_heap, __array_length(max_heap));
    EXPECT_TRUE(c_algo_is_heap(&__first, &__last));
}

TEST_F(CHeapTest, PushHeap)
{
    EXPECT_TRUE(c_algo_is_heap(&__first, &__last));

    for (int i : default_data) {
        c_vector_push_back(__v, C_REF_T(&i));
        __first = c_vector_begin(__v);
        __last = c_vector_end(__v);
        c_algo_push_heap(&__first, &__last);
        EXPECT_TRUE(c_algo_is_heap(&__first, &__last));
    }
}

TEST_F(CHeapTest, PopHeap)
{
    SetupVector(default_data, default_length);
    c_algo_make_heap(&__first, &__last);
    EXPECT_TRUE(c_algo_is_heap(&__first, &__last));

    while (!c_vector_empty(__v)) {
        c_algo_pop_heap(&__first, &__last);
        c_vector_pop_back(__v);
        __last = c_vector_end(__v);
        EXPECT_TRUE(c_algo_is_heap(&__first, &__last));
    }
}

TEST_F(CHeapTest, MakeHeap)
{
    SetupVector(default_data, default_length);
    c_algo_make_heap(&__first, &__last);
    EXPECT_TRUE(c_algo_is_heap(&__first, &__last));
}

TEST_F(CHeapTest, SortHeap)
{
    SetupVector(default_data, default_length);
    c_algo_make_heap(&__first, &__last);
    EXPECT_TRUE(c_algo_is_heap(&__first, &__last));

    c_algo_sort_heap(&__first, &__last);
    EXPECT_TRUE(c_algo_is_sorted(&__first, &__last));
}

} // namespace
} // namespace c_container
