

#ifndef __C_QUEUE_H__
#define __C_QUEUE_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif __cplusplus

struct __c_queue;
typedef struct __c_queue c_queue_t;

/**
 * constructor/destructor
 */
c_queue_t* c_queue_create(c_queue_t** queue, c_containable_t type_info);
void c_queue_destroy(c_queue_t* queue);

/**
 * element access
 */
c_ref_t c_queue_front(c_queue_t* queue);
c_ref_t c_queue_back(c_queue_t* queue);

/**
 * capacity
 */
bool c_queue_empty(c_queue_t* queue);
size_t c_queue_size(c_queue_t* queue);

/**
 * modifiers
 */
void c_queue_push(c_queue_t* queue, const c_ref_t data);
void c_queue_pop(c_queue_t* queue);
void c_queue_swap(c_queue_t* queue, c_queue_t* other);

#ifdef __cplusplus
}
#endif __cplusplus

#endif // __C_QUEUE_H__
