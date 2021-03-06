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
#include <list>
#include "c_internal.h"
#include "c_list.h"
#include "c_algorithm.h"
#include "c_test_util.hpp"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

bool c_int_greater(const c_ref_t lhs, const c_ref_t rhs)
{
    return C_DEREF_INT(lhs) > C_DEREF_INT(rhs);
}

bool greater_than_five(const c_ref_t data)
{
    return C_DEREF_INT(data) > 5;
}

bool abs_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return abs(C_DEREF_INT(lhs)) == abs(C_DEREF_INT(rhs));
}

#pragma GCC diagnostic ignored "-Weffc++"
class CListTest : public ::testing::Test
{
public:
    CListTest() : list(0) {}
    ~CListTest() { c_list_destroy(list); }

    void SetupPerformance(void)
    {
        std_list.clear();
        std_list.resize(__PERF_SET_SIZE);
        srandom(static_cast<unsigned int>(time(0)));
        int data = 0;
        for (auto iter = std_list.begin(); iter != std_list.end(); ++iter) {
            data = random() % INT32_MAX;
            *iter = data;
            c_list_push_back(list, C_REF_T(&data));
        }
    }

    void SetupList(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_list_push_back(list, C_REF_T(&datas[i]));

        ExpectNotEmpty();
        EXPECT_EQ(length, c_list_size(list));
    }

    void ExpectEqualToArray(const int* datas, int length)
    {
        EXPECT_EQ(length, c_list_size(list));
        c_list_iterator_t first = c_list_begin(list);
        c_ref_t data = 0;
        for (int i = 0; i < length; ++i) {
            data = C_ITER_DEREF(&first);
            EXPECT_EQ(datas[i], C_DEREF_INT(data));
            C_ITER_INC(&first);
        }
    }

    void ExpectEmpty()
    {
        c_list_iterator_t first = c_list_begin(list);
        c_list_iterator_t last = c_list_end(list);
        EXPECT_TRUE(c_list_empty(list));
        EXPECT_EQ(0, c_list_size(list));
        EXPECT_TRUE(C_ITER_EQ(&first, &last));
    }

    void ExpectNotEmpty()
    {
        c_list_iterator_t first = c_list_begin(list);
        c_list_iterator_t last = c_list_end(list);
        EXPECT_FALSE(c_list_empty(list));
        EXPECT_NE(0, c_list_size(list));
        EXPECT_TRUE(C_ITER_NE(&first, &last));
    }

    void SetUp()
    {
        list = C_LIST_INT;
        ExpectEmpty();
    }

    void TearDown()
    {
        c_list_destroy(list);
        list = 0;
        EXPECT_TRUE(c_list_empty(list));
        EXPECT_EQ(0, c_list_size(list));

        std_list.clear();
    }

protected:
    c_list_t* list;
    std::list<int> std_list;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CListTest, Clear)
{
    // clear a non-empty list
    SetupList(default_data, default_length);
    c_list_clear(list);
    ExpectEmpty();

    // clear an empty list
    c_list_clear(list);
    ExpectEmpty();
}

TEST_F(CListTest, BeginEnd)
{
    // iterate a non-empty list
    SetupList(default_data, default_length);
    c_list_iterator_t first = c_list_begin(list);
    c_list_iterator_t last = c_list_end(list);
    c_list_iterator_t rfirst = c_list_rbegin(list);
    c_list_iterator_t rlast = c_list_rend(list);
    C_ITER_DEC(&rlast);
    while (C_ITER_NE(&first, &last)) {
        EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&first)), C_DEREF_INT(C_ITER_DEREF(&rlast)));
        C_ITER_INC(&first);
        C_ITER_DEC(&rlast);
    }

    first = c_list_begin(list);
    last = c_list_end(list);
    rfirst = c_list_rbegin(list);
    rlast = c_list_rend(list);
    C_ITER_DEC(&last);
    while (C_ITER_NE(&rfirst, &rlast)) {
        EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&rfirst)), C_DEREF_INT(C_ITER_DEREF(&last)));
        C_ITER_INC(&rfirst);
        C_ITER_DEC(&last);
    }

    // iterate an empty list
    c_list_clear(list);
    first = c_list_begin(list);
    last = c_list_end(list);
    rfirst = c_list_rbegin(list);
    rlast = c_list_rend(list);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));
    EXPECT_TRUE(C_ITER_EQ(&rfirst, &rlast));
}

TEST_F(CListTest, BackOperations)
{
    // back operations on a non-empty list
    SetupList(default_data, default_length);

    c_list_iterator_t first, last;
    size_t size = 0;
    c_ref_t data = 0;
    while (!c_list_empty(list)) {
        size = c_list_size(list);
        first = c_list_begin(list);
        last = c_list_end(list);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_list_back(list);
        EXPECT_EQ(size - 1, C_DEREF_INT(data));
        c_list_pop_back(list);
        EXPECT_EQ(size - 1, c_list_size(list));
    }

    // back operations on an empty list
    ExpectEmpty();
    data = c_list_back(list);
    EXPECT_TRUE(data == 0);
    c_list_pop_back(list); // nothing should happen
}

TEST_F(CListTest, FrontOperations)
{
    // front operations on a non-empty list
    SetupList(default_data, default_length);

    c_list_iterator_t first, last;
    size_t size = 0;
    c_ref_t data = 0;
    int i = 0;
    while (!c_list_empty(list)) {
        size = c_list_size(list);
        first = c_list_begin(list);
        last = c_list_end(list);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_list_front(list);
        EXPECT_EQ(i++, C_DEREF_INT(data));
        c_list_pop_front(list);
        EXPECT_EQ(size - 1, c_list_size(list));
    }

    // front operations on an empty list
    ExpectEmpty();
    data = c_list_front(list);
    EXPECT_TRUE(data == 0);
    c_list_pop_front(list); // nothing should happen
}

TEST_F(CListTest, Resize)
{
    SetupList(default_data, default_length);

    int bigger[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0 };
    c_list_resize(list, c_list_size(list) + 3);
    ExpectEqualToArray(bigger, __array_length(bigger));

    int bigger_with_value[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 6, 6, 6 };
    int value = 6;
    c_list_resize_with_value(list, c_list_size(list) + 3, C_REF_T(&value));
    ExpectEqualToArray(bigger_with_value, __array_length(bigger_with_value));

    int smaller[] = { 0, 1, 2, 3, 4, 5 };
    c_list_resize(list, 6);
    ExpectEqualToArray(smaller, __array_length(smaller));

    int even_smaller[] = { 0, 1, 2, 3, 4 };
    c_list_resize(list, 5);
    ExpectEqualToArray(even_smaller, __array_length(even_smaller));
}

TEST_F(CListTest, Swap)
{
    SetupList(default_data, default_length);

    c_list_t* other = C_LIST_INT;

    c_list_swap(list, other);
    ExpectEmpty();
    c_list_swap(list, other);
    ExpectEqualToArray(default_data, default_length);
    EXPECT_TRUE(c_list_empty(other));

    c_list_destroy(other);
}

TEST_F(CListTest, InsertErase)
{
    c_list_iterator_t first, last, iter;
    c_ref_t data = 0;

    for (int i = 0; i < 9; ++i) {
        last = c_list_end(list);
        iter = c_list_insert(list, last, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        EXPECT_EQ(i, C_DEREF_INT(data));
        EXPECT_EQ(i + 1, c_list_size(list));
    }

    ExpectNotEmpty();

    first = c_list_begin(list);
    last = c_list_end(list);
    while (C_ITER_NE(&first, &last)) {
        size_t size = c_list_size(list);
        first = c_list_erase(list, first);
        last = c_list_end(list);
        EXPECT_EQ(size - 1, c_list_size(list));
    }

    first = c_list_erase(list, first);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));

    for (int i = 0; i < 9; ++i) {
        first = c_list_begin(list);
        iter = c_list_insert(list, first, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        first = c_list_begin(list);
        EXPECT_TRUE(C_ITER_EQ(&first, &iter));
        EXPECT_EQ(i, C_DEREF_INT(data));
    }

    first = c_list_begin(list);
    last = c_list_end(list);
    first = c_list_erase_range(list, first, last);
    ExpectEmpty();
}

TEST_F(CListTest, Merge)
{
    c_list_t* other = C_LIST_INT;

    // merge non-empty to non-empty
    int origin[] = { -2, -2, -1, 1, 3, 3, 4, 5, 5, 5, 7, 10, 10, 11 };
    int merged[] = { -2, -2, -1, 0, 1, 1, 2, 3, 3, 3, 4, 4, 5, 5, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11 };

    for (unsigned int i = 0; i < __array_length(origin); ++i)
        c_list_push_back(other, C_REF_T(&origin[i]));

    SetupList(default_data, default_length);
    c_list_sort(list);
    c_list_sort(other);
    c_list_merge(list, other);
    ExpectEqualToArray(merged, __array_length(merged));
    EXPECT_TRUE(c_list_empty(other));

    // merge empty to non-empty
    c_list_merge(list, other);
    ExpectEqualToArray(merged, __array_length(merged));

    // merge two empty lists
    c_list_clear(list);
    c_list_merge(list, other);
    ExpectEmpty();

    // merge non-empty to empty
    __array_foreach(origin, i) {
        c_list_push_back(other, C_REF_T(&origin[i]));
    }
    c_list_merge(list, other);
    ExpectEqualToArray(origin, __array_length(origin));

    c_list_destroy(other);
}

TEST_F(CListTest, MergeBy)
{
    c_list_t* other = C_LIST_INT;

    int origin[] = { -2, -2, -1, 1, 3, 3, 4, 5, 5, 5, 7, 10, 10, 11 };
    int merged[] = { 11, 10, 10, 9, 8, 7, 7, 6, 5, 5, 5, 5, 4, 4, 3, 3, 3, 2, 1, 1, 0, -1, -2, -2 };

    for (unsigned int i = 0; i < __array_length(origin); ++i)
        c_list_push_back(other, C_REF_T(&origin[i]));

    SetupList(default_data, default_length);
    c_list_sort_by(list, c_int_greater);
    c_list_sort_by(other, c_int_greater);
    c_list_merge_by(list, other, c_int_greater);
    ExpectEqualToArray(merged, __array_length(merged));

    c_list_destroy(other);
}

TEST_F(CListTest, Splice)
{
    SetupList(default_data, default_length);
    int origin[] = { 10, 11, 12 };

    c_list_t* other = C_LIST_INT;

    __array_foreach(origin, i) {
        c_list_push_back(other, C_REF_T(&origin[i]));
    }
    c_list_splice(list, c_list_begin(list), other);
    int spliced_begin[] = { 10, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ExpectEqualToArray(spliced_begin, __array_length(spliced_begin));
    EXPECT_TRUE(c_list_empty(other));

    __array_foreach(origin, i) {
        c_list_push_back(other, C_REF_T(&origin[i]));
    }
    c_list_splice(list, c_list_end(list), other);
    int spliced_end[] = { 10, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    ExpectEqualToArray(spliced_end, __array_length(spliced_end));
    EXPECT_TRUE(c_list_empty(other));

    __array_foreach(origin, i) {
        c_list_push_back(other, C_REF_T(&origin[i]));
    }
    c_list_splice_from(list, c_list_begin(list), other, c_list_begin(other));
    int spliced_from[] = { 10, 11, 12, 10, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    ExpectEqualToArray(spliced_from, __array_length(spliced_from));
    EXPECT_TRUE(c_list_empty(other));

    __array_foreach(origin, i) {
        c_list_push_back(other, C_REF_T(&origin[i]));
    }
    c_list_splice_range(list, c_list_begin(list), other, c_list_begin(other), c_list_end(other));
    int spliced_range[] = { 10, 11, 12, 10, 11, 12, 10, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    ExpectEqualToArray(spliced_range, __array_length(spliced_range));
    EXPECT_TRUE(c_list_empty(other));

    c_list_destroy(other);
}

TEST_F(CListTest, Remove)
{
    int origin[] = { 1, 2, 3, 1, 3, 3, 2, 4, 4, 1, 4, 4, 4 };
    int removed_4[] = { 1, 2, 3, 1, 3, 3, 2, 1 };
    int removed_3[] = { 1, 2, 1, 2, 1 };
    int removed_2[] = { 1, 1, 1 };

    SetupList(origin, __array_length(origin));

    int to_be_removed = 4;
    c_list_remove(list, C_REF_T(&to_be_removed));
    ExpectEqualToArray(removed_4, __array_length(removed_4));

    to_be_removed = 3;
    c_list_remove(list, C_REF_T(&to_be_removed));
    ExpectEqualToArray(removed_3, __array_length(removed_3));

    to_be_removed = 2;
    c_list_remove(list, C_REF_T(&to_be_removed));
    ExpectEqualToArray(removed_2, __array_length(removed_2));

    to_be_removed = 1;
    c_list_remove(list, C_REF_T(&to_be_removed));
    ExpectEmpty();
}

TEST_F(CListTest, RemoveIf)
{
    int removed[] = { 0, 1, 2, 3, 4, 5 };

    SetupList(default_data, default_length);
    c_list_remove_if(list, greater_than_five);
    ExpectEqualToArray(removed, __array_length(removed));
}

TEST_F(CListTest, Sort)
{
    int sorted[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int random[] = { 5, 8, 1, 0, 3, 2, 7, 6, 4, 9 };

    SetupList(random, __array_length(random));
    c_list_sort(list);
    ExpectEqualToArray(sorted, __array_length(sorted));
}

TEST_F(CListTest, SortBy)
{
    int sorted[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    int random[] = { 5, 8, 1, 0, 3, 2, 7, 6, 4, 9 };

    SetupList(random, __array_length(random));
    c_list_sort_by(list, c_int_greater);
    ExpectEqualToArray(sorted, __array_length(sorted));
}

TEST_F(CListTest, SortPerformance)
{
    SetupPerformance();

    __c_measure(std_list.sort());
    __c_measure(c_list_sort(list));

    c_list_iterator_t first = c_list_begin(list);
    c_list_iterator_t last = c_list_end(list);
    EXPECT_TRUE(c_algo_is_sorted(&first, &last));
}

TEST_F(CListTest, Reverse)
{
    int reversed[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

    SetupList(default_data, default_length);
    c_list_reverse(list);
    ExpectEqualToArray(reversed, __array_length(reversed));
}

TEST_F(CListTest, Unique)
{
    int not_unique[] = { 0, 1, 2, 2, 1, 1, 3, 0, 0, 1, 2, 2 };
    int uniqued[] = { 0, 1, 2, 1, 3, 0, 1, 2 };

    SetupList(not_unique, __array_length(not_unique));
    c_list_unique(list);
    ExpectEqualToArray(uniqued, __array_length(uniqued));
}

TEST_F(CListTest, UniqueIf)
{
    int not_unique[] = { 0, 1, -2, 2, 1, -1, 3, 0, 0, 1, -2, 2, 2, -2 };
    int uniqued[] = { 0, 1, -2, 1, 3, 0, 1, -2 };

    SetupList(not_unique, __array_length(not_unique));
    c_list_unique_if(list, abs_equal);
    ExpectEqualToArray(uniqued, __array_length(uniqued));
}

} // namespace
} // namespace c_container
