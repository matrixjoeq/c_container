
#include <gtest/gtest.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_forward_list.h"
#include "c_list.h"
#include "c_vector.h"
#include "c_algorithm.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

bool greater_than_two(const c_ref_t data)
{
    return C_DEREF_INT(data) > 2;
}

bool no_less_than_zero(const c_ref_t data)
{
    return C_DEREF_INT(data) >= 0;
}

bool greater_than_ten(const c_ref_t data)
{
    return C_DEREF_INT(data) > 10;
}

void print_value(const c_ref_t data)
{
    printf("%d ", C_DEREF_INT(data));
}

inline void print_newline(void)
{
    printf("\n");
}

#pragma GCC diagnostic ignored "-Weffc++"
class CNonModifyingTest : public ::testing::Test
{
public:
    CNonModifyingTest() : __fl(0), __l(0), __v(0), l_found(0), fl_found(0), v_found(0) {}
    ~CNonModifyingTest() {
        TearDown();
    }

    void SetupAll(const int *datas, int length)
    {
        SetupList(datas, length);
        SetupForwardList(datas, length);
        SetupVector(datas, length);
    }

    void SetupList(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_list_push_back(__l, C_REF_T(&datas[i]));

        l_first = c_list_begin(__l);
        l_last = c_list_end(__l);
    }

    void SetupForwardList(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_slist_push_front(__fl, C_REF_T(&datas[i]));

        fl_first = c_slist_begin(__fl);
        fl_last = c_slist_end(__fl);
    }

    void SetupVector(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_vector_push_back(__v, C_REF_T(&datas[i]));

        v_first = c_vector_begin(__v);
        v_last = c_vector_end(__v);
    }

    void SetUp()
    {
        __fl = C_SLIST_INT;
        fl_first = c_slist_begin(__fl);
        fl_last = c_slist_end(__fl);

        __l = C_LIST_INT;
        l_first = c_list_begin(__l);
        l_last = c_list_end(__l);

        __v = C_VECTOR_INT;
        v_first = c_vector_begin(__v);
        v_last = c_vector_end(__v);
    }

    void TearDown()
    {
        c_slist_destroy(__fl);
        __fl = 0;
        __c_free(fl_found);
        fl_found = 0;

        c_list_destroy(__l);
        __l = 0;
        __c_free(l_found);
        l_found = 0;

        c_vector_destroy(__v);
        __v = 0;
        __c_free(v_found);
        v_found = 0;
    }

protected:
    c_slist_t* __fl;
    c_list_t* __l;
    c_vector_t* __v;

    c_list_iterator_t l_first;
    c_list_iterator_t l_last;
    c_slist_iterator_t fl_first;
    c_slist_iterator_t fl_last;
    c_vector_iterator_t v_first;
    c_vector_iterator_t v_last;

    c_iterator_t* l_found;
    c_iterator_t* fl_found;
    c_iterator_t* v_found;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CNonModifyingTest, AllAnyNone)
{
    SetupAll(default_data, default_length);

    EXPECT_TRUE(c_algo_all_of(&l_first, &l_last, no_less_than_zero));
    EXPECT_TRUE(c_algo_all_of(&fl_first, &fl_last, no_less_than_zero));
    EXPECT_TRUE(c_algo_all_of(&v_first, &v_last, no_less_than_zero));

    EXPECT_FALSE(c_algo_all_of(&l_first, &l_last, greater_than_two));
    EXPECT_FALSE(c_algo_all_of(&fl_first, &fl_last, greater_than_two));
    EXPECT_FALSE(c_algo_all_of(&v_first, &v_last, greater_than_two));

    EXPECT_TRUE(c_algo_any_of(&l_first, &l_last, greater_than_two));
    EXPECT_TRUE(c_algo_any_of(&fl_first, &fl_last, greater_than_two));
    EXPECT_TRUE(c_algo_any_of(&v_first, &v_last, greater_than_two));

    EXPECT_FALSE(c_algo_any_of(&l_first, &l_last, greater_than_ten));
    EXPECT_FALSE(c_algo_any_of(&fl_first, &fl_last, greater_than_ten));
    EXPECT_FALSE(c_algo_any_of(&v_first, &v_last, greater_than_ten));

    EXPECT_TRUE(c_algo_none_of(&l_first, &l_last, greater_than_ten));
    EXPECT_TRUE(c_algo_none_of(&fl_first, &fl_last, greater_than_ten));
    EXPECT_TRUE(c_algo_none_of(&v_first, &v_last, greater_than_ten));

    EXPECT_FALSE(c_algo_none_of(&l_first, &l_last, no_less_than_zero));
    EXPECT_FALSE(c_algo_none_of(&fl_first, &fl_last, no_less_than_zero));
    EXPECT_FALSE(c_algo_none_of(&v_first, &v_last, no_less_than_zero));
}

TEST_F(CNonModifyingTest, ForEach)
{
    SetupAll(default_data, default_length);

    c_algo_for_each(&l_first, &l_last, print_value);
    print_newline();
    c_algo_for_each(&fl_first, &fl_last, print_value);
    print_newline();
    c_algo_for_each(&v_first, &v_last, print_value);
    print_newline();

    c_iterator_t* __l_first = C_ITER_T(&l_first);
    c_algo_for_each_n(&l_first, default_length, &__l_first, print_value);
    print_newline();
    EXPECT_TRUE(C_ITER_EQ(&l_first, &l_last));
    c_iterator_t* __fl_first = C_ITER_T(&fl_first);
    c_algo_for_each_n(&fl_first, default_length, &__fl_first, print_value);
    print_newline();
    EXPECT_TRUE(C_ITER_EQ(&fl_first, &fl_last));
    c_iterator_t* __v_first = C_ITER_T(&v_first);
    c_algo_for_each_n(&v_first, default_length, &__v_first, print_value);
    print_newline();
    EXPECT_TRUE(C_ITER_EQ(&v_first, &v_last));
}

TEST_F(CNonModifyingTest, Count)
{
    int numbers[] = { 1, 2, 2, 3, 3, 3, 4, 4, 4, 4 };
    SetupAll(numbers, __array_length(numbers));

    size_t count = 0;
    for (int i = 0; i < 5; ++i) {
        count = c_algo_count(&l_first, &l_last, &i);
        EXPECT_EQ(i, count);
        count = c_algo_count(&fl_first, &fl_last, &i);
        EXPECT_EQ(i, count);
        count = c_algo_count(&v_first, &v_last, &i);
        EXPECT_EQ(i, count);
    }

    count = c_algo_count_if(&l_first, &l_last, greater_than_two);
    EXPECT_EQ(7, count);
    count = c_algo_count_if(&fl_first, &fl_last, greater_than_two);
    EXPECT_EQ(7, count);
    count = c_algo_count_if(&v_first, &v_last, greater_than_two);
    EXPECT_EQ(7, count);
}

TEST_F(CNonModifyingTest, Mismatch)
{
    SetupAll(default_data, default_length);

    EXPECT_FALSE(c_algo_mismatch(&l_first, &l_last, &v_first, &l_found, &v_found));
    EXPECT_TRUE(C_ITER_EQ(&l_last, l_found));
    EXPECT_TRUE(C_ITER_EQ(&v_last, v_found));

    EXPECT_TRUE(c_algo_mismatch(&l_first, &l_last, &fl_first, &l_found, &fl_found));
    EXPECT_TRUE(C_ITER_EQ(&l_first, l_found));
    EXPECT_TRUE(C_ITER_EQ(&fl_first, fl_found));
}

TEST_F(CNonModifyingTest, Equal)
{
    SetupAll(default_data, default_length);

    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &v_first));
    EXPECT_FALSE(c_algo_equal(&l_first, &l_last, &fl_first));

    EXPECT_FALSE(c_algo_equal(&fl_first, &fl_last, &l_first));
    EXPECT_FALSE(c_algo_equal(&fl_first, &fl_last, &v_first));

    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &l_first));
    EXPECT_FALSE(c_algo_equal(&v_first, &v_last, &fl_first));

    c_list_reverse(__l);
    l_first = c_list_begin(__l);

    EXPECT_FALSE(c_algo_equal(&l_first, &l_last, &v_first));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &fl_first));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &l_first));
    EXPECT_FALSE(c_algo_equal(&v_first, &v_last, &l_first));
}

TEST_F(CNonModifyingTest, Find)
{
    SetupAll(default_data, default_length);

    for (int i : default_data) {
        EXPECT_TRUE(c_algo_find(&l_first, &l_last, &l_found, &i));
        EXPECT_EQ(i, C_DEREF_INT(C_ITER_DEREF(l_found)));

        EXPECT_TRUE(c_algo_find(&fl_first, &fl_last, &fl_found, &i));
        EXPECT_EQ(i, C_DEREF_INT(C_ITER_DEREF(fl_found)));

        EXPECT_TRUE(c_algo_find(&v_first, &v_last, &v_found, &i));
        EXPECT_EQ(i, C_DEREF_INT(C_ITER_DEREF(v_found)));
    }

    int not_exist = 10;
    EXPECT_FALSE(c_algo_find(&l_first, &l_last, &l_found, &not_exist));
    EXPECT_TRUE(C_ITER_EQ(&l_last, l_found));
    EXPECT_FALSE(c_algo_find(&fl_first, &fl_last, &fl_found, &not_exist));
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
    EXPECT_FALSE(c_algo_find(&v_first, &v_last, &v_found, &not_exist));
    EXPECT_TRUE(C_ITER_EQ(&v_last, v_found));

    int exist = 0;
    EXPECT_FALSE(c_algo_find(&l_first, &l_first, &l_found, &exist));
    EXPECT_FALSE(c_algo_find(&fl_first, &fl_first, &fl_found, &exist));
    EXPECT_FALSE(c_algo_find(&v_first, &v_first, &v_found, &exist));
}

TEST_F(CNonModifyingTest, FindIf)
{
    SetupAll(default_data, default_length);

    // if
    EXPECT_TRUE(c_algo_find_if(&l_first, &l_last, &l_found, greater_than_two));
    EXPECT_EQ(3, C_DEREF_INT(C_ITER_DEREF(l_found)));
    EXPECT_TRUE(c_algo_find_if(&fl_first, &fl_last, &fl_found, greater_than_two));
    EXPECT_EQ(9, C_DEREF_INT(C_ITER_DEREF(fl_found)));
    EXPECT_TRUE(c_algo_find_if(&v_first, &v_last, &v_found, greater_than_two));
    EXPECT_EQ(3, C_DEREF_INT(C_ITER_DEREF(v_found)));

    EXPECT_FALSE(c_algo_find_if(&l_first, &l_last, &l_found, greater_than_ten));
    EXPECT_TRUE(C_ITER_EQ(&l_last, l_found));
    EXPECT_FALSE(c_algo_find_if(&fl_first, &fl_last, &fl_found, greater_than_ten));
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
    EXPECT_FALSE(c_algo_find_if(&v_first, &v_last, &v_found, greater_than_ten));
    EXPECT_TRUE(C_ITER_EQ(&v_last, v_found));

    EXPECT_FALSE(c_algo_find_if(&l_first, &l_first, &l_found, greater_than_two));
    EXPECT_FALSE(c_algo_find_if(&fl_first, &fl_first, &fl_found, greater_than_two));
    EXPECT_FALSE(c_algo_find_if(&v_first, &v_first, &v_found, greater_than_two));

    // if not
    EXPECT_TRUE(c_algo_find_if_not(&l_first, &l_last, &l_found, greater_than_two));
    EXPECT_EQ(0, C_DEREF_INT(C_ITER_DEREF(l_found)));
    EXPECT_TRUE(c_algo_find_if_not(&fl_first, &fl_last, &fl_found, greater_than_two));
    EXPECT_EQ(2, C_DEREF_INT(C_ITER_DEREF(fl_found)));
    EXPECT_TRUE(c_algo_find_if_not(&v_first, &v_last, &v_found, greater_than_two));
    EXPECT_EQ(0, C_DEREF_INT(C_ITER_DEREF(v_found)));

    EXPECT_FALSE(c_algo_find_if_not(&l_first, &l_last, &l_found, no_less_than_zero));
    EXPECT_TRUE(C_ITER_EQ(&l_last, l_found));
    EXPECT_FALSE(c_algo_find_if_not(&fl_first, &fl_last, &fl_found, no_less_than_zero));
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
    EXPECT_FALSE(c_algo_find_if_not(&v_first, &v_last, &v_found, no_less_than_zero));
    EXPECT_TRUE(C_ITER_EQ(&v_last, v_found));

    EXPECT_FALSE(c_algo_find_if_not(&l_first, &l_first, &l_found, greater_than_two));
    EXPECT_FALSE(c_algo_find_if_not(&fl_first, &fl_first, &fl_found, greater_than_two));
    EXPECT_FALSE(c_algo_find_if_not(&v_first, &v_first, &v_found, greater_than_two));
}

TEST_F(CNonModifyingTest, FindFirstOf)
{
    SetupAll(default_data, default_length);
    int numbers[] = { 10, 11, 12, 3, 4, 5, 6 };
    c_list_t* seq = c_list_create_from(c_get_int_type_info(), C_REF_T(numbers), __array_length(numbers));
    c_list_iterator_t s_first = c_list_begin(seq);
    c_list_iterator_t s_last = c_list_end(seq);

    EXPECT_TRUE(c_algo_find_first_of(&l_first, &l_last, &s_first, &s_last, &l_found));
    EXPECT_EQ(3, C_DEREF_INT(C_ITER_DEREF(l_found)));
    EXPECT_TRUE(c_algo_find_first_of(&fl_first, &fl_last, &s_first, &s_last, &fl_found));
    EXPECT_EQ(6, C_DEREF_INT(C_ITER_DEREF(fl_found)));
    EXPECT_TRUE(c_algo_find_first_of(&v_first, &v_last, &s_first, &s_last, &v_found));
    EXPECT_EQ(3, C_DEREF_INT(C_ITER_DEREF(v_found)));

    C_ITER_ASSIGN(&s_last, &s_first);
    C_ITER_INC(&s_last);
    EXPECT_FALSE(c_algo_find_first_of(&l_first, &l_last, &s_first, &s_last, &l_found));
    EXPECT_TRUE(C_ITER_EQ(&l_last, l_found));
    EXPECT_FALSE(c_algo_find_first_of(&fl_first, &fl_last, &s_first, &s_last, &fl_found));
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
    EXPECT_FALSE(c_algo_find_first_of(&v_first, &v_last, &s_first, &s_last, &v_found));
    EXPECT_TRUE(C_ITER_EQ(&v_last, v_found));

    EXPECT_FALSE(c_algo_find_first_of(&l_first, &l_first, &s_first, &s_last, &l_found));
    EXPECT_TRUE(C_ITER_EQ(&l_first, l_found));
    EXPECT_FALSE(c_algo_find_first_of(&fl_first, &fl_first, &s_first, &s_last, &fl_found));
    EXPECT_TRUE(C_ITER_EQ(&fl_first, fl_found));
    EXPECT_FALSE(c_algo_find_first_of(&v_first, &v_first, &s_first, &s_last, &v_found));
    EXPECT_TRUE(C_ITER_EQ(&v_first, v_found));

    c_list_destroy(seq);
}

TEST_F(CNonModifyingTest, FindLastOf)
{
    SetupAll(default_data, default_length);
    int numbers[] = { 10, 11, 12, 3, 4, 5, 6 };
    c_list_t* seq = c_list_create_from(c_get_int_type_info(), C_REF_T(numbers), __array_length(numbers));
    c_list_iterator_t s_first = c_list_begin(seq);
    c_list_iterator_t s_last = c_list_end(seq);

    EXPECT_TRUE(c_algo_find_last_of(&l_first, &l_last, &s_first, &s_last, &l_found));
    EXPECT_EQ(6, C_DEREF_INT(C_ITER_DEREF(l_found)));
    EXPECT_TRUE(c_algo_find_last_of(&fl_first, &fl_last, &s_first, &s_last, &fl_found));
    EXPECT_EQ(3, C_DEREF_INT(C_ITER_DEREF(fl_found)));
    EXPECT_TRUE(c_algo_find_last_of(&v_first, &v_last, &s_first, &s_last, &v_found));
    EXPECT_EQ(6, C_DEREF_INT(C_ITER_DEREF(v_found)));

    C_ITER_ASSIGN(&s_last, &s_first);
    C_ITER_INC(&s_last);
    EXPECT_FALSE(c_algo_find_last_of(&l_first, &l_last, &s_first, &s_last, &l_found));
    EXPECT_TRUE(C_ITER_EQ(&l_last, l_found));
    EXPECT_FALSE(c_algo_find_last_of(&fl_first, &fl_last, &s_first, &s_last, &fl_found));
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
    EXPECT_FALSE(c_algo_find_last_of(&v_first, &v_last, &s_first, &s_last, &v_found));
    EXPECT_TRUE(C_ITER_EQ(&v_last, v_found));

    EXPECT_FALSE(c_algo_find_last_of(&l_first, &l_first, &s_first, &s_last, &l_found));
    EXPECT_TRUE(C_ITER_EQ(&l_first, l_found));
    EXPECT_FALSE(c_algo_find_last_of(&fl_first, &fl_first, &s_first, &s_last, &fl_found));
    EXPECT_TRUE(C_ITER_EQ(&fl_first, fl_found));
    EXPECT_FALSE(c_algo_find_last_of(&v_first, &v_first, &s_first, &s_last, &v_found));
    EXPECT_TRUE(C_ITER_EQ(&v_first, v_found));

    c_list_destroy(seq);
}

TEST_F(CNonModifyingTest, AdjacentFind)
{
    int numbers[] = { 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4 };
    SetupAll(numbers, __array_length(numbers));

    EXPECT_TRUE(c_algo_adjacent_find(&l_first, &l_last, &l_found));
    EXPECT_EQ(2, C_DEREF_INT(C_ITER_DEREF(l_found)));
    EXPECT_TRUE(c_algo_adjacent_find(&fl_first, &fl_last, &fl_found));
    EXPECT_EQ(4, C_DEREF_INT(C_ITER_DEREF(fl_found)));
    EXPECT_TRUE(c_algo_adjacent_find(&v_first, &v_last, &v_found));
    EXPECT_EQ(2, C_DEREF_INT(C_ITER_DEREF(v_found)));

    TearDown();
    SetUp();

    SetupAll(default_data, default_length);
    EXPECT_FALSE(c_algo_adjacent_find(&l_first, &l_last, &l_found));
    EXPECT_FALSE(c_algo_adjacent_find(&fl_first, &fl_last, &fl_found));
    EXPECT_FALSE(c_algo_adjacent_find(&v_first, &v_last, &v_found));

    EXPECT_FALSE(c_algo_adjacent_find(&l_first, &l_first, &l_found));
    EXPECT_FALSE(c_algo_adjacent_find(&fl_first, &fl_first, &fl_found));
    EXPECT_FALSE(c_algo_adjacent_find(&v_first, &v_first, &v_found));
}

TEST_F(CNonModifyingTest, Search)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 3, 4, 5, 6 };
    c_list_t* seq = c_list_create_from(c_get_int_type_info(), C_REF_T(numbers), __array_length(numbers));
    c_list_iterator_t s_first = c_list_begin(seq);
    c_list_iterator_t s_last = c_list_end(seq);

    EXPECT_TRUE(c_algo_search(&l_first, &l_last, &s_first, &s_last, &l_found));
    EXPECT_EQ(3, C_DEREF_INT(C_ITER_DEREF(l_found)));
    EXPECT_FALSE(c_algo_search(&fl_first, &fl_last, &s_first, &s_last, &fl_found));
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
    EXPECT_TRUE(c_algo_search(&v_first, &v_last, &s_first, &s_last, &v_found));
    EXPECT_EQ(3, C_DEREF_INT(C_ITER_DEREF(v_found)));

    c_list_destroy(seq);
}

TEST_F(CNonModifyingTest, SearchLast)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 3, 4, 5, 6 };
    c_list_t* seq = c_list_create_from(c_get_int_type_info(), C_REF_T(numbers), __array_length(numbers));
    c_list_iterator_t s_first = c_list_begin(seq);
    c_list_iterator_t s_last = c_list_end(seq);

    EXPECT_TRUE(c_algo_search_last(&l_first, &l_last, &s_first, &s_last, &l_found));
    EXPECT_EQ(3, C_DEREF_INT(C_ITER_DEREF(l_found)));
    EXPECT_FALSE(c_algo_search_last(&fl_first, &fl_last, &s_first, &s_last, &fl_found));
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
    EXPECT_TRUE(c_algo_search_last(&v_first, &v_last, &s_first, &s_last, &v_found));
    EXPECT_EQ(3, C_DEREF_INT(C_ITER_DEREF(v_found)));

    c_list_destroy(seq);
}

TEST_F(CNonModifyingTest, SearchN)
{
    int numbers[] = { 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4 };
    SetupAll(numbers, __array_length(numbers));

    for (int i : { 1, 2, 3, 4 } ) {
        EXPECT_TRUE(c_algo_search_n(&l_first, &l_last, (size_t)i, &i, &l_found));
        EXPECT_EQ(i, C_DEREF_INT(C_ITER_DEREF(l_found)));
        EXPECT_TRUE(c_algo_search_n(&fl_first, &fl_last, (size_t)i, &i, &fl_found));
        EXPECT_EQ(i, C_DEREF_INT(C_ITER_DEREF(fl_found)));
        EXPECT_TRUE(c_algo_search_n(&v_first, &v_last, (size_t)i, &i, &v_found));
        EXPECT_EQ(i, C_DEREF_INT(C_ITER_DEREF(v_found)));

        EXPECT_FALSE(c_algo_search_n(&l_first, &l_last, 0, &i, &l_found));
        EXPECT_TRUE(C_ITER_EQ(&l_last, l_found));
        EXPECT_FALSE(c_algo_search_n(&fl_first, &fl_last, 0, &i, &fl_found));
        EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
        EXPECT_FALSE(c_algo_search_n(&v_first, &v_last, 0, &i, &v_found));
        EXPECT_TRUE(C_ITER_EQ(&v_last, v_found));

        EXPECT_FALSE(c_algo_search_n(&l_first, &l_first, (size_t)i, &i, &l_found));
        EXPECT_TRUE(C_ITER_EQ(&l_first, l_found));
        EXPECT_FALSE(c_algo_search_n(&fl_first, &fl_first, (size_t)i, &i, &fl_found));
        EXPECT_TRUE(C_ITER_EQ(&fl_first, fl_found));
        EXPECT_FALSE(c_algo_search_n(&v_first, &v_first, (size_t)i, &i, &v_found));
        EXPECT_TRUE(C_ITER_EQ(&v_first, v_found));
    }

    int not_exist = 5;
    EXPECT_FALSE(c_algo_search_n(&l_first, &l_last, 0, &not_exist, &l_found));
    EXPECT_TRUE(C_ITER_EQ(&l_last, l_found));
    EXPECT_FALSE(c_algo_search_n(&fl_first, &fl_last, 0, &not_exist, &fl_found));
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
    EXPECT_FALSE(c_algo_search_n(&v_first, &v_last, 0, &not_exist, &v_found));
    EXPECT_TRUE(C_ITER_EQ(&v_last, v_found));

    int exist = 1;
    EXPECT_FALSE(c_algo_search_n(&l_first, &l_last, 10, &exist, &l_found));
    EXPECT_TRUE(C_ITER_EQ(&l_last, l_found));
    EXPECT_FALSE(c_algo_search_n(&fl_first, &fl_last, 10, &exist, &fl_found));
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_found));
    EXPECT_FALSE(c_algo_search_n(&v_first, &v_last, 10, &exist, &v_found));
    EXPECT_TRUE(C_ITER_EQ(&v_last, v_found));
}

} // namespace
} // namespace c_container
