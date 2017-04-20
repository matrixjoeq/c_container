
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

#pragma GCC diagnostic ignored "-Weffc++"
class CNonModifyingTest : public ::testing::Test
{
public:
    CNonModifyingTest() : __fl(0), __l(0), __v(0) {}
    ~CNonModifyingTest() {
        c_slist_destroy(__fl);
        c_list_destroy(__l);
        c_vector_destroy(__v);
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
    }

    void SetupForwardList(const int *datas, int length)
    {
        c_slist_iterator_t before_begin = c_slist_before_begin(__fl);
        for (int i = 0; i < length; ++i)
            c_slist_insert_after(__fl, before_begin, C_REF_T(&datas[i]));
    }

    void SetupVector(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_vector_push_back(__v, C_REF_T(&datas[i]));
    }

    void SetUp()
    {
        __fl = C_SLIST_INT;
        __l = C_LIST_INT;
        __v = C_VECTOR_INT;
    }

    void TearDown()
    {
        c_slist_destroy(__fl);
        __fl = 0;

        c_list_destroy(__l);
        __l = 0;

        c_vector_destroy(__v);
        __v = 0;
    }

protected:
    c_slist_t* __fl;
    c_list_t* __l;
    c_vector_t* __v;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CNonModifyingTest, AllAnyNone)
{
    SetupAll(default_data, default_length);

    c_list_iterator_t l_first = c_list_begin(__l);
    c_list_iterator_t l_last = c_list_end(__l);
    c_slist_iterator_t fl_first = c_slist_begin(__fl);
    c_slist_iterator_t fl_last = c_slist_end(__fl);
    c_vector_iterator_t v_first = c_vector_begin(__v);
    c_vector_iterator_t v_last = c_vector_end(__v);

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

TEST_F(CNonModifyingTest, Count)
{
    int numbers[] = { 1, 2, 2, 3, 3, 3, 4, 4, 4, 4 };
    SetupAll(numbers, __array_length(numbers));

    c_list_iterator_t l_first = c_list_begin(__l);
    c_list_iterator_t l_last = c_list_end(__l);
    c_slist_iterator_t fl_first = c_slist_begin(__fl);
    c_slist_iterator_t fl_last = c_slist_end(__fl);
    c_vector_iterator_t v_first = c_vector_begin(__v);
    c_vector_iterator_t v_last = c_vector_end(__v);
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

} // namespace
} // namespace c_container
