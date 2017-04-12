
#include <stdlib.h>
#include "c_internal.h"
#include "c_list.h"
#include "c_queue.h"

struct __c_queue {
    c_backend_container_t* backend;
};

/**
 * constructor/destructor
 */
c_queue_t* c_queue_create(const c_containable_t* type_info)
{
	c_queue_t* queue = (c_queue_t*)malloc(sizeof(c_queue_t));
	if (!queue) return 0;
	
	queue->backend = (c_backend_container_t*)c_list_create_backend(type_info);
	if (!queue->backend) {
		__c_free(queue);
		return 0;
	}
	
	return queue;
}

void c_queue_destroy(c_queue_t* queue)
{
	if (!queue) return;
	queue->backend->destroy(queue->backend);
	__c_free(queue);
}

/**
 * element access
 */
c_ref_t c_queue_front(c_queue_t* queue)
{
	if (!queue) return 0;
	return queue->backend->front(queue->backend);
}

c_ref_t c_queue_back(c_queue_t* queue)
{
	if (!queue) return 0;
	return queue->backend->back(queue->backend);
}

/**
 * capacity
 */
bool c_queue_empty(c_queue_t* queue)
{
	if (!queue) return true;
	return queue->backend->empty(queue->backend);
}

size_t c_queue_size(c_queue_t* queue)
{
	if (!queue) return 0;
	return queue->backend->size(queue->backend);
}

/**
 * modifiers
 */
void c_queue_push(c_queue_t* queue, const c_ref_t data)
{
	if (!queue || !data) return;
	queue->backend->push_back(queue->backend, data);
}

void c_queue_pop(c_queue_t* queue)
{
	if (!queue) return;
	queue->backend->pop_front(queue->backend);
}

void c_queue_swap(c_queue_t* queue, c_queue_t* other)
{
	if (!queue || !other) return;
	queue->backend->swap(queue->backend, other->backend);
}
