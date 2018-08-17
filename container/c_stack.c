/**
 * MIT License
 *
 * Copyright (c) 2017 MatrixJoeQ
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include "c_internal.h"
#include "c_util.h"
#include "c_stack.h"

struct __c_stack {
    c_backend_container_t* backend;
};

/**
 * constructor/destructor
 */
c_stack_t* c_stack_create(const c_type_info_t* value_type, BackendContainerCreator creator)
{
    if (!value_type || !creator) return 0;
    validate_type_info(value_type);
    
    c_stack_t* stack = (c_stack_t*)malloc(sizeof(c_stack_t));
    if (!stack) return 0;

    stack->backend = creator(value_type);
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
void c_stack_push(c_stack_t* stack, c_ref_t value)
{
    if (!stack || !value) return;
    stack->backend->ops->push_back(stack->backend, value);
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
