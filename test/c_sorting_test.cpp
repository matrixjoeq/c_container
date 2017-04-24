
#include <gtest/gtest.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_list.h"
#include "c_algorithm.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

bool greater(c_ref_t lhs, c_ref_t rhs)
{
    return (*(int*)lhs) > (*(int*)rhs);
}

#pragma GCC diagnostic ignored "-Weffc++"
class CSortTest : public ::testing::Test
{
public:
    CSortTest() : list(0) {}
    ~CSortTest() { TearDown(); }

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

TEST_F(CSortTest, IsSorted)
{
    SetupList(default_data, default_length);
    EXPECT_TRUE(c_algo_is_sorted(&first, &last));
    EXPECT_FALSE(c_algo_is_sorted_by(&first, &last, greater));
}

} // namespace
} // namespace c_container
