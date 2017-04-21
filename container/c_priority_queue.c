
#include <stdlib.h>
#include "c_internal.h"
#include "c_algorithm.h"
#include "c_priority_queue.h"

struct __c_priority_queue {
    c_backend_container_t* backend;
    c_compare comp;
};

/**
 * constructor/destructor
 */
c_priority_queue_t* c_priority_queue_create(
    c_containable_t* type_info, BackendContainerCreator creator, c_compare comp)
{
    c_priority_queue_t* queue = (c_priority_queue_t*)malloc(sizeof(c_priority_queue_t));
    if (!queue) return 0;

    queue->backend = creator(type_info);
    if (!queue->backend) {
        __c_free(queue);
        return 0;
    }

    queue->comp = comp ? comp : type_info->less;
    return queue;
}

void c_priority_queue_destroy(c_priority_queue_t* queue)
{
    if (!queue) return;
    queue->backend->ops->destroy(queue->backend);
    __c_free(queue);
}

/**
 * element access
 */
c_ref_t c_priority_queue_top(c_priority_queue_t* queue)
{
    if (!queue) return 0;
    return queue->backend->ops->front(queue->backend);
}

/**
 * capacity
 */
bool c_priority_queue_empty(c_priority_queue_t* queue)
{
    if (!queue) return true;
    return queue->backend->ops->empty(queue->backend);
}

size_t c_priority_queue_size(c_priority_queue_t* queue)
{
    if (!queue) return 0;
    return queue->backend->ops->size(queue->backend);
}

/**
 * modifiers
 */
void c_priority_queue_push(c_priority_queue_t* queue, c_ref_t data)
{
    if (!queue || !data) return;

    c_iterator_t* first = 0;
    c_iterator_t* last = 0;
    queue->backend->ops->begin(queue->backend, &first);
    queue->backend->ops->end(queue->backend, &last);

    queue->backend->ops->push_back(queue->backend, data);
    c_algo_push_heap_by(first, last, queue->comp);

    __c_free(first);
    __c_free(last);
}

void c_priority_queue_pop(c_priority_queue_t* queue)
{
    if (!queue) return;

    c_iterator_t* first = 0;
    c_iterator_t* last = 0;
    queue->backend->ops->begin(queue->backend, &first);
    queue->backend->ops->end(queue->backend, &last);

    c_algo_pop_heap_by(first, last, queue->comp);
    queue->backend->ops->pop_back(queue->backend);

    __c_free(first);
    __c_free(last);
}

void c_priority_queue_swap(c_priority_queue_t* queue, c_priority_queue_t* other)
{
    if (!queue || !other) return;
    queue->backend->ops->swap(queue->backend, other->backend);
}
