
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
int generate_seed;

void plus_one(c_ref_t value)
{
    ++(*((int*)value));
}

bool greater_than_two(c_ref_t value)
{
    return (*((int*)value) > 2);
}

void print_value(c_ref_t data)
{
    printf("%d ", C_DEREF_INT(data));
}

void generate_number(c_ref_t address)
{
    if (!address) return;
    *(int*)address = generate_seed++;
}

inline void print_newline(void)
{
    printf("\n");
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

TEST_F(CModifyingTest, Copy)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 10, 11, 12, 13, 14, 15 };
    c_list_t* from = c_list_create_from(c_get_int_type_info(), (c_ref_t)numbers, __array_length(numbers));
    c_list_iterator_t from_first = c_list_begin(from);
    c_list_iterator_t from_last = c_list_end(from);

    EXPECT_EQ(__array_length(numbers), c_algo_copy(&from_first, &from_last, &l_first, &l_output));
    EXPECT_TRUE(c_algo_equal(&from_first, &from_last, &l_first));
    EXPECT_EQ(__array_length(numbers), C_ITER_DISTANCE(&l_first, l_output));

    EXPECT_EQ(__array_length(numbers), c_algo_copy(&from_first, &from_last, &fl_first, &fl_output));
    EXPECT_TRUE(c_algo_equal(&from_first, &from_last, &fl_first));
    EXPECT_EQ(__array_length(numbers), C_ITER_DISTANCE(&fl_first, fl_output));

    EXPECT_EQ(__array_length(numbers), c_algo_copy(&from_first, &from_last, &v_first, &v_output));
    EXPECT_TRUE(c_algo_equal(&from_first, &from_last, &v_first));
    EXPECT_EQ(__array_length(numbers), C_ITER_DISTANCE(&v_first, v_output));

    c_list_destroy(from);
}

TEST_F(CModifyingTest, CopyIf)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 0, 1, 2, 3, 4, 5 };
    c_list_t* from = c_list_create_from(c_get_int_type_info(), (c_ref_t)numbers, __array_length(numbers));
    c_list_iterator_t from_first = c_list_begin(from);
    c_list_iterator_t from_last = c_list_end(from);

    int expect_numbers[] = { 3, 4, 5 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), (c_ref_t)expect_numbers, __array_length(expect_numbers));
    c_list_iterator_t e_first = c_list_begin(expected);
    c_list_iterator_t e_last = c_list_end(expected);

    EXPECT_EQ(__array_length(expect_numbers), c_algo_copy_if(&from_first, &from_last, &l_first, &l_output, greater_than_two));
    EXPECT_TRUE(c_algo_equal(&e_first, &e_last, &l_first));
    EXPECT_EQ(__array_length(expect_numbers), C_ITER_DISTANCE(&l_first, l_output));

    EXPECT_EQ(__array_length(expect_numbers), c_algo_copy_if(&from_first, &from_last, &fl_first, &fl_output, greater_than_two));
    EXPECT_TRUE(c_algo_equal(&e_first, &e_last, &fl_first));
    EXPECT_EQ(__array_length(expect_numbers), C_ITER_DISTANCE(&fl_first, fl_output));

    EXPECT_EQ(__array_length(expect_numbers), c_algo_copy_if(&from_first, &from_last, &v_first, &v_output, greater_than_two));
    EXPECT_TRUE(c_algo_equal(&e_first, &e_last, &v_first));
    EXPECT_EQ(__array_length(expect_numbers), C_ITER_DISTANCE(&v_first, v_output));

    c_list_destroy(from);
    c_list_destroy(expected);
}

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

TEST_F(CModifyingTest, Generate)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 100, 101, 102, 103, 104, 105, 106, 107, 108, 109 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), (c_ref_t)numbers, __array_length(numbers));
    c_list_iterator_t e_first = c_list_begin(expected);

    generate_seed = 100;
    EXPECT_EQ(default_length, c_algo_generate(&l_first, &l_last, generate_number));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));

    generate_seed = 100;
    EXPECT_EQ(default_length, c_algo_generate(&fl_first, &fl_last, generate_number));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));

    generate_seed = 100;
    EXPECT_EQ(default_length, c_algo_generate(&v_first, &v_last, generate_number));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, GenerateN)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 100, 101, 102, 103, 104, 105, 106, 107, 108, 109 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), (c_ref_t)numbers, __array_length(numbers));
    c_list_iterator_t e_first = c_list_begin(expected);

    generate_seed = 100;
    c_algo_generate_n(&l_first, default_length, generate_number, &l_output);
    EXPECT_TRUE(C_ITER_EQ(l_output, &l_last));
    EXPECT_TRUE(c_algo_equal(&l_first, l_output, &e_first));

    generate_seed = 100;
    c_algo_generate_n(&fl_first, default_length, generate_number, &fl_output);
    EXPECT_TRUE(C_ITER_EQ(fl_output, &fl_last));
    EXPECT_TRUE(c_algo_equal(&fl_first, fl_output, &e_first));

    generate_seed = 100;
    c_algo_generate_n(&v_first, default_length, generate_number, &v_output);
    EXPECT_TRUE(C_ITER_EQ(v_output, &v_last));
    EXPECT_TRUE(c_algo_equal(&v_first, v_output, &e_first));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, Remove)
{
    int numbers[] = { 0, 1, 1, 2, 1, 3, 4, 1, 1, 1, 5 };
    SetupAll(numbers, __array_length(numbers));

    int to_be_removed = 1;
    int removed[] = { 0, 2, 3, 4, 5, 3, 4, 1, 1, 1, 5 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), C_REF_T(removed), __array_length(removed));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(6, c_algo_remove(&l_first, &l_last, &to_be_removed, &l_output));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));

    EXPECT_EQ(6, c_algo_remove(&fl_first, &fl_last, &to_be_removed, &fl_output));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));

    EXPECT_EQ(6, c_algo_remove(&v_first, &v_last, &to_be_removed, &v_output));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, RemoveIf)
{
    int numbers[] = { 0, 1, 1, 2, 1, 3, 4, 1, 1, 1, 5 };
    SetupAll(numbers, __array_length(numbers));

    int removed[] = { 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 5 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), C_REF_T(removed), __array_length(removed));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(3, c_algo_remove_if(&l_first, &l_last, &l_output, greater_than_two));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));

    EXPECT_EQ(3, c_algo_remove_if(&fl_first, &fl_last, &fl_output, greater_than_two));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));

    EXPECT_EQ(3, c_algo_remove_if(&v_first, &v_last, &v_output, greater_than_two));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, RemoveCopy)
{
    int numbers[] = { 0, 1, 1, 2, 1, 3, 4, 1, 1, 1, 5 };
    SetupAll(numbers, __array_length(numbers));

    int to_be_removed = 1;
    int copied[] = { 0, 2, 3, 4, 5, 3, 4, 1, 1, 1, 5 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), C_REF_T(copied), __array_length(copied));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(5, c_algo_remove_copy(&l_first, &l_last, &fl_first, &to_be_removed, &fl_output));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));
    EXPECT_EQ(5, C_ITER_DISTANCE(&fl_first, fl_output));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, RemoveCopyIf)
{
    int numbers[] = { 0, 1, 1, 2, 1, 3, 4, 1, 1, 1, 5 };
    SetupAll(numbers, __array_length(numbers));

    int removed[] = { 0, 1, 1, 2, 1, 1, 1, 1, 1, 1, 5 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), C_REF_T(removed), __array_length(removed));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(8, c_algo_remove_copy_if(&l_first, &l_last, &fl_first, &fl_output, greater_than_two));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));
    EXPECT_EQ(8, C_ITER_DISTANCE(&fl_first, fl_output));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, Replace)
{
    int numbers[] = { 0, 1, 1, 2, 1, 3, 4, 1, 1, 5, 1 };
    SetupAll(numbers, __array_length(numbers));

    int old_value = 1;
    int new_value = 10;
    int replaced[] = { 0, 10, 10, 2, 10, 3, 4, 10, 10, 5, 10 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), replaced, __array_length(replaced));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(6, c_algo_replace(&l_first, &l_last, &old_value, &new_value));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));

    EXPECT_EQ(6, c_algo_replace(&fl_first, &fl_last, &old_value, &new_value));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));

    EXPECT_EQ(6, c_algo_replace(&v_first, &v_last, &old_value, &new_value));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, ReplaceIf)
{
    int numbers[] = { 0, 1, 1, 2, 1, 3, 4, 1, 1, 5, 6 };
    SetupAll(numbers, __array_length(numbers));

    int new_value = 10;
    int replaced[] = { 0, 1, 1, 2, 1, 10, 10, 1, 1, 10, 10 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), replaced, __array_length(replaced));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(4, c_algo_replace_if(&l_first, &l_last, greater_than_two, &new_value));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));

    EXPECT_EQ(4, c_algo_replace_if(&fl_first, &fl_last, greater_than_two, &new_value));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));

    EXPECT_EQ(4, c_algo_replace_if(&v_first, &v_last, greater_than_two, &new_value));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, ReplaceCopy)
{
    int numbers[] = { 0, 1, 1, 2, 1, 3, 4, 1, 1, 5, 1 };
    SetupAll(numbers, __array_length(numbers));

    int old_value = 1;
    int new_value = 10;
    int replaced[] = { 0, 10, 10, 2, 10, 3, 4, 10, 10, 5, 10 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), replaced, __array_length(replaced));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(11, c_algo_replace_copy(&l_first, &l_last, &fl_first, &old_value, &new_value, &fl_output));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));
    EXPECT_EQ(11, C_ITER_DISTANCE(&fl_first, fl_output));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, ReplaceCopyIf)
{
    int numbers[] = { 0, 1, 1, 2, 1, 3, 4, 1, 1, 5, 6 };
    SetupAll(numbers, __array_length(numbers));

    int new_value = 10;
    int replaced[] = { 0, 1, 1, 2, 1, 10, 10, 1, 1, 10, 10 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), replaced, __array_length(replaced));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(11, c_algo_replace_copy_if(&l_first, &l_last, &fl_first, greater_than_two, &new_value, &fl_output));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));
    EXPECT_EQ(11, C_ITER_DISTANCE(&fl_first, fl_output));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, Swap)
{
    int x = 100, y = 200;
    c_algo_swap(c_get_int_type_info(), &x, &y);
    EXPECT_EQ(100, y);
    EXPECT_EQ(200, x);
}

TEST_F(CModifyingTest, SwapRange)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 5, 6, 7, 8, 9, 0, 1, 2, 3, 4 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), numbers, __array_length(numbers));
    c_list_iterator_t e_first = c_list_begin(expected);

    c_list_iterator_t* l_first2 = 0;
    C_ITER_COPY(&l_first2, &l_first);
    C_ITER_ADVANCE(l_first2, default_length / 2);
    EXPECT_EQ(5, C_DEREF_INT(C_ITER_DEREF(l_first2)));
    EXPECT_EQ(default_length / 2, c_algo_swap_range(&l_first, l_first2, l_first2));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));
    __c_free(l_first2);

    c_list_iterator_t* fl_first2 = 0;
    C_ITER_COPY(&fl_first2, &fl_first);
    C_ITER_ADVANCE(fl_first2, default_length / 2);
    EXPECT_EQ(default_length / 2, c_algo_swap_range(&fl_first, fl_first2, fl_first2));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));
    __c_free(fl_first2);

    c_list_iterator_t* v_first2 = 0;
    C_ITER_COPY(&v_first2, &v_first);
    C_ITER_ADVANCE(v_first2, default_length / 2);
    EXPECT_EQ(default_length / 2, c_algo_swap_range(&v_first, v_first2, v_first2));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));
    __c_free(v_first2);

    c_list_destroy(expected);
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

TEST_F(CModifyingTest, Reverse)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), numbers, __array_length(numbers));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(default_length, c_algo_reverse(&l_first, &l_last));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));

    EXPECT_EQ(default_length, c_algo_reverse(&v_first, &v_last));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, ReverseCopy)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), numbers, __array_length(numbers));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(default_length, c_algo_reverse_copy(&l_first, &l_last, &fl_first, &fl_output));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));
    EXPECT_TRUE(C_ITER_EQ(fl_output, &fl_last));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, Rotate)
{
}

TEST_F(CModifyingTest, RotateCopy)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 5, 6, 7, 8, 9, 0, 1, 2, 3, 4 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), numbers, __array_length(numbers));
    c_list_iterator_t e_first = c_list_begin(expected);

    c_list_iterator_t n_first = c_list_begin(__l);
    C_ITER_ADVANCE(&n_first, default_length / 2);

    EXPECT_EQ(default_length, c_algo_rotate_copy(&l_first, &n_first, &l_last, &fl_first, &fl_output));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));
    EXPECT_EQ(default_length, C_ITER_DISTANCE(&fl_first, fl_output));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, Unique)
{
    int numbers[] = { 0, 1, 1, 2, 1, 3, 4, 1, 1, 5, 1 };
    SetupAll(numbers, __array_length(numbers));

    int uniqued[] = { 0, 1, 2, 1, 3, 4, 1, 5, 1, 5, 1 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), uniqued, __array_length(uniqued));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(9, c_algo_unique(&l_first, &l_last, &l_output));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));
    EXPECT_EQ(9, C_ITER_DISTANCE(&l_first, l_output));

    EXPECT_EQ(9, c_algo_unique(&fl_first, &fl_last, &fl_output));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));
    EXPECT_EQ(9, C_ITER_DISTANCE(&fl_first, fl_output));

    EXPECT_EQ(9, c_algo_unique(&v_first, &v_last, &v_output));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));
    EXPECT_EQ(9, C_ITER_DISTANCE(&v_first, v_output));

    c_list_destroy(expected);
}

TEST_F(CModifyingTest, UniqueCopy)
{
    SetupAll(default_data, default_length);

    int numbers[] = { 0, 1, 1, 2, 1, 1, 1, 5, 1 };
    c_list_t* from = c_list_create_from(c_get_int_type_info(), numbers, __array_length(numbers));
    c_list_iterator_t from_first = c_list_begin(from);
    c_list_iterator_t from_last = c_list_end(from);

    int uniqued[] = { 0, 1, 2, 1, 5, 1, 6, 7, 8, 9 };
    c_list_t* expected = c_list_create_from(c_get_int_type_info(), uniqued, __array_length(uniqued));
    c_list_iterator_t e_first = c_list_begin(expected);

    EXPECT_EQ(6, c_algo_unique_copy(&from_first, &from_last, &l_first, &l_output));
    EXPECT_TRUE(c_algo_equal(&l_first, &l_last, &e_first));
    EXPECT_EQ(6, C_ITER_DISTANCE(&l_first, l_output));

    EXPECT_EQ(6, c_algo_unique_copy(&from_first, &from_last, &fl_first, &fl_output));
    EXPECT_TRUE(c_algo_equal(&fl_first, &fl_last, &e_first));
    EXPECT_EQ(6, C_ITER_DISTANCE(&fl_first, fl_output));

    EXPECT_EQ(6, c_algo_unique_copy(&from_first, &from_last, &v_first, &v_output));
    EXPECT_TRUE(c_algo_equal(&v_first, &v_last, &e_first));
    EXPECT_EQ(6, C_ITER_DISTANCE(&v_first, v_output));

    c_list_destroy(from);
    c_list_destroy(expected);
}

} // namespace
} // namespace c_container
