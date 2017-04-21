
#include <stdlib.h>
#include "c_internal.h"
#include "c_stack.h"

struct __c_stack {
    c_backend_container_t* backend;
};

/**
 * constructor/destructor
 */
c_stack_t* c_stack_create(c_containable_t* type_info, BackendContainerCreator creator)
{
    c_stack_t* stack = (c_stack_t*)malloc(sizeof(c_stack_t));
    if (!stack) return 0;

    stack->backend = creator(type_info);
    if (!stack->backend) {
        __c_free(stack);
        return 0;
    }

    return stack;
}

void c_stack_destroy(c_stack_t* stack)
{
    if (!stack) return;
    stack->backend->ops->destroy(stack->backend);
    __c_free(stack);
}

/**
 * element access
 */
c_ref_t c_stack_top(c_stack_t* stack)
{
    if (!stack) return 0;
    return stack->backend->ops->back(stack->backend);
}

/**
 * capacity
 */
bool c_stack_empty(c_stack_t* stack)
{
    if (!stack) return true;
    return stack->backend->ops->empty(stack->backend);
}

size_t c_stack_max_size(c_stack_t* stack)
{
    if (!stack) return 0;
    return stack->backend->ops->max_size();
}

/**
 * modifiers
 */
void c_stack_push(c_stack_t* stack, c_ref_t data)
{
    if (!stack || !data) return;
    stack->backend->ops->push_back(stack->backend, data);
}

void c_stack_pop(c_stack_t* stack)
{
    if (!stack) return;
    stack->backend->ops->pop_back(stack->backend);
}

void c_stack_swap(c_stack_t* stack, c_stack_t* other)
{
    if (!stack || !other) return;
    stack->backend->ops->swap(stack->backend, other->backend);
}
