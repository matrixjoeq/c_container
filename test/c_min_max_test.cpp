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

#pragma GCC diagnostic ignored "-Weffc++"
class CMinMaxTest : public ::testing::Test
{
public:
    CMinMaxTest() : list(0) {}
    ~CMinMaxTest() { c_list_destroy(list); }

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
    }

    void TearDown()
    {
        c_list_destroy(list);
        list = 0;
    }

protected:
    c_list_t* list;
    c_list_iterator_t first;
    c_list_iterator_t last;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CMinMaxTest, MinMax)
{
    SetupList(default_data, default_length);

    c_list_iterator_t* min = 0;
    c_list_iterator_t* max = 0;
    c_algo_minmax_element(&first, &last, &min, &max);
    EXPECT_EQ(0, C_DEREF_INT(C_ITER_DEREF(min)));
    EXPECT_EQ(9, C_DEREF_INT(C_ITER_DEREF(max)));

    __c_free(min);
    __c_free(max);
}

TEST_F(CMinMaxTest, LexicographicalCompare)
{
    SetupList(default_data, default_length);

    int smaller[] = { 0, 1, 2, 3 };
    int bigger[] = { 1 };
    c_list_t* l_smaller = c_list_create_from(c_get_int_type_info(), C_REF_T(smaller), __array_length(smaller));
    c_list_t* l_bigger = c_list_create_from(c_get_int_type_info(), C_REF_T(bigger), __array_length(bigger));
    c_list_t* l_equal = c_list_create_from(c_get_int_type_info(), C_REF_T(default_data), default_length);
    c_list_iterator_t s_first = c_list_begin(l_smaller);
    c_list_iterator_t s_last = c_list_end(l_smaller);
    c_list_iterator_t b_first = c_list_begin(l_bigger);
    c_list_iterator_t b_last = c_list_end(l_bigger);
    c_list_iterator_t e_first = c_list_begin(l_equal);
    c_list_iterator_t e_last = c_list_end(l_equal);

    EXPECT_TRUE(c_algo_lexicographical_compare(&s_first, &s_last, &first, &last));
    EXPECT_FALSE(c_algo_lexicographical_compare(&b_first, &b_last, &first, &last));
    EXPECT_FALSE(c_algo_lexicographical_compare(&e_first, &e_last, &first, &last));

    c_list_destroy(l_equal);
    c_list_destroy(l_smaller);
    c_list_destroy(l_bigger);
}

} // namespace
} // namespace c_container
