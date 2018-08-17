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
#include <set>
#include "c_test_util.hpp"
#include "c_internal.h"
#include "c_list.h"
#include "c_tree.h"

namespace c_container {
namespace {

const int unique_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int unique_length = __array_length(unique_data);

const int equal_data[] = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 };
const int equal_length = __array_length(equal_data);

#pragma GCC diagnostic ignored "-Weffc++"
class CTreeTest : public ::testing::Test
{
public:
    CTreeTest() : __unique_tree(0), __equal_tree(0) {}
    ~CTreeTest() { TearDown(); }

    void SetupAllTrees(const int *datas, int length)
    {
        SetupUniqueTree(datas, length);
        SetupEqualTree(datas, length);
    }

    void SetupUniqueTree(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_tree_insert_unique_value(__unique_tree, C_REF_T(&datas[i]));

        ExpectNotEmpty(__unique_tree);
        __unique_first = c_tree_begin(__unique_tree);
        __unique_last = c_tree_end(__unique_tree);
    }

    void SetupEqualTree(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_tree_insert_equal_value(__equal_tree, C_REF_T(&datas[i]));

        ExpectNotEmpty(__equal_tree);
        __equal_first = c_tree_begin(__equal_tree);
        __equal_last = c_tree_end(__equal_tree);
    }

    void ExpectEqualToArray(c_tree_t* tree, const int* datas, int length)
    {
        EXPECT_EQ(length, c_tree_size(tree));
        c_tree_iterator_t first = c_tree_begin(tree);
        c_ref_t data = 0;
        for (int i = 0; i < length; ++i) {
            data = C_ITER_DEREF(&first);
            EXPECT_EQ(datas[i], C_DEREF_INT(data));
            C_ITER_INC(&first);
        }
    }

    void ExpectEmpty(c_tree_t* tree)
    {
        c_tree_iterator_t first = c_tree_begin(tree);
        c_tree_iterator_t last = c_tree_end(tree);
        EXPECT_TRUE(c_tree_empty(tree));
        EXPECT_EQ(0, c_tree_size(tree));
        EXPECT_TRUE(C_ITER_EQ(&first, &last));
    }

    void ExpectNotEmpty(c_tree_t* tree)
    {
        c_tree_iterator_t first = c_tree_begin(tree);
        c_tree_iterator_t last = c_tree_end(tree);
        EXPECT_FALSE(c_tree_empty(tree));
        EXPECT_NE(0, c_tree_size(tree));
        EXPECT_TRUE(C_ITER_NE(&first, &last));
    }

    void SetUp()
    {
        __unique_tree = C_TREE_INT;
        __unique_first = c_tree_begin(__unique_tree);
        __unique_last = c_tree_end(__unique_tree);
        ExpectEmpty(__unique_tree);

        __equal_tree = C_TREE_INT;
        __equal_first = c_tree_begin(__equal_tree);
        __equal_last = c_tree_end(__equal_tree);
        ExpectEmpty(__equal_tree);
    }

    void TearDown()
    {
        c_tree_destroy(__unique_tree);
        __unique_tree = 0;
        EXPECT_TRUE(c_tree_empty(__unique_tree));
        EXPECT_EQ(0, c_tree_size(__unique_tree));

        c_tree_destroy(__equal_tree);
        __equal_tree = 0;
        EXPECT_TRUE(c_tree_empty(__equal_tree));
        EXPECT_EQ(0, c_tree_size(__equal_tree));
    }

protected:
    c_tree_t* __unique_tree;
    c_tree_t* __equal_tree;
    c_tree_iterator_t __unique_first;
    c_tree_iterator_t __unique_last;
    c_tree_iterator_t __equal_first;
    c_tree_iterator_t __equal_last;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CTreeTest, Clear)
{
    // clear a non-empty tree
    SetupUniqueTree(unique_data, unique_length);
    c_tree_clear(__unique_tree);
    ExpectEmpty(__unique_tree);

    // clear an empty tree
    c_tree_clear(__unique_tree);
    ExpectEmpty(__unique_tree);
}

TEST_F(CTreeTest, BeginEnd)
{
    // iterate a non-empty tree
    SetupUniqueTree(unique_data, unique_length);
    c_tree_iterator_t rfirst = c_tree_rbegin(__unique_tree);
    c_tree_iterator_t rlast = c_tree_rend(__unique_tree);
    C_ITER_DEC(&rlast);
    while (C_ITER_NE(&__unique_first, &__unique_last)) {
        EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&__unique_first)), C_DEREF_INT(C_ITER_DEREF(&rlast)));
        C_ITER_INC(&__unique_first);
        C_ITER_DEC(&rlast);
    }

    __unique_first = c_tree_begin(__unique_tree);
    __unique_last = c_tree_end(__unique_tree);
    rfirst = c_tree_rbegin(__unique_tree);
    rlast = c_tree_rend(__unique_tree);
    C_ITER_DEC(&__unique_last);
    while (C_ITER_NE(&rfirst, &rlast)) {
        EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&rfirst)), C_DEREF_INT(C_ITER_DEREF(&__unique_last)));
        C_ITER_INC(&rfirst);
        C_ITER_DEC(&__unique_last);
    }

    // iterate an empty tree
    c_tree_clear(__unique_tree);
    __unique_first = c_tree_begin(__unique_tree);
    __unique_last = c_tree_end(__unique_tree);
    rfirst = c_tree_rbegin(__unique_tree);
    rlast = c_tree_rend(__unique_tree);
    EXPECT_TRUE(C_ITER_EQ(&__unique_first, &__unique_last));
    EXPECT_TRUE(C_ITER_EQ(&rfirst, &rlast));
}

TEST_F(CTreeTest, InsertValue)
{
    __array_foreach(unique_data, i) {
        c_tree_iterator_t inserted = c_tree_insert_unique_value(__unique_tree, C_REF_T(&unique_data[i]));
        EXPECT_EQ(unique_data[i], C_DEREF_INT(C_ITER_DEREF(&inserted)));
        inserted = c_tree_insert_equal_value(__equal_tree, C_REF_T(&unique_data[i]));
        EXPECT_EQ(unique_data[i], C_DEREF_INT(C_ITER_DEREF(&inserted)));
    }

    ExpectEqualToArray(__unique_tree, unique_data, unique_length);
    ExpectEqualToArray(__equal_tree, unique_data, unique_length);

    c_tree_clear(__unique_tree);
    c_tree_clear(__equal_tree);

    __array_foreach(equal_data, i) {
        c_tree_iterator_t inserted = c_tree_insert_unique_value(__unique_tree, C_REF_T(&equal_data[i]));
        EXPECT_EQ(equal_data[i], C_DEREF_INT(C_ITER_DEREF(&inserted)));
        inserted = c_tree_insert_equal_value(__equal_tree, C_REF_T(&equal_data[i]));
        EXPECT_EQ(equal_data[i], C_DEREF_INT(C_ITER_DEREF(&inserted)));
    }

    ExpectEqualToArray(__unique_tree, unique_data, unique_length);
    ExpectEqualToArray(__equal_tree, equal_data, equal_length);
}

TEST_F(CTreeTest, InsertPos)
{
    __array_foreach(equal_data, i) {
        c_tree_iterator_t inserted = c_tree_insert_unique(__unique_tree, __unique_last, C_REF_T(&equal_data[i]));
        EXPECT_EQ(equal_data[i], C_DEREF_INT(C_ITER_DEREF(&inserted)));
        inserted = c_tree_insert_equal(__equal_tree, __equal_last, C_REF_T(&equal_data[i]));
        EXPECT_EQ(equal_data[i], C_DEREF_INT(C_ITER_DEREF(&inserted)));
    }
    ExpectEqualToArray(__unique_tree, unique_data, unique_length);
    ExpectEqualToArray(__equal_tree, equal_data, equal_length);
}

TEST_F(CTreeTest, InsertValueRange)
{
    c_list_t* unique_list = c_list_create_from(c_get_int_type_info(), C_REF_T(unique_data), unique_length);
    c_list_t* equal_list = c_list_create_from(c_get_int_type_info(), C_REF_T(equal_data), equal_length);
    c_list_iterator_t u_first = c_list_begin(unique_list);
    c_list_iterator_t u_last = c_list_end(unique_list);
    c_list_iterator_t e_first = c_list_begin(equal_list);
    c_list_iterator_t e_last = c_list_end(equal_list);

    c_tree_insert_unique_range(__unique_tree, C_ITER_T(&u_first), C_ITER_T(&u_last));
    c_tree_insert_equal_range(__equal_tree, C_ITER_T(&u_first), C_ITER_T(&u_last));
    ExpectEqualToArray(__unique_tree, unique_data, unique_length);
    ExpectEqualToArray(__equal_tree, unique_data, unique_length);

    c_tree_clear(__unique_tree);
    c_tree_clear(__equal_tree);

    c_tree_insert_unique_range(__unique_tree, C_ITER_T(&e_first), C_ITER_T(&e_last));
    c_tree_insert_equal_range(__equal_tree, C_ITER_T(&e_first), C_ITER_T(&e_last));
    ExpectEqualToArray(__unique_tree, unique_data, unique_length);
    ExpectEqualToArray(__equal_tree, equal_data, equal_length);

    c_list_destroy(unique_list);
    c_list_destroy(equal_list);
}

TEST_F(CTreeTest, InsertValueFrom)
{
    c_tree_insert_unique_from(__unique_tree, C_REF_T(unique_data), C_REF_T(unique_data + unique_length));
    c_tree_insert_equal_from(__equal_tree, C_REF_T(unique_data), C_REF_T(unique_data + unique_length));
    ExpectEqualToArray(__unique_tree, unique_data, unique_length);
    ExpectEqualToArray(__equal_tree, unique_data, unique_length);

    c_tree_clear(__unique_tree);
    c_tree_clear(__equal_tree);

    c_tree_insert_unique_from(__unique_tree, C_REF_T(equal_data), C_REF_T(equal_data + equal_length));
    c_tree_insert_equal_from(__equal_tree, C_REF_T(equal_data), C_REF_T(equal_data + equal_length));
    ExpectEqualToArray(__unique_tree, unique_data, unique_length);
    ExpectEqualToArray(__equal_tree, equal_data, equal_length);
}

TEST_F(CTreeTest, Erase)
{
    SetupAllTrees(equal_data, equal_length);

    int index = 0;
    c_tree_iterator_t next = __unique_first;
    while (!c_tree_empty(__unique_tree)) {
        ++index;
        next = c_tree_erase(__unique_tree, next);
        ExpectEqualToArray(__unique_tree, &(unique_data[index]), unique_length - index);
        if (C_ITER_NE(&next, &__unique_last)) {
            EXPECT_EQ(unique_data[index], C_DEREF_INT(C_ITER_DEREF(&next)));
        }
    }
    ExpectEmpty(__unique_tree);

    index = 0;
    next = __equal_first;
    while (!c_tree_empty(__equal_tree)) {
        ++index;
        next = c_tree_erase(__equal_tree, next);
        ExpectEqualToArray(__equal_tree, &(equal_data[index]), equal_length - index);
        if (C_ITER_NE(&next, &__equal_last)) {
            EXPECT_EQ(equal_data[index], C_DEREF_INT(C_ITER_DEREF(&next)));
        }
    }
    ExpectEmpty(__equal_tree);
}

TEST_F(CTreeTest, EraseKey)
{
    SetupAllTrees(equal_data, equal_length);

    int not_exist = -1;
    EXPECT_EQ(0, c_tree_erase_key(__unique_tree, C_REF_T(&not_exist)));
    EXPECT_EQ(0, c_tree_erase_key(__equal_tree, C_REF_T(&not_exist)));

    __array_foreach(unique_data, i) {
        size_t n = c_tree_erase_key(__unique_tree, C_REF_T(&unique_data[i]));
        EXPECT_EQ(1, n);
        ExpectEqualToArray(__unique_tree, &(unique_data[i + 1]), unique_length - i - 1);
    }
    ExpectEmpty(__unique_tree);

    __array_foreach(unique_data, i) {
        size_t n = c_tree_erase_key(__equal_tree, C_REF_T(&unique_data[i]));
        EXPECT_EQ(2, n);
        ExpectEqualToArray(__equal_tree, &(equal_data[2 * (i + 1)]), equal_length - 2 * (i + 1));
    }
    ExpectEmpty(__equal_tree);
}

TEST_F(CTreeTest, EraseRange)
{
    SetupAllTrees(equal_data, equal_length);

    c_tree_erase_range(__unique_tree, __unique_first, __unique_last);
    ExpectEmpty(__unique_tree);

    c_tree_erase_range(__equal_tree, __equal_first, __equal_last);
    ExpectEmpty(__equal_tree);
}

TEST_F(CTreeTest, EraseFrom)
{
    SetupAllTrees(equal_data, equal_length);

    c_tree_erase_from(__unique_tree, C_REF_T(unique_data), C_REF_T(unique_data + unique_length));
    ExpectEmpty(__unique_tree);

    c_tree_erase_from(__equal_tree, C_REF_T(unique_data), C_REF_T(unique_data + unique_length));
    ExpectEmpty(__equal_tree);
}

TEST_F(CTreeTest, Swap)
{
    SetupAllTrees(equal_data, equal_length);

    ExpectEqualToArray(__unique_tree, unique_data, unique_length);
    ExpectEqualToArray(__equal_tree, equal_data, equal_length);

    c_tree_swap(__unique_tree, __equal_tree);

    ExpectEqualToArray(__unique_tree, equal_data, equal_length);
    ExpectEqualToArray(__equal_tree, unique_data, unique_length);

    c_tree_t* empty = C_TREE_INT;
    ExpectEmpty(empty);

    c_tree_swap(empty, __unique_tree);
    ExpectEmpty(__unique_tree);
    ExpectEqualToArray(empty, equal_data, equal_length);

    c_tree_swap(empty, __equal_tree);
    ExpectEqualToArray(__equal_tree, equal_data, equal_length);
    ExpectEqualToArray(empty, unique_data, unique_length);

    c_tree_destroy(empty);
}

TEST_F(CTreeTest, Find)
{
    SetupAllTrees(equal_data, equal_length);

    __array_foreach(unique_data, i) {
        c_tree_iterator_t u_found = c_tree_find(__unique_tree, C_REF_T(&unique_data[i]));
        EXPECT_TRUE(C_ITER_NE(&u_found, &__unique_last));
        EXPECT_EQ(unique_data[i], C_DEREF_INT(C_ITER_DEREF(&u_found)));

        c_tree_iterator_t e_found = c_tree_find(__equal_tree, C_REF_T(&unique_data[i]));
        EXPECT_TRUE(C_ITER_NE(&e_found, &__equal_last));
        EXPECT_EQ(unique_data[i], C_DEREF_INT(C_ITER_DEREF(&e_found)));
    }

    int not_exist = -1;
    c_tree_iterator_t u_found = c_tree_find(__unique_tree, C_REF_T(&not_exist));
    EXPECT_TRUE(C_ITER_EQ(&u_found, &__unique_last));

    c_tree_iterator_t e_found = c_tree_find(__equal_tree, C_REF_T(&not_exist));
    EXPECT_TRUE(C_ITER_EQ(&e_found, &__equal_last));
}

TEST_F(CTreeTest, Count)
{
    SetupAllTrees(equal_data, equal_length);

    __array_foreach(unique_data, i) {
        size_t u_count = c_tree_count(__unique_tree, C_REF_T(&unique_data[i]));
        EXPECT_EQ(1, u_count);

        size_t e_count = c_tree_count(__equal_tree, C_REF_T(&unique_data[i]));
        EXPECT_EQ(2, e_count);
    }

    int not_exist = -1;
    size_t u_count = c_tree_count(__unique_tree, C_REF_T(&not_exist));
    EXPECT_EQ(0, u_count);

    size_t e_count = c_tree_count(__equal_tree, C_REF_T(&not_exist));
    EXPECT_EQ(0, e_count);
}

TEST_F(CTreeTest, Bound)
{
    SetupAllTrees(equal_data, equal_length);

    __array_foreach(unique_data, i) {
        c_tree_iterator_t lower = c_tree_lower_bound(__unique_tree, C_REF_T(&unique_data[i]));
        c_tree_iterator_t upper = c_tree_upper_bound(__unique_tree, C_REF_T(&unique_data[i]));
        EXPECT_EQ(unique_data[i], C_DEREF_INT(C_ITER_DEREF(&lower)));
        if (C_ITER_NE(&__unique_last, &upper)) {
            EXPECT_TRUE(unique_data[i] < C_DEREF_INT(C_ITER_DEREF(&upper)));
        }
    }

    __array_foreach(unique_data, i) {
        c_tree_iterator_t lower = c_tree_lower_bound(__equal_tree, C_REF_T(&unique_data[i]));
        c_tree_iterator_t upper = c_tree_upper_bound(__equal_tree, C_REF_T(&unique_data[i]));
        EXPECT_EQ(unique_data[i], C_DEREF_INT(C_ITER_DEREF(&lower)));
        if (C_ITER_NE(&__equal_last, &upper)) {
            EXPECT_TRUE(unique_data[i] < C_DEREF_INT(C_ITER_DEREF(&upper)));
        }
    }
}

TEST_F(CTreeTest, RBVerify)
{
    __array_foreach(equal_data, i) {
        c_tree_insert_unique_value(__unique_tree, C_REF_T(&equal_data[i]));
        EXPECT_TRUE(c_tree_rb_verify(__unique_tree));

        c_tree_insert_equal_value(__equal_tree, C_REF_T(&equal_data[i]));
        EXPECT_TRUE(c_tree_rb_verify(__equal_tree));
    }

    __array_foreach(unique_data, i) {
        c_tree_erase_key(__unique_tree, C_REF_T(&unique_data[i]));
        EXPECT_TRUE(c_tree_rb_verify(__unique_tree));

        c_tree_erase_key(__equal_tree, C_REF_T(&unique_data[i]));
        EXPECT_TRUE(c_tree_rb_verify(__equal_tree));
    }
}

} // namespace
} // namespace c_container
