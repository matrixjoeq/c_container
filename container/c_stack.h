

#ifndef __C_STACK_H__
#define __C_STACK_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif __cplusplus

struct __c_stack;
typedef struct __c_stack c_stack_t;

/**
 * constructor/destructor
 */
c_stack_t* c_stack_create(c_stack_t** stack, c_containable_t type_info);
void c_stack_destroy(c_stack_t* stack);

/**
 * element access
 */
c_ref_t c_stack_top(c_stack_t* stack);

/**
 * capacity
 */
bool c_stack_empty(c_stack_t* stack);
size_t c_stack_max_size(void);

/**
 * modifiers
 */
void c_stack_push(c_stack_t* stack, const c_ref_t data);
void c_stack_pop(c_stack_t* stack);
void c_stack_swap(c_stack_t* stack, c_stack_t* other);

#ifdef __cplusplus
}
#endif __cplusplus

#endif // __C_STACK_H__
