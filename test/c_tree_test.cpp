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
#include "c_internal.h"
#include "c_tree.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

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
        ExpectEmpty(__unique_tree);

        __equal_tree = C_TREE_INT;
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
    SetupUniqueTree(default_data, default_length);
    c_tree_clear(__unique_tree);
    ExpectEmpty(__unique_tree);

    // clear an empty tree
    c_tree_clear(__unique_tree);
    ExpectEmpty(__unique_tree);
}

TEST_F(CTreeTest, BeginEnd)
{
    // iterate a non-empty tree
    SetupUniqueTree(default_data, default_length);
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

} // namespace
} // namespace c_container
