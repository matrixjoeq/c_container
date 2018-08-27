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
#include <algorithm>
#include <vector>
#include "c_internal.h"
#include "c_vector.h"
#include "c_algorithm.h"
#include "c_test_util.hpp"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

bool greater(c_ref_t lhs, c_ref_t rhs)
{
    return (*(int*)lhs) > (*(int*)rhs);
}

void print_value(c_ref_t data)
{
    printf("%d ", C_DEREF_INT(data));
}

inline void print_newline(void)
{
    printf("\n");
}

#pragma GCC diagnostic ignored "-Weffc++"
class CSortTest : public ::testing::Test
{
public:
    CSortTest() : vector(0) {}
    ~CSortTest() { TearDown(); }

    void SetupVector(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_vector_push_back(vector, C_REF_T(&datas[i]));
        first = c_vector_begin(vector);
        last = c_vector_end(vector);
    }

    void SetUp()
    {
        vector = C_VECTOR_INT;
        first = c_vector_begin(vector);
        last = c_vector_end(vector);
        output = 0;
    }

    void TearDown()
    {
        __c_free(output);
        c_vector_destroy(vector);
        vector = 0;
        output = 0;
    }

protected:
    c_vector_t* vector;
    c_vector_iterator_t first;
    c_vector_iterator_t last;
    c_vector_iterator_t* output;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CSortTest, IsSorted)
{
    SetupVector(default_data, default_length);
    EXPECT_TRUE(c_algo_is_sorted(&first, &last));
    EXPECT_FALSE(c_algo_is_sorted_by(&first, &last, greater));
}

TEST_F(CSortTest, Sort)
{
    int numbers[] = { 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
    SetupVector(numbers, __array_length(numbers));

    c_algo_sort(&first, &last);
    c_algo_for_each(&first, &last, print_value);
    print_newline();
    EXPECT_TRUE(c_algo_is_sorted(&first, &last));
}

TEST_F(CSortTest, SortPerformance)
{
    std::vector<int> v(__PERF_SET_SIZE);
    srandom(static_cast<unsigned int>(time(0)));
    int data = 0;
    for (std::vector<int>::iterator iter = v.begin(); iter != v.end(); ++iter) {
        data = random() % INT32_MAX;
        *iter = data;
        c_vector_push_back(vector, C_REF_T(&data));
    }
    first = c_vector_begin(vector);
    last = c_vector_end(vector);

    __c_measure(std::sort(v.begin(), v.end()));

    // in c_algo_sort, intro sort takes about 80ms for 100000 integers
    // and final insertion sort takes about 30ms for 100000 integers when threshold is 16
    // after changing it to 512, total performance improved to 70ms
    __c_measure(c_algo_sort(&first, &last));

    EXPECT_TRUE(c_algo_is_sorted(&first, &last));
}

TEST_F(CSortTest, PartialSort)
{
    int numbers[] = { 5, 7, 4, 2, 8, 6, 1, 9, 0, 3 };
    SetupVector(numbers, __array_length(numbers));
    c_iterator_t* middle = 0;
    __c_iter_copy_and_move(&middle, C_ITER_T(&first), 3);

    c_algo_partial_sort(&first, middle, &last);
    c_algo_for_each(&first, &last, print_value);
    print_newline();
    EXPECT_TRUE(c_algo_is_sorted(&first, middle));

    __c_free(middle);
}

TEST_F(CSortTest, PartialSortCopy)
{
    int v0_numbers[] = { 4, 2, 5, 1, 3 };
    int v1_numbers[] = { 10, 11, 12 };
    int v2_numbers[] = { 10, 11, 12, 13, 14, 15, 16 };

    c_vector_t* v0 = c_vector_create_from_array(c_get_int_type_info(), v0_numbers, __array_length(v0_numbers));
    c_vector_t* v1 = c_vector_create_from_array(c_get_int_type_info(), v1_numbers, __array_length(v1_numbers));
    c_vector_t* v2 = c_vector_create_from_array(c_get_int_type_info(), v2_numbers, __array_length(v2_numbers));

    c_vector_iterator_t v0_first = c_vector_begin(v0);
    c_vector_iterator_t v0_last = c_vector_end(v0);
    c_vector_iterator_t v1_first = c_vector_begin(v1);
    c_vector_iterator_t v1_last = c_vector_end(v1);
    c_vector_iterator_t v2_first = c_vector_begin(v2);
    c_vector_iterator_t v2_last = c_vector_end(v2);
    c_vector_iterator_t* v_output = 0;

    int e1_numbers[] = { 1, 2, 3 };
    EXPECT_EQ(std::min(__array_length(v0_numbers), __array_length(v1_numbers)),
              c_algo_partial_sort_copy(&v0_first, &v0_last, &v1_first, &v1_last, &v_output));
    EXPECT_TRUE(C_ITER_EQ(&v1_last, v_output));
    __array_foreach(e1_numbers, i) {
        EXPECT_EQ(e1_numbers[i], C_DEREF_INT(C_ITER_DEREF(&v1_first)));
        C_ITER_INC(&v1_first);
    }

    int e2_numbers[] = { 1, 2, 3, 4, 5, 15, 16 };
    EXPECT_EQ(std::min(__array_length(v0_numbers), __array_length(v2_numbers)),
              c_algo_partial_sort_copy(&v0_first, &v0_last, &v2_first, &v2_last, &v_output));
    EXPECT_EQ(15, C_DEREF_INT(C_ITER_DEREF(v_output)));
    __array_foreach(e2_numbers, i) {
        EXPECT_EQ(e2_numbers[i], C_DEREF_INT(C_ITER_DEREF(&v2_first)));
        C_ITER_INC(&v2_first);
    }

    __c_free(v_output);
    c_vector_destroy(v0);
    c_vector_destroy(v1);
    c_vector_destroy(v2);
}

} // namespace
} // namespace c_container
