
#include <gtest/gtest.h>
#include <stdlib.h>
#include <stdint.h>
#include <vector>
#include "c_internal.h"
#include "c_iterator.h"
#include "c_vector.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

#pragma GCC diagnostic ignored "-Weffc++"
class CVectorTest : public ::testing::Test
{
public:
    CVectorTest() : vector(0) {}
    ~CVectorTest() { c_vector_destroy(vector); }

    void SetupVector(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_vector_push_back(vector, C_REF_T(&datas[i]));

        ExpectNotEmpty();
        EXPECT_EQ(length, c_vector_size(vector));
    }

    void Traverse(void)
    {
#ifdef CONFIG_TRAVERSE
        c_vector_iterator_t last = c_vector_end(vector);
        for (c_vector_iterator_t iter = c_vector_begin(vector); C_ITER_NE(&iter, &last); C_ITER_INC(&iter))
            printf("%d ", C_DEREF_INT(C_ITER_DEREF(&iter)));

        printf("\n");
#endif
    }

    void ExpectEqualToArray(const int* datas, int length)
    {
        EXPECT_EQ(length, c_vector_size(vector));
        c_vector_iterator_t first = c_vector_begin(vector);
        c_ref_t data = 0;
        for (int i = 0; i < length; ++i) {
            data = C_ITER_DEREF(&first);
            EXPECT_EQ(datas[i], C_DEREF_INT(data));
            C_ITER_INC(&first);
        }
    }

    void ExpectEmpty()
    {
        c_vector_iterator_t first = c_vector_begin(vector);
        c_vector_iterator_t last = c_vector_end(vector);
        EXPECT_TRUE(c_vector_empty(vector));
        EXPECT_EQ(0, c_vector_size(vector));
        EXPECT_TRUE(C_ITER_EQ(&first, &last));
    }

    void ExpectNotEmpty()
    {
        c_vector_iterator_t first = c_vector_begin(vector);
        c_vector_iterator_t last = c_vector_end(vector);
        EXPECT_FALSE(c_vector_empty(vector));
        EXPECT_NE(0, c_vector_size(vector));
        EXPECT_TRUE(C_ITER_NE(&first, &last));
    }

    void SetUp()
    {
        vector = C_VECTOR_INT;
        ExpectEmpty();
    }

    void TearDown()
    {
        c_vector_destroy(vector);
        vector = 0;
        EXPECT_TRUE(c_vector_empty(vector));
        EXPECT_EQ(0, c_vector_size(vector));
    }

protected:
    c_vector_t* vector;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CVectorTest, Clear)
{
    // clear a non-empty vector
    SetupVector(default_data, default_length);
    c_vector_clear(vector);
    ExpectEmpty();

    // clear an empty vector
    c_vector_clear(vector);
    ExpectEmpty();
}

TEST_F(CVectorTest, BeginEnd)
{
    // iterate a non-empty vector
    SetupVector(default_data, default_length);
    c_vector_iterator_t first = c_vector_begin(vector);
    c_vector_iterator_t last = c_vector_end(vector);
    c_vector_iterator_t rfirst = c_vector_rbegin(vector);
    c_vector_iterator_t rlast = c_vector_rend(vector);
    C_ITER_DEC(&last);
    C_ITER_DEC(&rlast);
    EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&first)), C_DEREF_INT(C_ITER_DEREF(&rlast)));
    EXPECT_EQ(C_DEREF_INT(C_ITER_DEREF(&rfirst)), C_DEREF_INT(C_ITER_DEREF(&last)));

    // iterate an empty vector
    c_vector_clear(vector);
    first = c_vector_begin(vector);
    last = c_vector_end(vector);
    rfirst = c_vector_rbegin(vector);
    rlast = c_vector_rend(vector);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));
    EXPECT_TRUE(C_ITER_EQ(&rfirst, &rlast));
}

TEST_F(CVectorTest, At)
{
	SetupVector(default_data, default_length);
	
	for (int i = 0; i < default_length; ++i) {
		EXPECT_EQ(i, C_DEREF_INT(c_vector_at(vector, i)));
	}
}

TEST_F(CVectorTest, BackOperations)
{
    // back operations on a non-empty vector
    SetupVector(default_data, default_length);

    c_vector_iterator_t first, last;
    size_t size = 0;
    c_ref_t data = 0;
    while (!c_vector_empty(vector)) {
        size = c_vector_size(vector);
        first = c_vector_begin(vector);
        last = c_vector_end(vector);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_vector_back(vector);
        EXPECT_EQ(size - 1, C_DEREF_INT(data));
        c_vector_pop_back(vector);
        EXPECT_EQ(size - 1, c_vector_size(vector));
    }

    // back operations on an empty vector
    ExpectEmpty();
    data = c_vector_back(vector);
    EXPECT_TRUE(data == 0);
    c_vector_pop_back(vector); // nothing should happen
}

TEST_F(CVectorTest, FrontOperations)
{
    // front operations on a non-empty vector
    SetupVector(default_data, default_length);

    c_vector_iterator_t first, last;
    size_t index = 0;
    c_ref_t data = 0;
    while (!c_vector_empty(vector)) {
        first = c_vector_begin(vector);
        last = c_vector_end(vector);
        EXPECT_TRUE(C_ITER_NE(&first, &last));
        data = c_vector_front(vector);
        EXPECT_EQ(default_data[index++], C_DEREF_INT(data));
        c_vector_erase(vector, first);
    }

    // front operations on an empty vector
    ExpectEmpty();
    data = c_vector_front(vector);
    EXPECT_TRUE(data == 0);
}

TEST_F(CVectorTest, Data)
{
    SetupVector(default_data, default_length);
    c_ref_t data = c_vector_data(vector);
    EXPECT_EQ(data, c_vector_front(vector));
}

TEST_F(CVectorTest, Reserve)
{
    SetupVector(default_data, default_length);
    size_t old_cap = c_vector_capacity(vector);

    c_vector_iterator_t first = c_vector_begin(vector);
    c_vector_iterator_t last = c_vector_end(vector);
    c_vector_reserve(vector, 0);
    c_vector_iterator_t new_first = c_vector_begin(vector);
    c_vector_iterator_t new_last = c_vector_end(vector);
    ExpectEqualToArray(default_data, default_length);
    EXPECT_TRUE(C_ITER_EQ(&first, &new_first));
    EXPECT_TRUE(C_ITER_EQ(&last, &new_last));

    c_vector_reserve(vector, old_cap);
    new_first = c_vector_begin(vector);
    new_last = c_vector_end(vector);
    ExpectEqualToArray(default_data, default_length);
    EXPECT_TRUE(C_ITER_EQ(&first, &new_first));
    EXPECT_TRUE(C_ITER_EQ(&last, &new_last));

    c_vector_reserve(vector, old_cap + 1);
    new_first = c_vector_begin(vector);
    new_last = c_vector_end(vector);
    EXPECT_TRUE(C_ITER_NE(&first, &new_first));
    EXPECT_TRUE(C_ITER_NE(&last, &new_last));
    EXPECT_EQ(old_cap * 2, c_vector_capacity(vector));

    old_cap = c_vector_capacity(vector);
    first = c_vector_begin(vector);
    last = c_vector_end(vector);
    c_vector_reserve(vector, old_cap * 2 + 1);
    new_first = c_vector_begin(vector);
    new_last = c_vector_end(vector);
    EXPECT_TRUE(C_ITER_NE(&first, &new_first));
    EXPECT_TRUE(C_ITER_NE(&last, &new_last));
    EXPECT_EQ(old_cap * 2 + 1, c_vector_capacity(vector));
}

TEST_F(CVectorTest, Shrink)
{
    SetupVector(default_data, default_length);
    EXPECT_NE(c_vector_size(vector), c_vector_capacity(vector));
    c_vector_shrink_to_fit(vector);
    EXPECT_EQ(c_vector_size(vector), c_vector_capacity(vector));

    c_vector_clear(vector);
    EXPECT_TRUE(0 == c_vector_front(vector));
    c_vector_shrink_to_fit(vector);
    EXPECT_TRUE(0 == c_vector_front(vector));
    EXPECT_TRUE(0 == c_vector_back(vector));
    EXPECT_EQ(0, c_vector_capacity(vector));
}

TEST_F(CVectorTest, Resize)
{
    SetupVector(default_data, default_length);

    int bigger[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0 };
    c_vector_resize(vector, c_vector_size(vector) + 3);
    ExpectEqualToArray(bigger, __array_length(bigger));

    int bigger_with_value[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 6, 6, 6 };
    int value = 6;
    c_vector_resize_with_value(vector, c_vector_size(vector) + 3, C_REF_T(&value));
    ExpectEqualToArray(bigger_with_value, __array_length(bigger_with_value));

    int smaller[] = { 0, 1, 2, 3, 4, 5 };
    c_vector_resize(vector, 6);
    ExpectEqualToArray(smaller, __array_length(smaller));

    int even_smaller[] = { 0, 1, 2, 3, 4 };
    c_vector_resize(vector, 5);
    ExpectEqualToArray(even_smaller, __array_length(even_smaller));
}

TEST_F(CVectorTest, Swap)
{
    SetupVector(default_data, default_length);

    c_vector_t* other = C_VECTOR_INT;

    c_vector_swap(vector, other);
    ExpectEmpty();
    c_vector_swap(vector, other);
    ExpectEqualToArray(default_data, default_length);
    EXPECT_TRUE(c_vector_empty(other));

    c_vector_destroy(other);
}

TEST_F(CVectorTest, InsertErase)
{
    c_vector_iterator_t first, last, iter;
    c_ref_t data = 0;

    for (int i = 0; i < 9; ++i) {
        last = c_vector_end(vector);
        iter = c_vector_insert(vector, last, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        EXPECT_EQ(i, C_DEREF_INT(data));
        EXPECT_EQ(i + 1, c_vector_size(vector));
    }

    ExpectNotEmpty();

    first = c_vector_begin(vector);
    last = c_vector_end(vector);
    while (C_ITER_NE(&first, &last)) {
        size_t size = c_vector_size(vector);
        first = c_vector_erase(vector, first);
        last = c_vector_end(vector);
        EXPECT_EQ(size - 1, c_vector_size(vector));
    }

    first = c_vector_erase(vector, first);
    EXPECT_TRUE(C_ITER_EQ(&first, &last));

    for (int i = 0; i < 9; ++i) {
        first = c_vector_begin(vector);
        iter = c_vector_insert(vector, first, C_REF_T(&i));
        data = C_ITER_DEREF(&iter);
        first = c_vector_begin(vector);
        EXPECT_TRUE(C_ITER_EQ(&first, &iter));
        EXPECT_EQ(i, C_DEREF_INT(data));
    }

    first = c_vector_begin(vector);
    last = c_vector_end(vector);
    first = c_vector_erase_range(vector, first, last);
    ExpectEmpty();
}

} // namespace
} // namespace c_container
