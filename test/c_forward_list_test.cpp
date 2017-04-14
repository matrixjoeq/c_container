
#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <forward_list>
#include "c_internal.h"
#include "c_iterator.h"
#include "c_forward_list.h"

namespace c_container {
namespace {

const int default_data[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
const int expect_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

uint64_t get_time_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    uint64_t ret = tv.tv_usec;
    /* Convert from micro seconds (10^-6) to milliseconds (10^-3) */
    ret /= 1000;

    /* Adds the seconds (10^0) after converting them to milliseconds (10^-3) */
    ret += (tv.tv_sec * 1000);

    return ret;
}

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
class CForwardListTest : public ::testing::Test
{
public:
    CForwardListTest() : list(0) {}
    ~CForwardListTest() { c_slist_destroy(list); }

    void SetupList(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_slist_push_front(list, C_REF_T(&datas[i]));

        ExpectNotEmpty();
    }

    void Traverse(void)
    {
#ifdef CONFIG_TRAVERSE
        c_slist_iterator_t last = c_slist_end(list);
        for (c_slist_iterator_t iter = c_slist_begin(list); C_ITER_NE(&iter, &last); C_ITER_INC(&iter))
            printf("%d ", C_DEREF_INT(C_ITER_DEREF(&iter)));

        printf("\n");
#endif
    }

    void ExpectEqualToArray(const int* datas, int length)
    {
        c_slist_iterator_t first = c_slist_begin(list);
        c_ref_t data = 0;
        for (int i = 0; i < length; ++i) {
            data = C_ITER_DEREF(&first);
            EXPECT_EQ(datas[i], C_DEREF_INT(data));
            C_ITER_INC(&first);
        }
    }

    void ExpectEmpty()
    {
        c_slist_iterator_t first = c_slist_begin(list);
        c_slist_iterator_t last = c_slist_end(list);
        EXPECT_TRUE(c_slist_empty(list));
        EXPECT_TRUE(C_ITER_EQ(&first, &last));
    }

    void ExpectNotEmpty()
    {
        c_slist_iterator_t first = c_slist_begin(list);
        c_slist_iterator_t last = c_slist_end(list);
        EXPECT_FALSE(c_slist_empty(list));
        EXPECT_TRUE(C_ITER_NE(&first, &last));
    }

    void SetUp()
    {
        list = C_SLIST_INT;
        ExpectEmpty();
    }

    void TearDown()
    {
        c_slist_destroy(list);
        list = 0;
        EXPECT_TRUE(c_slist_empty(list));
    }

protected:
    c_slist_t* list;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CForwardListTest, Clear)
{
    SetupList(default_data, default_length);
    c_slist_clear(list);
    ExpectEmpty();
}

TEST_F(CForwardListTest, BeginEnd)
{
    SetupList(default_data, default_length);
    c_slist_iterator_t before_first = c_slist_before_begin(list);
    c_slist_iterator_t first = c_slist_begin(list);
    c_slist_iterator_t last = c_slist_end(list);

    C_ITER_INC(&before_first);
    EXPECT_TRUE(C_ITER_EQ(&before_first, &first));

    size_t size = 0;
    while (C_ITER_NE(&first, &last)) {
        C_ITER_INC(&first);
        ++size;
    }

    EXPECT_EQ(default_length, size);
}

TEST_F(CForwardListTest, InsertErase)
{
    c_slist_iterator_t first, last, iter;
    c_ref_t data = 0;

    first = c_slist_before_begin(list);
    last = c_slist_end(list);

    for (int i = 9; i >= 0; --i) {
        iter = c_slist_insert_after(list, first, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        EXPECT_EQ(i, C_DEREF_INT(data));
    }

    ExpectNotEmpty();

    iter = c_slist_begin(list);
    int index = 0;
    while (C_ITER_NE(&iter, &last)) {
        data = C_ITER_DEREF(&iter);
        EXPECT_EQ(index++, C_DEREF_INT(data));
        iter = c_slist_erase_after(list, first);
    }

    iter = c_slist_erase_after(list, first);
    EXPECT_TRUE(C_ITER_EQ(&iter, &last));

    iter = c_slist_erase_after(list, last);
    EXPECT_TRUE(C_ITER_EQ(&iter, &last));

    SetupList(default_data, default_length);

    iter = c_slist_erase_after_range(list, first, last);
    EXPECT_TRUE(C_ITER_EQ(&iter, &last));
    ExpectEmpty();
}

TEST_F(CForwardListTest, FrontOperations)
{
    SetupList(default_data, default_length);

    c_slist_iterator_t first, last;
    c_ref_t data = 0;
    int i = 0;
    while (!c_slist_empty(list)) {
        first = c_slist_begin(list);
        last = c_slist_end(list);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_slist_front(list);
        EXPECT_EQ(i++, C_DEREF_INT(data));
        c_slist_pop_front(list);
    }

    ExpectEmpty();
}

TEST_F(CForwardListTest, Resize)
{
    SetupList(default_data, default_length);
    int count = default_length;

    int bigger[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0 };
    count += 3;
    c_slist_resize(list, count);
    ExpectEqualToArray(bigger, __array_length(bigger));

    int bigger_with_value[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 6, 6, 6 };
    int value = 6;
    count += 3;
    c_slist_resize_with_value(list, count, C_REF_T(&value));
    ExpectEqualToArray(bigger_with_value, __array_length(bigger_with_value));

    int smaller[] = { 0, 1, 2, 3, 4, 5 };
    c_slist_resize(list, 6);
    ExpectEqualToArray(smaller, __array_length(smaller));

    c_slist_resize(list, 0);
    ExpectEmpty();
}

TEST_F(CForwardListTest, Swap)
{
    SetupList(default_data, default_length);

    c_slist_t* other = C_SLIST_INT;

    c_slist_swap(list, other);
    ExpectEmpty();
    c_slist_swap(list, other);
    ExpectEqualToArray(expect_data, __array_length(expect_data));
    EXPECT_TRUE(c_slist_empty(other));

    c_slist_destroy(other);
}

TEST_F(CForwardListTest, Merge)
{
    c_slist_t* other_list = C_SLIST_INT;

    int origin[] = { 11, 10, 10, 7, 5, 5, 5, 4, 3, 3, 1, -1, -2, -2 };
    int merged[] = { -2, -2, -1, 0, 1, 1, 2, 3, 3, 3, 4, 4, 5, 5, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11 };

    __array_foreach(origin, i) {
        c_slist_push_front(other_list, C_REF_T(&origin[i]));
    }

    SetupList(default_data, default_length);
    //c_slist_sort(list);
    //c_slist_sort(other_list);
    c_slist_merge(list, other_list);
    Traverse();
    ExpectEqualToArray(merged, __array_length(merged));

    c_slist_destroy(other_list);
}

TEST_F(CForwardListTest, MergeBy)
{
    c_slist_t* other_list = C_SLIST_INT;

    int origin[] = { -2, -2, -1, 1, 3, 3, 4, 5, 5, 5, 7, 10, 10, 11 };
    int merged[] = { 11, 10, 10, 9, 8, 7, 7, 6, 5, 5, 5, 5, 4, 4, 3, 3, 3, 2, 1, 1, 0, -1, -2, -2 };

    __array_foreach(origin, i) {
        c_slist_push_front(other_list, C_REF_T(&origin[i]));
    }

    int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    __array_foreach(default_data, i) {
        c_slist_push_front(list, C_REF_T(&default_data[i]));
    }

    //SetupList(default_data, default_length);
    //c_slist_sort_by(list, c_int_greater);
    //c_slist_sort_by(other_list, c_int_greater);
    c_slist_merge_by(list, other_list, c_int_greater);
    ExpectEqualToArray(merged, __array_length(merged));

    c_slist_destroy(other_list);
}

TEST_F(CForwardListTest, Splice)
{
    SetupList(default_data, default_length);
    int origin[] = { 12, 11, 10 };

    c_slist_t* other = C_SLIST_INT;

    __array_foreach(origin, i) {
        c_slist_push_front(other, C_REF_T(&origin[i]));
    }
    c_slist_splice_after(list, c_slist_before_begin(list), other);
    Traverse();
    int spliced_before_begin[] = { 10, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ExpectEqualToArray(spliced_before_begin, __array_length(spliced_before_begin));
    EXPECT_TRUE(c_slist_empty(other));

    __array_foreach(origin, i) {
        c_slist_push_front(other, C_REF_T(&origin[i]));
    }
    c_slist_splice_after(list, c_slist_begin(list), other);
    Traverse();
    int spliced_begin[] = { 10, 10, 11, 12, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ExpectEqualToArray(spliced_begin, __array_length(spliced_begin));
    EXPECT_TRUE(c_slist_empty(other));

    __array_foreach(origin, i) {
        c_slist_push_front(other, C_REF_T(&origin[i]));
    }
    c_slist_splice_after_from(list, c_slist_before_begin(list), other, c_slist_before_begin(other));
    Traverse();
    int spliced_from[] = { 10, 11, 12, 10, 10, 11, 12, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ExpectEqualToArray(spliced_from, __array_length(spliced_from));
    EXPECT_TRUE(c_slist_empty(other));

    __array_foreach(origin, i) {
        c_slist_push_front(other, C_REF_T(&origin[i]));
    }
    c_slist_splice_after_range(list, c_slist_before_begin(list), other, c_slist_begin(other), c_slist_end(other));
    Traverse();
    int spliced_range[] = { 11, 12, 10, 11, 12, 10, 10, 11, 12, 11, 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    ExpectEqualToArray(spliced_range, __array_length(spliced_range));
    EXPECT_FALSE(c_slist_empty(other));

    c_slist_destroy(other);
}

TEST_F(CForwardListTest, Remove)
{
    int origin[] = { 4, 4, 4, 1, 4, 4, 2, 3, 3, 1, 3, 2, 1 };
    int removed_4[] = { 1, 2, 3, 1, 3, 3, 2, 1 };
    int removed_3[] = { 1, 2, 1, 2, 1 };
    int removed_2[] = { 1, 1, 1 };

    SetupList(origin, __array_length(origin));

    int to_be_removed = 4;
    c_slist_remove(list, C_REF_T(&to_be_removed));
    Traverse();
    ExpectEqualToArray(removed_4, __array_length(removed_4));

    to_be_removed = 3;
    c_slist_remove(list, C_REF_T(&to_be_removed));
    Traverse();
    ExpectEqualToArray(removed_3, __array_length(removed_3));

    to_be_removed = 2;
    c_slist_remove(list, C_REF_T(&to_be_removed));
    Traverse();
    ExpectEqualToArray(removed_2, __array_length(removed_2));

    to_be_removed = 1;
    c_slist_remove(list, C_REF_T(&to_be_removed));
    Traverse();
    ExpectEmpty();
}

TEST_F(CForwardListTest, RemoveIf)
{
    int removed[] = { 0, 1, 2, 3, 4, 5 };

    SetupList(default_data, default_length);
    c_slist_remove_if(list, greater_than_five);
    Traverse();
    ExpectEqualToArray(removed, __array_length(removed));
}

TEST_F(CForwardListTest, Sort)
{
    int sorted[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int random[] = { 5, 8, 1, 0, 3, 2, 7, 6, 4, 9 };

    SetupList(random, __array_length(random));
    c_slist_sort(list);
    ExpectEqualToArray(sorted, __array_length(sorted));
}

TEST_F(CForwardListTest, SortBy)
{
    int sorted[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    int random[] = { 5, 8, 1, 0, 3, 2, 7, 6, 4, 9 };

    SetupList(random, __array_length(random));
    c_slist_sort_by(list, c_int_greater);
    ExpectEqualToArray(sorted, __array_length(sorted));
}

TEST_F(CForwardListTest, SortPerformance)
{
    std::forward_list<int> fl(1000);
    srandom(static_cast<unsigned int>(time(0)));
    int data = 0;
    for (std::forward_list<int>::iterator iter = fl.begin(); iter != fl.end(); ++iter) {
        data = random() % INT32_MAX;
        *iter = data;
        c_slist_push_front(list, C_REF_T(&data));
    }

    uint64_t b_time = get_time_ms();
    fl.sort();
    uint64_t e_time = get_time_ms();
    printf("STL takes %lu ms to sort\n", e_time - b_time);

    b_time = get_time_ms();
    c_slist_sort(list);
    e_time = get_time_ms();
    printf("C takes %lu ms to sort\n", e_time - b_time);
}

TEST_F(CForwardListTest, Reverse)
{
    int reversed[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

    SetupList(default_data, default_length);
    c_slist_reverse(list);
    ExpectEqualToArray(reversed, __array_length(reversed));
}

TEST_F(CForwardListTest, Unique)
{
    int not_unique[] = { 2, 2, 1, 0, 0, 3, 1, 1, 2, 2, 1, 0 };
    int uniqued[] = { 0, 1, 2, 1, 3, 0, 1, 2 };

    SetupList(not_unique, __array_length(not_unique));
    c_slist_unique(list);
    ExpectEqualToArray(uniqued, __array_length(uniqued));
}

TEST_F(CForwardListTest, UniqueIf)
{
    int not_unique[] = { -2, 2, 2, -2, 1, 0, 0, 3, -1, 1, 2, -2, 1, 0 };
    int uniqued[] = { 0, 1, -2, 1, 3, 0, 1, -2 };

    SetupList(not_unique, __array_length(not_unique));
    c_slist_unique_if(list, abs_equal);
    ExpectEqualToArray(uniqued, __array_length(uniqued));
}

} // namespace
} // namespace c_container
