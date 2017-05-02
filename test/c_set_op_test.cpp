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
#include "c_set.h"
#include "c_list.h"
#include "c_algorithm.h"

namespace c_container {
namespace {

const int unique_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int unique_length = __array_length(unique_data);

const int equal_data[] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };
const int equal_length = __array_length(equal_data);

#pragma GCC diagnostic ignored "-Weffc++"
class CSetOpTest : public ::testing::Test
{
public:
    CSetOpTest() : set(0), m_set(0) {}
    ~CSetOpTest() { TearDown(); }

    void SetupAll(const int *datas, int length)
    {
        SetupSet(datas, length);
        SetupMultiset(datas, length);
    }

    void SetupSet(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_set_insert_value(set, C_REF_T(&datas[i]));
        s_first = c_set_begin(set);
        s_last = c_set_end(set);
    }

    void SetupMultiset(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_multiset_insert_value(m_set, C_REF_T(&datas[i]));
        ms_first = c_multiset_begin(m_set);
        ms_last = c_multiset_end(m_set);
    }

    void SetUp()
    {
        set = C_SET_INT;
        s_first = c_set_begin(set);
        s_last = c_set_end(set);
        s_output = 0;

        m_set = C_MULTISET_INT;
        ms_first = c_multiset_begin(m_set);
        ms_last = c_multiset_end(m_set);
        ms_output = 0;
    }

    void TearDown()
    {
        __c_free(s_output);
        c_set_destroy(set);
        set = 0;
        s_output = 0;

        __c_free(ms_output);
        c_multiset_destroy(m_set);
        m_set = 0;
        ms_output = 0;
    }

    void ExpectEqualToArray(c_set_t* s, const int* datas, int length)
    {
        EXPECT_EQ(length, c_set_size(s));
        c_set_iterator_t first = c_set_begin(s);
        c_ref_t data = 0;
        for (int i = 0; i < length; ++i) {
            data = C_ITER_DEREF(&first);
            EXPECT_EQ(datas[i], C_DEREF_INT(data));
            C_ITER_INC(&first);
        }
    }

protected:
    c_set_t* set;
    c_set_t* m_set;
    c_set_iterator_t s_first;
    c_set_iterator_t s_last;
    c_set_iterator_t ms_first;
    c_set_iterator_t ms_last;
    c_set_iterator_t* s_output;
    c_set_iterator_t* ms_output;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CSetOpTest, Merge)
{
    SetupAll(equal_data, equal_length);

    c_list_t* list = C_LIST_INT;
    c_list_resize(list, c_set_size(set) + c_multiset_size(m_set));
    c_list_iterator_t l_first = c_list_begin(list);
    c_list_iterator_t l_last = c_list_end(list);
    while (C_ITER_NE(&l_first, &l_last)) {
        EXPECT_EQ(0, C_DEREF_INT(C_ITER_DEREF(&l_first)));
        C_ITER_INC(&l_first);
    }

    l_first = c_list_begin(list);
    c_list_iterator_t* l_merged = 0;
    size_t merged = c_algo_merge(&s_first, &s_last, &ms_first, &ms_last, &l_first, &l_merged);
    EXPECT_EQ(c_set_size(set) + c_multiset_size(m_set), merged);

    int numbers[] = { 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9 };
    int index = 0;
    while (C_ITER_NE(&l_first, l_merged)) {
        EXPECT_EQ(numbers[index++], C_DEREF_INT(C_ITER_DEREF(&l_first)));
        C_ITER_INC(&l_first);
    }

    __c_free(l_merged);
    c_list_destroy(list);
}

TEST_F(CSetOpTest, Includes)
{
    SetupAll(equal_data, equal_length);

    int numbers1[] = { 0, 2, 4, 6, 8 };
    c_list_t* list = c_list_create_from(c_get_int_type_info(), numbers1, __array_length(numbers1));
    c_list_iterator_t l_first = c_list_begin(list);
    c_list_iterator_t l_last = c_list_end(list);

    EXPECT_TRUE(c_algo_includes(&s_first, &s_last, &l_first, &l_last));
    EXPECT_TRUE(c_algo_includes(&ms_first, &ms_last, &l_first, &l_last));

    int numbers2[] = { 1, 3, 5, 7, 9, 11 };
    c_list_clear(list);
    __array_foreach(numbers2, i) {
        c_list_push_back(list, C_REF_T(&numbers2[i]));
    }
    l_first = c_list_begin(list);
    l_last = c_list_end(list);
    EXPECT_FALSE(c_algo_includes(&s_first, &s_last, &l_first, &l_last));
    EXPECT_FALSE(c_algo_includes(&ms_first, &ms_last, &l_first, &l_last));

    int numbers3[] = { 0, 0, 1, 3, 5, 7, 9 };
    c_list_clear(list);
    __array_foreach(numbers3, i) {
        c_list_push_back(list, C_REF_T(&numbers3[i]));
    }
    l_first = c_list_begin(list);
    l_last = c_list_end(list);
    EXPECT_FALSE(c_algo_includes(&s_first, &s_last, &l_first, &l_last));
    EXPECT_TRUE(c_algo_includes(&ms_first, &ms_last, &l_first, &l_last));

    c_list_destroy(list);
}

TEST_F(CSetOpTest, SetDifference)
{
    SetupAll(equal_data, equal_length);
    c_list_t* list = C_LIST_INT;
    c_list_resize(list, c_multiset_size(m_set) - c_set_size(set));
    c_list_iterator_t l_first = c_list_begin(list);
    c_list_iterator_t* l_copied = 0;
    EXPECT_EQ(c_multiset_size(m_set) - c_set_size(set),
              c_algo_set_difference(&ms_first, &ms_last, &s_first, &s_last, &l_first, &l_copied));
    __array_foreach(unique_data, i) {
        EXPECT_EQ(unique_data[i], C_DEREF_INT(C_ITER_DEREF(&l_first)));
        C_ITER_INC(&l_first);
    }

    l_first = c_list_begin(list);
    EXPECT_EQ(0, c_algo_set_difference(&s_first, &s_last, &ms_first, &ms_last, &l_first, &l_copied));

    __c_free(l_copied);
    c_list_destroy(list);
}

TEST_F(CSetOpTest, SetIntersection)
{
    SetupAll(equal_data, equal_length);
    c_list_t* list = C_LIST_INT;
    c_list_resize(list, c_multiset_size(m_set) - c_set_size(set));
    c_list_iterator_t l_first = c_list_begin(list);
    c_list_iterator_t* l_copied = 0;
    EXPECT_EQ(c_multiset_size(m_set) - c_set_size(set),
              c_algo_set_intersection(&s_first, &s_last, &ms_first, &ms_last, &l_first, &l_copied));
    __array_foreach(unique_data, i) {
        EXPECT_EQ(unique_data[i], C_DEREF_INT(C_ITER_DEREF(&l_first)));
        C_ITER_INC(&l_first);
    }

    __c_free(l_copied);
    c_list_destroy(list);
}

TEST_F(CSetOpTest, SetSymmetricDifference)
{
    int set_numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int mset_numbers[] = { 5, 5, 7, 7, 7, 9, 9, 10 };
    SetupSet(set_numbers, __array_length(set_numbers));
    SetupMultiset(mset_numbers, __array_length(mset_numbers));

    c_list_t* list = C_LIST_INT;
    c_list_resize(list, 12);
    c_list_iterator_t l_first = c_list_begin(list);
    c_list_iterator_t* l_copied = 0;
    EXPECT_EQ(12, c_algo_set_symmetric_difference(&s_first, &s_last, &ms_first, &ms_last, &l_first, &l_copied));

    int difference[] = { 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 9, 10 };
    __array_foreach(difference, i) {
        EXPECT_EQ(difference[i], C_DEREF_INT(C_ITER_DEREF(&l_first)));
        C_ITER_INC(&l_first);
    }

    __c_free(l_copied);
    c_list_destroy(list);
}

TEST_F(CSetOpTest, SetUnion)
{
    int set_numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    int mset_numbers[] = { 5, 5, 7, 7, 7, 9, 9, 10 };
    SetupSet(set_numbers, __array_length(set_numbers));
    SetupMultiset(mset_numbers, __array_length(mset_numbers));

    c_list_t* list = C_LIST_INT;
    c_list_resize(list, 14);
    c_list_iterator_t l_first = c_list_begin(list);
    c_list_iterator_t* l_copied = 0;
    EXPECT_EQ(14, c_algo_set_union(&s_first, &s_last, &ms_first, &ms_last, &l_first, &l_copied));

    int set_union[] = { 1, 2, 3, 4, 5, 5, 6, 7, 7, 7, 8, 9, 9, 10 };
    __array_foreach(set_union, i) {
        EXPECT_EQ(set_union[i], C_DEREF_INT(C_ITER_DEREF(&l_first)));
        C_ITER_INC(&l_first);
    }

    __c_free(l_copied);
    c_list_destroy(list);
}

} // namespace
} // namespace c_container
