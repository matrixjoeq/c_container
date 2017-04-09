
#include <gtest/gtest.h>
#include "c_internal.h"
#include "c_list.h"

namespace c_container {
namespace {

using ::testing::TestWithParam;
using ::testing::ValuesIn;

TEST(CListTest, CreateDestroy)
{
    c_list_t* list = 0;
    c_list_t* ret = C_LIST_INT(&list);
    EXPECT_TRUE(ret == list);
    EXPECT_TRUE(c_list_empty(list));
    EXPECT_EQ(c_list_size(list), 0);

    c_list_destroy(list);
}

TEST(CListModifier, BackOperations)
{
    c_list_t* list = 0;
    C_LIST_INT(&list);

    for (int i = 0; i < 9; ++i) {
		c_list_push_back(list, C_REF_T(&i));
        EXPECT_EQ(c_list_size(list), i + 1);
    }
	
	EXPECT_FALSE(c_list_empty(list));
	
    while (!c_list_empty(list)) {
        size_t size = c_list_size(list);
        c_list_pop_back(list);
        EXPECT_EQ(c_list_size(list), size - 1);
    }

    c_list_destroy(list);
}

TEST(CListModifier, FrontOperations)
{
    c_list_t* list = 0;
    C_LIST_INT(&list);

    for (int i = 0; i < 9; ++i) {
		c_list_push_front(list, C_REF_T(&i));
        EXPECT_EQ(c_list_size(list), i + 1);
    }
	
	EXPECT_FALSE(c_list_empty(list));
	
    while (!c_list_empty(list)) {
        size_t size = c_list_size(list);
        c_list_pop_front(list);
        EXPECT_EQ(c_list_size(list), size - 1);
    }

    c_list_destroy(list);
}

} // namespace
} // namespace c_container
