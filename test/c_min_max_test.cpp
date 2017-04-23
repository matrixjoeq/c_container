
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

} // namespace
} // namespace c_container
