
#include <gtest/gtest.h>
#include <stdlib.h>
#include "c_internal.h"
#include "c_stack.h"

namespace c_container {
namespace {

const int default_data[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
const int default_length = __array_length(default_data);

#pragma GCC diagnostic ignored "-Weffc++"
class CStackTest : public ::testing::Test
{
public:
    CStackTest() : stack(0) {}
    ~CStackTest() { c_stack_destroy(stack); }

    void SetupStack(const int *datas, int length)
    {
        for (int i = 0; i < length; ++i)
            c_stack_push(stack, C_REF_T(&datas[i]));

        ExpectNotEmpty();
    }

    void ExpectEmpty()
    {
        EXPECT_TRUE(c_stack_empty(stack));
    }

    void ExpectNotEmpty()
    {
        EXPECT_FALSE(c_stack_empty(stack));
    }

    void SetUp()
    {
        stack = C_STACK_INT;
        ExpectEmpty();
    }

    void TearDown()
    {
        c_stack_destroy(stack);
        stack = 0;
        ExpectEmpty();
    }

protected:
    c_stack_t* stack;
};
#pragma GCC diagnostic warning "-Weffc++"

TEST_F(CStackTest, Top)
{
    SetupStack(default_data, default_length);

	c_ref_t top = 0;
	int index = default_length;
    while (!c_stack_empty(stack)) {
        top = c_stack_top(stack);
        EXPECT_EQ(default_data[--index], C_DEREF_INT(top));
		c_stack_pop(stack);
    }

    ExpectEmpty();
}

TEST_F(CStackTest, Swap)
{
    SetupStack(default_data, default_length);

    c_stack_t* other = C_STACK_INT;

    c_stack_swap(stack, other);
    ExpectEmpty();
	
    c_stack_swap(stack, other);
	EXPECT_TRUE(c_stack_empty(other));
	c_ref_t top = 0;
	int index = default_length;
    while (!c_stack_empty(stack)) {
        top = c_stack_top(stack);
        EXPECT_EQ(default_data[--index], C_DEREF_INT(top));
		c_stack_pop(stack);
    }

	ExpectEmpty();
	
    c_stack_destroy(other);
}

} // namespace
} // namespace c_container
