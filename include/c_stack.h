

#ifndef __C_STACK_H__
#define __C_STACK_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_stack;
typedef struct __c_stack c_stack_t;

/**
 * constructor/destructor
 */
c_stack_t* c_stack_create(const c_containable_t* type_info, BackendContainerCreator creator);
void c_stack_destroy(c_stack_t* stack);

/**
 * element access
 */
c_ref_t c_stack_top(c_stack_t* stack);

/**
 * capacity
 */
bool c_stack_empty(c_stack_t* stack);
size_t c_stack_max_size(c_stack_t* stack);

/**
 * modifiers
 */
void c_stack_push(c_stack_t* stack, const c_ref_t data);
void c_stack_pop(c_stack_t* stack);
void c_stack_swap(c_stack_t* stack, c_stack_t* other);

/**
 * helpers
 */
#define C_STACK_INT c_stack_create(&c_int_t, c_list_create_backend)
#define C_STACK_SINT c_stack_create(&c_signed_int_t, c_list_create_backend)
#define C_STACK_UINT c_stack_create(&c_unsigned_int_t, c_list_create_backend)
#define C_STACK_SHORT c_stack_create(&c_short_t, c_list_create_backend)
#define C_STACK_SSHORT c_stack_create(&c_signed_short_t, c_list_create_backend)
#define C_STACK_USHORT c_stack_create(&c_unsigned_short_t, c_list_create_backend)
#define C_STACK_LONG c_stack_create(&c_long_t, c_list_create_backend)
#define C_STACK_SLONG c_stack_create(&c_signed_long_t, c_list_create_backend)
#define C_STACK_ULONG c_stack_create(&c_unsigned_long_t, c_list_create_backend)
#define C_STACK_CHAR c_stack_create(&c_char_t, c_list_create_backend)
#define C_STACK_SCHAR c_stack_create(&c_signed_char_t, c_list_create_backend)
#define C_STACK_UCHAR c_stack_create(&c_unsigned_char_t, c_list_create_backend)
#define C_STACK_FLOAT c_stack_create(&c_float_t, c_list_create_backend)
#define C_STACK_DOUBLE c_stack_create(&c_double_t, c_list_create_backend)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_STACK_H__