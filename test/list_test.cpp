
#include <gtest/gtest.h>
#include "c_internal.h"
#include "c_iterator.h"
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
    EXPECT_EQ(0, c_list_size(list));
    c_list_iterator_t first = c_list_begin(list);
    c_list_iterator_t last = c_list_end(list);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));

    c_list_destroy(list);
}

TEST(CListModifier, Clear)
{
    c_list_t* list = 0;
    C_LIST_INT(&list);

    for (int i = 0; i < 9; ++i)
        c_list_push_back(list, C_REF_T(&i));

    c_list_iterator_t first = c_list_begin(list);
    c_list_iterator_t last = c_list_end(list);
    EXPECT_FALSE(c_list_empty(list));
    EXPECT_NE(0, c_list_size(list));
    EXPECT_TRUE(C_ITER_NE(&first, &last));

    c_list_clear(list);
    first = c_list_begin(list);
    last = c_list_end(list);
    EXPECT_TRUE(c_list_empty(list));
    EXPECT_EQ(0, c_list_size(list));
    EXPECT_TRUE(C_ITER_EQ(&first, &last));

    c_list_destroy(list);
}

TEST(CListModifier, BackOperations)
{
    c_list_t* list = 0;
    C_LIST_INT(&list);

    for (int i = 0; i < 9; ++i) {
        c_list_push_back(list, C_REF_T(&i));
        c_ref_t data = c_list_back(list);
        EXPECT_EQ(i + 1, c_list_size(list));
        EXPECT_EQ(i, C_DEREF_INT(data));
    }

    c_list_iterator_t first = c_list_begin(list);
    c_list_iterator_t last = c_list_end(list);
    EXPECT_TRUE(C_ITER_NE(&first, &last));
    EXPECT_FALSE(c_list_empty(list));

    while (!c_list_empty(list)) {
        size_t size = c_list_size(list);
        first = c_list_begin(list);
        last = c_list_end(list);
        EXPECT_TRUE(C_ITER_NE(&first, &last));

        c_list_pop_back(list);
        EXPECT_EQ(size - 1, c_list_size(list));
    }

    first = c_list_begin(list);
    last = c_list_end(list);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));
    EXPECT_EQ(0, c_list_size(list));

    c_list_destroy(list);
}

TEST(CListModifier, FrontOperations)
{
    c_list_t* list = 0;
    C_LIST_INT(&list);

    for (int i = 0; i < 9; ++i) {
        c_list_push_front(list, C_REF_T(&i));
        c_ref_t data = c_list_front(list);
        EXPECT_EQ(i + 1, c_list_size(list));
        EXPECT_EQ(i, C_DEREF_INT(data));
    }

    c_list_iterator_t first = c_list_begin(list);
    c_list_iterator_t last = c_list_end(list);
    EXPECT_TRUE(C_ITER_NE(&first, &last));
    EXPECT_FALSE(c_list_empty(list));

    while (!c_list_empty(list)) {
        size_t size = c_list_size(list);
        first = c_list_begin(list);
        last = c_list_end(list);
        EXPECT_TRUE(C_ITER_NE(&first, &last));

        c_list_pop_front(list);
        EXPECT_EQ(size - 1, c_list_size(list));
    }

    first = c_list_begin(list);
    last = c_list_end(list);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));
    EXPECT_EQ(0, c_list_size(list));

    c_list_destroy(list);
}

TEST(CListModifier, InsertErase)
{
    c_list_t* list = 0;
    C_LIST_INT(&list);

    c_list_iterator_t first, last;

    for (int i = 0; i < 9; ++i) {
        last = c_list_end(list);
        c_list_iterator_t iter = c_list_insert(list, last, C_REF_T(&i));
        c_ref_t data = C_ITER_DEREF(&iter);
        EXPECT_EQ(i, C_DEREF_INT(data));
        EXPECT_EQ(i + 1, c_list_size(list));
    }

    first = c_list_begin(list);
    last = c_list_end(list);
    EXPECT_TRUE(C_ITER_NE(&first, &last));
    EXPECT_FALSE(c_list_empty(list));

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
        c_list_iterator_t iter = c_list_insert(list, first, C_REF_T(&i));
        c_ref_t data = C_ITER_DEREF(&iter);
        first = c_list_begin(list);
        EXPECT_TRUE(C_ITER_EQ(&first, &iter));
        EXPECT_EQ(i, C_DEREF_INT(data));
    }

    first = c_list_begin(list);
    last = c_list_end(list);
    first = c_list_erase_range(list, first, last);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));
    EXPECT_TRUE(c_list_empty(list));
    EXPECT_EQ(0, c_list_size(list));

    c_list_destroy(list);
}

} // namespace
} // namespace c_container
