

#ifndef __C_PRIORITY_QUEUE_H__
#define __C_PRIORITY_QUEUE_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_priority_queue;
typedef struct __c_priority_queue c_priority_queue_t;

/**
 * constructor/destructor
 */
c_priority_queue_t* c_priority_queue_create(const c_containable_t* type_info, BackendContainerCreator creator, c_compare comp);
void c_priority_queue_destroy(c_priority_queue_t* queue);

/**
 * element access
 */
c_ref_t c_priority_queue_top(c_priority_queue_t* queue);

/**
 * capacity
 */
bool c_priority_queue_empty(c_priority_queue_t* queue);
size_t c_priority_queue_size(c_priority_queue_t* queue);

/**
 * modifiers
 */
void c_priority_queue_push(c_priority_queue_t* queue, const c_ref_t data);
void c_priority_queue_pop(c_priority_queue_t* queue);
void c_priority_queue_swap(c_priority_queue_t* queue, c_priority_queue_t* other);

/**
 * helpers
 */
#define C_PRIORITY_QUEUE_INT c_priority_queue_create(&c_int_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_SINT c_priority_queue_create(&c_signed_int_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_UINT c_priority_queue_create(&c_unsigned_int_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_SHORT c_priority_queue_create(&c_short_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_SSHORT c_priority_queue_create(&c_signed_short_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_USHORT c_priority_queue_create(&c_unsigned_short_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_LONG c_priority_queue_create(&c_long_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_SLONG c_priority_queue_create(&c_signed_long_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_ULONG c_priority_queue_create(&c_unsigned_long_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_CHAR c_priority_queue_create(&c_char_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_SCHAR c_priority_queue_create(&c_signed_char_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_UCHAR c_priority_queue_create(&c_unsigned_char_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_FLOAT c_priority_queue_create(&c_float_t, c_vector_create_backend)
#define C_PRIORITY_QUEUE_DOUBLE c_priority_queue_create(&c_double_t, c_vector_create_backend)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_PRIORITY_QUEUE_H__
