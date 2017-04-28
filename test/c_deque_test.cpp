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
#include "c_algorithm.h"
#include "c_deque.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

void print_value(c_ref_t data)
{
    printf("%d ", C_DEREF_INT(data));
}

#pragma GCC diagnostic ignored "-Weffc++"
class CDequeTest : public ::testing::Test
{
public:
    CDequeTest() : deque(0) {}
    ~CDequeTest() { c_deque_destroy(deque); }

    void SetupDeque(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i) {
            c_deque_iterator_t last = c_deque_end(deque);
            c_deque_insert(deque, last, C_REF_T(&datas[i]));
        }

        ExpectNotEmpty();
        EXPECT_EQ(length, c_deque_size(deque));
    }

    void ExpectEqualToArray(const int* datas, int length)
    {
        EXPECT_EQ(length, c_deque_size(deque));
        c_deque_iterator_t first = c_deque_begin(deque);
        c_ref_t data = 0;
        for (int i = 0; i < length; ++i) {
            data = C_ITER_DEREF(&first);
            EXPECT_EQ(datas[i], C_DEREF_INT(data));
            C_ITER_INC(&first);
        }
    }

    void ExpectEmpty()
    {
        c_deque_iterator_t first = c_deque_begin(deque);
        c_deque_iterator_t last = c_deque_end(deque);
        EXPECT_TRUE(c_deque_empty(deque));
        EXPECT_EQ(0, c_deque_size(deque));
        EXPECT_TRUE(C_ITER_EQ(&first, &last));
    }

    void ExpectNotEmpty()
    {
        c_deque_iterator_t first = c_deque_begin(deque);
        c_deque_iterator_t last = c_deque_end(deque);
        EXPECT_FALSE(c_deque_empty(deque));
        EXPECT_NE(0, c_deque_size(deque));
        EXPECT_TRUE(C_ITER_NE(&first, &last));
    }

    void SetUp()
    {
        deque = C_DEQUE_INT;
        ExpectEmpty();
    }

    void TearDown()
    {
        c_deque_destroy(deque);
        deque = 0;
        EXPECT_TRUE(c_deque_empty(deque));
        EXPECT_EQ(0, c_deque_size(deque));
    }

protected:
    c_deque_t* deque;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CDequeTest, Clear)
{
    // clear a non-empty deque
    SetupDeque(default_data, default_length);
    c_deque_clear(deque);
    ExpectEmpty();

    // clear an empty deque
    c_deque_clear(deque);
    ExpectEmpty();
}

TEST_F(CDequeTest, BeginEnd)
{
    // iterate a non-empty deque
    SetupDeque(default_data, default_length);
    c_deque_iterator_t first = c_deque_begin(deque);
    c_deque_iterator_t last = c_deque_end(deque);
    c_deque_iterator_t rfirst = c_deque_rbegin(deque);
    c_deque_iterator_t rlast = c_deque_rend(deque);
    C_ITER_DEC(&last);
    C_ITER_DEC(&rlast);
    EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&first)), C_DEREF_INT(C_ITER_DEREF(&rlast)));
    EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&rfirst)), C_DEREF_INT(C_ITER_DEREF(&last)));

    // iterate an empty deque
    c_deque_clear(deque);
    first = c_deque_begin(deque);
    last = c_deque_end(deque);
    rfirst = c_deque_rbegin(deque);
    rlast = c_deque_rend(deque);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));
    EXPECT_TRUE(C_ITER_EQ(&rfirst, &rlast));
}

TEST_F(CDequeTest, At)
{
    SetupDeque(default_data, default_length);

    for (int i = 0; i < default_length; ++i) {
        EXPECT_EQ(i, C_DEREF_INT(c_deque_at(deque, i)));
    }
}

TEST_F(CDequeTest, BackOperations)
{
    // back operations on a non-empty deque
    SetupDeque(default_data, default_length);
    c_deque_iterator_t first = c_deque_begin(deque);
    c_deque_iterator_t last = c_deque_end(deque);

    size_t size = 0;
    c_ref_t data = 0;
    while (!c_deque_empty(deque)) {
        size = c_deque_size(deque);
        first = c_deque_begin(deque);
        last = c_deque_end(deque);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_deque_back(deque);
        EXPECT_EQ(size - 1, C_DEREF_INT(data));
        c_algo_for_each(&first, &last, print_value);
        printf("\n");
        c_deque_pop_back(deque);
        EXPECT_EQ(size - 1, c_deque_size(deque));
    }

    // back operations on an empty deque
    ExpectEmpty();
    data = c_deque_back(deque);
    EXPECT_TRUE(data == 0);
    c_deque_pop_back(deque); // nothing should happen
}

TEST_F(CDequeTest, FrontOperations)
{
    // front operations on a non-empty deque
    SetupDeque(default_data, default_length);
    c_deque_iterator_t first = c_deque_begin(deque);
    c_deque_iterator_t last = c_deque_end(deque);

    size_t size = 0;
    c_ref_t data = 0;
    while (!c_deque_empty(deque)) {
        size = c_deque_size(deque);
        first = c_deque_begin(deque);
        last = c_deque_end(deque);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_deque_front(deque);
        EXPECT_EQ(default_data[default_length - size], C_DEREF_INT(data));
        c_algo_for_each(&first, &last, print_value);
        printf("\n");
        c_deque_pop_front(deque);
        EXPECT_EQ(size - 1, c_deque_size(deque));
    }

    // front operations on an empty deque
    ExpectEmpty();
    data = c_deque_front(deque);
    EXPECT_TRUE(data == 0);
    c_deque_pop_front(deque);
}

TEST_F(CDequeTest, Shrink)
{
    SetupDeque(default_data, default_length);
    size_t old_size = c_deque_size(deque);
    c_deque_shrink_to_fit(deque);
    size_t new_size = c_deque_size(deque);
    EXPECT_EQ(old_size, new_size);

    c_deque_clear(deque);
    EXPECT_TRUE(0 == c_deque_front(deque));
    c_deque_shrink_to_fit(deque);
    EXPECT_TRUE(0 == c_deque_front(deque));
    EXPECT_TRUE(0 == c_deque_back(deque));
}

TEST_F(CDequeTest, Resize)
{
    SetupDeque(default_data, default_length);

    int bigger[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0 };
    c_deque_resize(deque, c_deque_size(deque) + 3);
    ExpectEqualToArray(bigger, __array_length(bigger));

    int bigger_with_value[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 6, 6, 6 };
    int value = 6;
    c_deque_resize_with_value(deque, c_deque_size(deque) + 3, C_REF_T(&value));
    ExpectEqualToArray(bigger_with_value, __array_length(bigger_with_value));

    int smaller[] = { 0, 1, 2, 3, 4, 5 };
    c_deque_resize(deque, 6);
    ExpectEqualToArray(smaller, __array_length(smaller));

    int even_smaller[] = { 0, 1, 2, 3, 4 };
    c_deque_resize(deque, 5);
    ExpectEqualToArray(even_smaller, __array_length(even_smaller));
}

TEST_F(CDequeTest, Swap)
{
    SetupDeque(default_data, default_length);

    c_deque_t* other = C_DEQUE_INT;

    c_deque_swap(deque, other);
    ExpectEmpty();
    c_deque_swap(deque, other);
    ExpectEqualToArray(default_data, default_length);
    EXPECT_TRUE(c_deque_empty(other));

    c_deque_destroy(other);
}

TEST_F(CDequeTest, InsertErase)
{
    c_deque_iterator_t first, last, iter;
    c_ref_t data = 0;

    for (int i = 0; i < 9; ++i) {
        last = c_deque_end(deque);
        iter = c_deque_insert(deque, last, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        EXPECT_EQ(i, C_DEREF_INT(data));
        EXPECT_EQ(i + 1, c_deque_size(deque));
    }

    ExpectNotEmpty();

    first = c_deque_begin(deque);
    last = c_deque_end(deque);
    while (C_ITER_NE(&first, &last)) {
        size_t size = c_deque_size(deque);
        first = c_deque_erase(deque, first);
        last = c_deque_end(deque);
        EXPECT_EQ(size - 1, c_deque_size(deque));
    }

    first = c_deque_erase(deque, first);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));

    for (int i = 0; i < 9; ++i) {
        first = c_deque_begin(deque);
        iter = c_deque_insert(deque, first, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        first = c_deque_begin(deque);
        EXPECT_TRUE(C_ITER_EQ(&first, &iter));
        EXPECT_EQ(i, C_DEREF_INT(data));
    }

    first = c_deque_begin(deque);
    last = c_deque_end(deque);
    first = c_deque_erase_range(deque, first, last);
    ExpectEmpty();
}

} // namespace
} // namespace c_container
