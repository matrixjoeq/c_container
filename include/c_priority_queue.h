

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
c_priority_queue_t* c_priority_queue_create(c_containable_t* type_info, BackendContainerCreator creator, c_compare comp);
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
void c_priority_queue_push(c_priority_queue_t* queue, c_ref_t data);
void c_priority_queue_pop(c_priority_queue_t* queue);
void c_priority_queue_swap(c_priority_queue_t* queue, c_priority_queue_t* other);

/**
 * helpers
 */
#define C_PRIORITY_QUEUE_INT c_priority_queue_create(c_get_int_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_SINT c_priority_queue_create(c_get_sint_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_UINT c_priority_queue_create(c_get_uint_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_SHORT c_priority_queue_create(c_get_short_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_SSHORT c_priority_queue_create(c_get_sshort_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_USHORT c_priority_queue_create(c_get_ushort_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_LONG c_priority_queue_create(c_get_long_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_SLONG c_priority_queue_create(c_get_slong_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_ULONG c_priority_queue_create(c_get_ulong_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_CHAR c_priority_queue_create(c_get_char_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_SCHAR c_priority_queue_create(c_get_schar_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_UCHAR c_priority_queue_create(c_get_uchar_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_FLOAT c_priority_queue_create(c_get_float_type_info(), c_vector_create_backend)
#define C_PRIORITY_QUEUE_DOUBLE c_priority_queue_create(c_get_double_type_info(), c_vector_create_backend)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_PRIORITY_QUEUE_H__
