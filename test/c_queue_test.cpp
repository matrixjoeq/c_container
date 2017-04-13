
#include <gtest/gtest.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_list.h"
#include "c_queue.h"

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

    void SetupStack(const int *datas, int length)
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
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CQueueTest, FrontBack)
{
    SetupStack(default_data, default_length);

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
    SetupStack(default_data, default_length);

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

} // namespace
} // namespace c_container
