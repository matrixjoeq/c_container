
#include <gtest/gtest.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_list.h"
#include "c_algorithm.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 3, 3, 3, 4, 6, 6, 7, 9 };
const int default_length = __array_length(default_data);

#pragma GCC diagnostic ignored "-Weffc++"
class CBinarySearchTest : public ::testing::Test
{
public:
    CBinarySearchTest() : list(0) {}
    ~CBinarySearchTest() { TearDown(); }

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

TEST_F(CBinarySearchTest, Bound)
{
    SetupList(default_data, default_length);

    int value = -1;
    c_algo_lower_bound(&first, &last, &value, &output);
    EXPECT_EQ(0, C_ITER_DISTANCE(&first, output));
    c_algo_upper_bound(&first, &last, &value, &output);
    EXPECT_EQ(0, C_ITER_DISTANCE(&first, output));

    value = 0;
    c_algo_lower_bound(&first, &last, &value, &output);
    EXPECT_EQ(0, C_ITER_DISTANCE(&first, output));
    c_algo_upper_bound(&first, &last, &value, &output);
    EXPECT_EQ(1, C_ITER_DISTANCE(&first, output));

    value = 1;
    c_algo_lower_bound(&first, &last, &value, &output);
    EXPECT_EQ(1, C_ITER_DISTANCE(&first, output));
    c_algo_upper_bound(&first, &last, &value, &output);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first, output));

    value = 2;
    c_algo_lower_bound(&first, &last, &value, &output);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first, output));
    c_algo_upper_bound(&first, &last, &value, &output);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first, output));

    value = 3;
    c_algo_lower_bound(&first, &last, &value, &output);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first, output));
    c_algo_upper_bound(&first, &last, &value, &output);
    EXPECT_EQ(5, C_ITER_DISTANCE(&first, output));

    value = 9;
    c_algo_lower_bound(&first, &last, &value, &output);
    EXPECT_EQ(9, C_ITER_DISTANCE(&first, output));
    c_algo_upper_bound(&first, &last, &value, &output);
    EXPECT_EQ(10, C_ITER_DISTANCE(&first, output));

    value = 10;
    c_algo_lower_bound(&first, &last, &value, &output);
    EXPECT_EQ(10, C_ITER_DISTANCE(&first, output));
    c_algo_upper_bound(&first, &last, &value, &output);
    EXPECT_EQ(10, C_ITER_DISTANCE(&first, output));
}

TEST_F(CBinarySearchTest, BinarySearch)
{
    SetupList(default_data, default_length);

    int value = -1;
    EXPECT_FALSE(c_algo_binary_search(&first, &last, &value));

    value = 0;
    EXPECT_TRUE(c_algo_binary_search(&first, &last, &value));

    value = 2;
    EXPECT_FALSE(c_algo_binary_search(&first, &last, &value));

    value = 3;
    EXPECT_TRUE(c_algo_binary_search(&first, &last, &value));

    value = 5;
    EXPECT_FALSE(c_algo_binary_search(&first, &last, &value));

    value = 6;
    EXPECT_TRUE(c_algo_binary_search(&first, &last, &value));

    value = 9;
    EXPECT_TRUE(c_algo_binary_search(&first, &last, &value));

    value = 10;
    EXPECT_FALSE(c_algo_binary_search(&first, &last, &value));
}

TEST_F(CBinarySearchTest, EqualRange)
{
    SetupList(default_data, default_length);

    int value = -1;
    c_iterator_t* lower = 0;
    c_iterator_t* upper = 0;

    c_algo_equal_range(&first, &last, &value, &lower, &upper);
    EXPECT_TRUE(C_ITER_EQ(lower, &first));
    EXPECT_TRUE(C_ITER_EQ(upper, &first));

    value = 0;
    c_algo_equal_range(&first, &last, &value, &lower, &upper);
    EXPECT_EQ(0, C_ITER_DISTANCE(&first, lower));
    EXPECT_EQ(1, C_ITER_DISTANCE(&first, upper));

    value = 2;
    c_algo_equal_range(&first, &last, &value, &lower, &upper);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first, lower));
    EXPECT_EQ(2, C_ITER_DISTANCE(&first, upper));

    value = 3;
    c_algo_equal_range(&first, &last, &value, &lower, &upper);
    EXPECT_EQ(2, C_ITER_DISTANCE(&first, lower));
    EXPECT_EQ(5, C_ITER_DISTANCE(&first, upper));

    value = 5;
    c_algo_equal_range(&first, &last, &value, &lower, &upper);
    EXPECT_EQ(6, C_ITER_DISTANCE(&first, lower));
    EXPECT_EQ(6, C_ITER_DISTANCE(&first, upper));

    value = 6;
    c_algo_equal_range(&first, &last, &value, &lower, &upper);
    EXPECT_EQ(6, C_ITER_DISTANCE(&first, lower));
    EXPECT_EQ(8, C_ITER_DISTANCE(&first, upper));

    value = 9;
    c_algo_equal_range(&first, &last, &value, &lower, &upper);
    EXPECT_EQ(9, C_ITER_DISTANCE(&first, lower));
    EXPECT_TRUE(C_ITER_EQ(upper, &last));

    value = 10;
    c_algo_equal_range(&first, &last, &value, &lower, &upper);
    EXPECT_TRUE(C_ITER_EQ(lower, &last));
    EXPECT_TRUE(C_ITER_EQ(upper, &last));
}

} // namespace
} // namespace c_container
