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
    CPartitionTest() : list(0) {}
    ~CPartitionTest() { TearDown(); }

    void SetupList(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_list_push_back(list, C_REF_T(&datas[i]));
        first = c_list_begin(list);
        last = c_list_end(list);
    }

    void SetUp()
    {
        list = C_LIST_INT;
        first = c_list_begin(list);
        last = c_list_end(list);
        output = 0;
    }

    void TearDown()
    {
        __c_free(output);
        c_list_destroy(list);
        list = 0;
        output = 0;
    }

protected:
    c_list_t* list;
    c_list_iterator_t first;
    c_list_iterator_t last;
    c_list_iterator_t* output;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CPartitionTest, IsPartitioned)
{
    SetupList(default_data, default_length);
    EXPECT_FALSE(c_algo_is_partitioned(&first, &last, is_even));

    c_list_clear(list);
    int numbers[] = { 0, 2, 4, 6, 8, 3, 5, 9, 1, 7 };
    SetupList(numbers, __array_length(numbers));
    first = c_list_begin(list);
    last = c_list_end(list);
    EXPECT_TRUE(c_algo_is_partitioned(&first, &last, is_even));
}

TEST_F(CPartitionTest, Partition)
{
    SetupList(default_data, default_length);

    EXPECT_FALSE(c_algo_is_partitioned(&first, &last, is_even));
    c_algo_partition(&first, &last, &output, is_even);
    EXPECT_TRUE(c_algo_is_partitioned(&first, &last, is_even));
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

    EXPECT_EQ(default_length, c_algo_partition_copy(&first, &last, &e_first, &o_first, &e_end, &o_end, is_even));
    EXPECT_TRUE(c_algo_equal(&e_first, e_end, &ee_first));
    EXPECT_TRUE(c_algo_equal(&o_first, o_end, &eo_first));

    c_list_destroy(expected_even);
    c_list_destroy(expected_odd);
    c_list_destroy(even);
    c_list_destroy(odd);
}

TEST_F(CPartitionTest, PartitionPoint)
{
    SetupList(default_data, default_length);
    c_algo_partition(&first, &last, &output, is_even);
    c_algo_partition_point(&first, &last, &output, is_even);
    EXPECT_TRUE(c_algo_all_of(&first, output, is_even));
    EXPECT_TRUE(c_algo_none_of(output, &last, is_even));
}

} // namespace
} // namespace c_container
