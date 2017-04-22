
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

void plus_one(c_ref_t value)
{
    ++(*((int*)value));
}

#pragma GCC diagnostic ignored "-Weffc++"
class CModifyingTest : public ::testing::Test
{
public:
    CModifyingTest() : __fl(0), __l(0), __v(0), l_output(0), fl_output(0), v_output(0) {}
    ~CModifyingTest() {
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
        c_slist_iterator_t iter = c_slist_before_begin(__fl);
        for (int i = 0; i < length; ++i)
             iter = c_slist_insert_after(__fl, iter, C_REF_T(&datas[i]));

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
        __c_free(fl_output);
        fl_output = 0;

        c_list_destroy(__l);
        __l = 0;
        __c_free(l_output);
        l_output = 0;

        c_vector_destroy(__v);
        __v = 0;
        __c_free(v_output);
        v_output = 0;
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

    c_iterator_t* l_output;
    c_iterator_t* fl_output;
    c_iterator_t* v_output;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CModifyingTest, CopyBackward)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 10, 11, 12, 13, 14, 15 };
    c_list_t* from = c_list_create_from(c_get_int_type_info(), (c_ref_t)numbers, __array_length(numbers));
    c_list_iterator_t from_first = c_list_begin(from);
    c_list_iterator_t from_last = c_list_end(from);

    EXPECT_EQ(__array_length(numbers), c_algo_copy_backward(&from_first, &from_last, &l_last, &l_output));
    EXPECT_TRUE(c_algo_equal(&from_first, &from_last, l_output));

    EXPECT_EQ(__array_length(numbers), c_algo_copy_backward(&from_first, &from_last, &v_last, &v_output));
    EXPECT_TRUE(c_algo_equal(&from_first, &from_last, v_output));

    c_list_destroy(from);
}

TEST_F(CModifyingTest, Fill)
{
    SetupAll(default_data, default_length);

    int to_be_filled[default_length] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
    c_list_t* filled = c_list_create_from(c_get_int_type_info(), (c_ref_t)to_be_filled, default_length);
    c_list_iterator_t filled_first = c_list_begin(filled);

    EXPECT_EQ(default_length, c_algo_fill(&l_first, &l_last, &to_be_filled[0]));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &filled_first));

    EXPECT_EQ(default_length, c_algo_fill(&fl_first, &fl_last, &to_be_filled[0]));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &filled_first));

    EXPECT_EQ(default_length, c_algo_fill(&v_first, &v_last, &to_be_filled[0]));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &filled_first));

    c_list_destroy(filled);
}

TEST_F(CModifyingTest, FillN)
{
    SetupAll(default_data, default_length);

    int to_be_filled[default_length] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
    c_list_t* filled = c_list_create_from(c_get_int_type_info(), (c_ref_t)to_be_filled, default_length);
    c_list_iterator_t filled_first = c_list_begin(filled);

    c_algo_fill_n(&l_first, default_length, &to_be_filled[0], &l_output);
    EXPECT_TRUE(C_ITER_EQ(&l_last, l_output));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &filled_first));

    c_algo_fill_n(&fl_first, default_length, &to_be_filled[0], &fl_output);
    EXPECT_TRUE(C_ITER_EQ(&fl_last, fl_output));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &filled_first));

    c_algo_fill_n(&v_first, default_length, &to_be_filled[0], &v_output);
    EXPECT_TRUE(C_ITER_EQ(&v_last, v_output));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &filled_first));

    c_list_destroy(filled);
}

TEST_F(CModifyingTest, Swap)
{
    int x = 100, y = 200;
    c_algo_swap(c_get_int_type_info(), &x, &y);
    EXPECT_EQ(100, y);
    EXPECT_EQ(200, x);
}

TEST_F(CModifyingTest, IterSwap)
{
    SetupAll(default_data, default_length);

    c_list_iterator_t* l_next = 0;
    C_ITER_COPY(&l_next, &l_first);
    C_ITER_INC(l_next);
    int l_first_value = C_DEREF_INT(C_ITER_DEREF(&l_first));
    int l_next_value = C_DEREF_INT(C_ITER_DEREF(l_next));
    c_algo_iter_swap(&l_first, l_next);
    EXPECT_EQ(l_first_value, C_DEREF_INT(C_ITER_DEREF(l_next)));
    EXPECT_EQ(l_next_value, C_DEREF_INT(C_ITER_DEREF(&l_first)));
    __c_free(l_next);

    c_slist_iterator_t* fl_next = 0;
    C_ITER_COPY(&fl_next, &fl_first);
    C_ITER_INC(fl_next);
    int fl_first_value = C_DEREF_INT(C_ITER_DEREF(&fl_first));
    int fl_next_value = C_DEREF_INT(C_ITER_DEREF(fl_next));
    c_algo_iter_swap(&fl_first, fl_next);
    EXPECT_EQ(fl_first_value, C_DEREF_INT(C_ITER_DEREF(fl_next)));
    EXPECT_EQ(fl_next_value, C_DEREF_INT(C_ITER_DEREF(&fl_first)));
    __c_free(fl_next);

    c_list_iterator_t* v_next = 0;
    C_ITER_COPY(&v_next, &v_first);
    C_ITER_INC(v_next);
    int v_first_value = C_DEREF_INT(C_ITER_DEREF(&v_first));
    int v_next_value = C_DEREF_INT(C_ITER_DEREF(v_next));
    c_algo_iter_swap(&v_first, v_next);
    EXPECT_EQ(v_first_value, C_DEREF_INT(C_ITER_DEREF(v_next)));
    EXPECT_EQ(v_next_value, C_DEREF_INT(C_ITER_DEREF(&v_first)));
    __c_free(v_next);
}

TEST_F(CModifyingTest, Transform)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), (c_ref_t)numbers, __array_length(numbers));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(default_length, c_algo_transform(&l_first, &l_last, &l_first, plus_one));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));

    EXPECT_EQ(default_length, c_algo_transform(&fl_first, &fl_last, &fl_first, plus_one));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));

    EXPECT_EQ(default_length, c_algo_transform(&v_first, &v_last, &v_first, plus_one));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));

    c_list_destroy(expected);
}

} // namespace
} // namespace c_container
