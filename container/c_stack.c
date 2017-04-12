

#include "c_internal.h"
#include "c_list.h"
#include "c_stack.h"

struct __c_stack {
    c_list_t backend;
};

/**
 * constructor/destructor
 */
c_stack_t* c_stack_create(c_stack_t** stack, c_containable_t type_info)
{
}

void c_stack_destroy(c_stack_t* stack)
{
}

/**
 * element access
 */
c_ref_t c_stack_top(c_stack_t* stack)
{
    if (!stack) return 0;

    return stack->backend
}

/**
 * capacity
 */
bool c_stack_empty(c_stack_t* stack)
{
}

size_t c_stack_max_size(void)
{
}

/**
 * modifiers
 */
void c_stack_push(c_stack_t* stack, const c_ref_t data)
{
}

void c_stack_pop(c_stack_t* stack)
{
}

void c_stack_swap(c_stack_t* stack, c_stack_t* other)
{
}
