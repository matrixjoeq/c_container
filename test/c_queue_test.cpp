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
#include "c_queue.h"
#include "c_priority_queue.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

#pragma GCC diagnostic ignored "-Weffc++"
class CQueueTest : public ::testing::Test
{
public:
    CQueueTest() : queue(0) {}
    ~CQueueTest() { c_queue_destroy(queue); }

    void SetupQueue(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_queue_push(queue, C_REF_T(&datas[i]));

        ExpectNotEmpty();
    }

    void ExpectEmpty()
    {
        EXPECT_TRUE(c_queue_empty(queue));
    }

    void ExpectNotEmpty()
    {
        EXPECT_FALSE(c_queue_empty(queue));
    }

    void SetUp()
    {
        queue = C_QUEUE_INT;
        ExpectEmpty();
    }

    void TearDown()
    {
        c_queue_destroy(queue);
        queue = 0;
        ExpectEmpty();
    }

protected:
    c_queue_t* queue;
};

class CPriorityQueueTest : public ::testing::Test
{
public:
    CPriorityQueueTest() : queue(0) {}
    ~CPriorityQueueTest() { c_priority_queue_destroy(queue); }

    void SetupQueue(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_priority_queue_push(queue, C_REF_T(&datas[i]));

        ExpectNotEmpty();
    }

    void ExpectEmpty()
    {
        EXPECT_TRUE(c_priority_queue_empty(queue));
    }

    void ExpectNotEmpty()
    {
        EXPECT_FALSE(c_priority_queue_empty(queue));
    }

    void SetUp()
    {
        queue = C_PRIORITY_QUEUE_INT;
        ExpectEmpty();
    }

    void TearDown()
    {
        c_priority_queue_destroy(queue);
        queue = 0;
        ExpectEmpty();
    }

protected:
    c_priority_queue_t* queue;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CQueueTest, FrontBack)
{
    SetupQueue(default_data, default_length);

    c_ref_t front = 0, back = 0;
    int index = 0;
    while (!c_queue_empty(queue)) {
        front = c_queue_front(queue);
        back = c_queue_back(queue);
        EXPECT_EQ(default_data[index++], C_DEREF_INT(front));
        EXPECT_EQ(default_data[default_length - 1], C_DEREF_INT(back));
        c_queue_pop(queue);
    }

    ExpectEmpty();
}

TEST_F(CQueueTest, Swap)
{
    SetupQueue(default_data, default_length);

    c_queue_t* other = C_QUEUE_INT;

    c_queue_swap(queue, other);
    ExpectEmpty();

    c_queue_swap(queue, other);
    EXPECT_TRUE(c_queue_empty(other));
    c_ref_t front = 0;
    int index = 0;
    while (!c_queue_empty(queue)) {
        front = c_queue_front(queue);
        EXPECT_EQ(default_data[index++], C_DEREF_INT(front));
        c_queue_pop(queue);
    }

    ExpectEmpty();

    c_queue_destroy(other);
}

TEST_F(CPriorityQueueTest, PushPopTop)
{
    int max = INT32_MAX;
    SetupQueue(default_data, default_length);
    while (!c_priority_queue_empty(queue)) {
        c_ref_t value = c_priority_queue_top(queue);
        EXPECT_TRUE(C_DEREF_INT(value) < max);
        max = C_DEREF_INT(value);
        c_priority_queue_pop(queue);
    }
}

} // namespace
} // namespace c_container
