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
#include "c_queue.h"

struct __c_queue {
    c_backend_container_t* backend;
};

/**
 * constructor/destructor
 */
c_queue_t* c_queue_create(const c_type_info_t* value_type, BackendContainerCreator creator)
{
    if (!value_type || !creator) return 0;
    validate_type_info(value_type);

    c_queue_t* queue = (c_queue_t*)malloc(sizeof(c_queue_t));
    if (!queue) return 0;

    queue->backend = creator(value_type);
    if (!queue->backend) {
        __c_free(queue);
        return 0;
    }

    return queue;
}

void c_queue_destroy(c_queue_t* queue)
{
    if (!queue) return;
    queue->backend->ops->destroy(queue->backend);
    __c_free(queue);
}

/**
 * element access
 */
c_ref_t c_queue_front(c_queue_t* queue)
{
    if (!queue) return 0;
    return queue->backend->ops->front(queue->backend);
}

c_ref_t c_queue_back(c_queue_t* queue)
{
    if (!queue) return 0;
    return queue->backend->ops->back(queue->backend);
}

/**
 * capacity
 */
bool c_queue_empty(c_queue_t* queue)
{
    if (!queue) return true;
    return queue->backend->ops->empty(queue->backend);
}

size_t c_queue_size(c_queue_t* queue)
{
    if (!queue) return 0;
    return queue->backend->ops->size(queue->backend);
}

/**
 * modifiers
 */
void c_queue_push(c_queue_t* queue, c_ref_t value)
{
    if (!queue || !value) return;
    queue->backend->ops->push_back(queue->backend, value);
}

void c_queue_pop(c_queue_t* queue)
{
    if (!queue) return;
    queue->backend->ops->pop_front(queue->backend);
}

void c_queue_swap(c_queue_t* queue, c_queue_t* other)
{
    if (!queue || !other) return;
    queue->backend->ops->swap(queue->backend, other->backend);
}
