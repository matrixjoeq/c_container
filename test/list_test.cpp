
#include <gtest/gtest.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_iterator.h"
#include "c_list.h"

namespace c_container {
namespace {

#define ARRAY_LENGTH(a) sizeof((a)) / sizeof((a)[0])

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = ARRAY_LENGTH(default_data);

bool c_int_greater(const c_ref_t lhs, const c_ref_t rhs)
{
    return C_DEREF_INT(lhs) > C_DEREF_INT(rhs);
}

bool greater_than_five(const c_ref_t data)
{
    return C_DEREF_INT(data) > 5;
}

bool abs_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return abs(C_DEREF_INT(lhs)) == abs(C_DEREF_INT(rhs));
}

#pragma GCC diagnostic ignored "-Weffc++"
class CListTest : public ::testing::Test
{
public:
    CListTest() : list(0) {}
    ~CListTest() { c_list_destroy(list); }

    void SetupList(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_list_push_back(list, C_REF_T(&datas[i]));

        ExpectNotEmpty();
        EXPECT_EQ(length, c_list_size(list));
    }

    void ExpectEqualToArray(const int* datas, int length)
    {
        EXPECT_EQ(length, c_list_size(list));
        c_list_iterator_t first = c_list_begin(list);
        c_ref_t data = 0;
        for (int i = 0; i < length; ++i) {
            data = C_ITER_DEREF(&first);
            EXPECT_EQ(datas[i], C_DEREF_INT(data));
            C_ITER_INC(&first);
        }
    }

    void ExpectEmpty()
    {
        c_list_iterator_t first = c_list_begin(list);
        c_list_iterator_t last = c_list_end(list);
        EXPECT_TRUE(c_list_empty(list));
        EXPECT_EQ(0, c_list_size(list));
        EXPECT_TRUE(C_ITER_EQ(&first, &last));
    }

    void ExpectNotEmpty()
    {
        c_list_iterator_t first = c_list_begin(list);
        c_list_iterator_t last = c_list_end(list);
        EXPECT_FALSE(c_list_empty(list));
        EXPECT_NE(0, c_list_size(list));
        EXPECT_TRUE(C_ITER_NE(&first, &last));
    }

    void SetUp()
    {
        c_list_t* ret = C_LIST_INT(&list);
        EXPECT_TRUE(ret == list);
        ExpectEmpty();
    }

    void TearDown()
    {
        c_list_destroy(list);
        list = 0;
        EXPECT_TRUE(c_list_empty(list));
        EXPECT_EQ(0, c_list_size(list));
    }

protected:
    c_list_t* list;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CListTest, Clear)
{
    SetupList(default_data, default_length);
    c_list_clear(list);
    ExpectEmpty();
}

TEST_F(CListTest, BeginEnd)
{
	SetupList(default_data, default_length);
	c_list_iterator_t first = c_list_begin(list);
	c_list_iterator_t last = c_list_end(list);
	c_list_iterator_t rfirst = c_list_rbegin(list);
	c_list_iterator_t rlast = c_list_rend(list);
	C_ITER_DEC(&last);
	C_ITER_DEC(&rlast);
	EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&first)), C_DEREF_INT(C_ITER_DEREF(&rlast)));
	EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&rfirst)), C_DEREF_INT(C_ITER_DEREF(&last)));
}

TEST_F(CListTest, BackOperations)
{
    SetupList(default_data, default_length);

    c_list_iterator_t first, last;
    size_t size = 0;
    c_ref_t data = 0;
    while (!c_list_empty(list)) {
        size = c_list_size(list);
        first = c_list_begin(list);
        last = c_list_end(list);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_list_back(list);
        EXPECT_EQ(size - 1, C_DEREF_INT(data));
        c_list_pop_back(list);
        EXPECT_EQ(size - 1, c_list_size(list));
    }

    ExpectEmpty();
}

TEST_F(CListTest, FrontOperations)
{
    SetupList(default_data, default_length);

    c_list_iterator_t first, last;
    size_t size = 0;
    c_ref_t data = 0;
    int i = 0;
    while (!c_list_empty(list)) {
        size = c_list_size(list);
        first = c_list_begin(list);
        last = c_list_end(list);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_list_front(list);
        EXPECT_EQ(i++, C_DEREF_INT(data));
        c_list_pop_front(list);
        EXPECT_EQ(size - 1, c_list_size(list));
    }

    ExpectEmpty();
}

TEST_F(CListTest, InsertErase)
{
    c_list_iterator_t first, last, iter;
    c_ref_t data = 0;

    for (int i = 0; i < 9; ++i) {
        last = c_list_end(list);
        iter = c_list_insert(list, last, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        EXPECT_EQ(i, C_DEREF_INT(data));
        EXPECT_EQ(i + 1, c_list_size(list));
    }

    ExpectNotEmpty();

    first = c_list_begin(list);
    last = c_list_end(list);
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
        iter = c_list_insert(list, first, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        first = c_list_begin(list);
        EXPECT_TRUE(C_ITER_EQ(&first, &iter));
        EXPECT_EQ(i, C_DEREF_INT(data));
    }

    first = c_list_begin(list);
    last = c_list_end(list);
    first = c_list_erase_range(list, first, last);
    ExpectEmpty();
}

TEST_F(CListTest, Merge)
{
    c_list_t* other_list = 0;
    C_LIST_INT(&other_list);

    int origin[] = { -2, -2, -1, 1, 3, 3, 4, 5, 5, 5, 7, 10, 10, 11 };
    int merged[] = { -2, -2, -1, 0, 1, 1, 2, 3, 3, 3, 4, 4, 5, 5, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11 };

    for (unsigned int i = 0; i < ARRAY_LENGTH(origin); ++i)
        c_list_push_back(other_list, C_REF_T(&origin[i]));

    SetupList(default_data, default_length);
    c_list_sort(list);
    c_list_sort(other_list);
    c_list_merge(list, other_list);
    ExpectEqualToArray(merged, ARRAY_LENGTH(merged));

    c_list_destroy(other_list);
}

TEST_F(CListTest, MergeBy)
{
    c_list_t* other_list = 0;
    C_LIST_INT(&other_list);

    int origin[] = { -2, -2, -1, 1, 3, 3, 4, 5, 5, 5, 7, 10, 10, 11 };
    int merged[] = { 11, 10, 10, 9, 8, 7, 7, 6, 5, 5, 5, 5, 4, 4, 3, 3, 3, 2, 1, 1, 0, -1, -2, -2 };

    for (unsigned int i = 0; i < ARRAY_LENGTH(origin); ++i)
        c_list_push_back(other_list, C_REF_T(&origin[i]));

    SetupList(default_data, default_length);
    c_list_sort_by(list, c_int_greater);
    c_list_sort_by(other_list, c_int_greater);
    c_list_merge_by(list, other_list, c_int_greater);
    ExpectEqualToArray(merged, ARRAY_LENGTH(merged));

    c_list_destroy(other_list);
}

TEST_F(CListTest, Remove)
{
    int origin[] = { 1, 2, 3, 1, 3, 3, 2, 4, 4, 1, 4, 4, 4 };
    int removed_4[] = { 1, 2, 3, 1, 3, 3, 2, 1 };
    int removed_3[] = { 1, 2, 1, 2, 1 };
    int removed_2[] = { 1, 1, 1 };

    SetupList(origin, ARRAY_LENGTH(origin));

    int to_be_removed = 4;
    c_list_remove(list, C_REF_T(&to_be_removed));
    ExpectEqualToArray(removed_4, ARRAY_LENGTH(removed_4));

    to_be_removed = 3;
    c_list_remove(list, C_REF_T(&to_be_removed));
    ExpectEqualToArray(removed_3, ARRAY_LENGTH(removed_3));

    to_be_removed = 2;
    c_list_remove(list, C_REF_T(&to_be_removed));
    ExpectEqualToArray(removed_2, ARRAY_LENGTH(removed_2));

    to_be_removed = 1;
    c_list_remove(list, C_REF_T(&to_be_removed));
    ExpectEmpty();
}

TEST_F(CListTest, RemoveIf)
{
    int removed[] = { 0, 1, 2, 3, 4, 5 };

    SetupList(default_data, default_length);
    c_list_remove_if(list, greater_than_five);
    ExpectEqualToArray(removed, ARRAY_LENGTH(removed));
}

TEST_F(CListTest, Sort)
{
    int sorted[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int random[] = { 5, 8, 1, 0, 3, 2, 7, 6, 4, 9 };

    SetupList(random, ARRAY_LENGTH(random));
    c_list_sort(list);
    ExpectEqualToArray(sorted, ARRAY_LENGTH(sorted));
}

TEST_F(CListTest, SortBy)
{
    int sorted[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    int random[] = { 5, 8, 1, 0, 3, 2, 7, 6, 4, 9 };

    SetupList(random, ARRAY_LENGTH(random));
    c_list_sort_by(list, c_int_greater);
    ExpectEqualToArray(sorted, ARRAY_LENGTH(sorted));
}

TEST_F(CListTest, Reverse)
{
    int reversed[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

    SetupList(default_data, default_length);
    c_list_reverse(list);
    ExpectEqualToArray(reversed, ARRAY_LENGTH(reversed));
}

TEST_F(CListTest, Unique)
{
    int not_unique[] = { 0, 1, 2, 2, 1, 1, 3, 0, 0, 1, 2, 2 };
    int uniqued[] = { 0, 1, 2, 1, 3, 0, 1, 2 };

    SetupList(not_unique, ARRAY_LENGTH(not_unique));
    c_list_unique(list);
    ExpectEqualToArray(uniqued, ARRAY_LENGTH(uniqued));
}

TEST_F(CListTest, UniqueIf)
{
    int not_unique[] = { 0, 1, -2, 2, 1, -1, 3, 0, 0, 1, -2, 2, 2, -2 };
    int uniqued[] = { 0, 1, -2, 1, 3, 0, 1, -2 };

    SetupList(not_unique, ARRAY_LENGTH(not_unique));
    c_list_unique_if(list, abs_equal);
    ExpectEqualToArray(uniqued, ARRAY_LENGTH(uniqued));
}

} // namespace
} // namespace c_container
