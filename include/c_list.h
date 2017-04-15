

#ifndef __C_LIST_H__
#define __C_LIST_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_list;
struct __c_list_node;
struct __c_backend_list;

typedef struct __c_list c_list_t;
typedef struct __c_backend_list c_backend_list_t;

typedef struct __c_list_iterator {
    c_iterator_t base_iter;
    struct __c_list_node* node;
} c_list_iterator_t;

/**
 * constructor/destructor
 */
c_list_t* c_list_create(const c_containable_t* type_info);
void c_list_destroy(c_list_t* list);

/**
 * element access
 */
c_ref_t c_list_front(c_list_t* list);
c_ref_t c_list_back(c_list_t* list);

/**
 * iterators
 */
c_list_iterator_t c_list_begin(c_list_t* list);
c_list_iterator_t c_list_rbegin(c_list_t* list);
c_list_iterator_t c_list_end(c_list_t* list);
c_list_iterator_t c_list_rend(c_list_t* list);

/**
 * capacity
 */
bool c_list_empty(c_list_t* list);
size_t c_list_size(c_list_t* list);
size_t c_list_max_size(void);

/**
 * modifiers
 */
void c_list_clear(c_list_t* list);
c_list_iterator_t c_list_insert(c_list_t* list, c_list_iterator_t pos, const c_ref_t data);
c_list_iterator_t c_list_erase(c_list_t* list, c_list_iterator_t pos);
c_list_iterator_t c_list_erase_range(c_list_t* list, c_list_iterator_t first, c_list_iterator_t last);
void c_list_push_back(c_list_t* list, const c_ref_t data);
void c_list_pop_back(c_list_t* list);
void c_list_push_front(c_list_t* list, const c_ref_t data);
void c_list_pop_front(c_list_t* list);
void c_list_resize(c_list_t* list, size_t count);
void c_list_resize_with_value(c_list_t* list, size_t count, const c_ref_t data);
void c_list_swap(c_list_t* list, c_list_t* other);

/**
 * operations
 */
void c_list_merge(c_list_t* list, c_list_t* other);
void c_list_merge_by(c_list_t* list, c_list_t* other, c_compare comp);
void c_list_splice(c_list_t* list, c_list_iterator_t pos, c_list_t* other);
void c_list_splice_from(c_list_t* list, c_list_iterator_t pos, c_list_t* other, c_list_iterator_t from);
void c_list_splice_range(c_list_t* list, c_list_iterator_t pos, c_list_t* other, c_list_iterator_t first, c_list_iterator_t last);
void c_list_remove(c_list_t* list, const c_ref_t data);
void c_list_remove_if(c_list_t* list, c_unary_predicate pred);
void c_list_sort(c_list_t* list);
void c_list_sort_by(c_list_t* list, c_compare comp);
void c_list_reverse(c_list_t* list);
void c_list_unique(c_list_t* list);
void c_list_unique_if(c_list_t* list, c_binary_predicate pred);

/**
 * backend
 */
c_backend_container_t* c_list_create_backend(const c_containable_t* type_info);

/**
 * helpers
 */
#define C_LIST_INT c_list_create(&c_int_t)
#define C_LIST_SINT c_list_create(&c_signed_int_t)
#define C_LIST_UINT c_list_create(&c_unsigned_int_t)
#define C_LIST_SHORT c_list_create(&c_short_t)
#define C_LIST_SSHORT c_list_create(&c_signed_short_t)
#define C_LIST_USHORT c_list_create(&c_unsigned_short_t)
#define C_LIST_LONG c_list_create(&c_long_t)
#define C_LIST_SLONG c_list_create(&c_signed_long_t)
#define C_LIST_ULONG c_list_create(&c_unsigned_long_t)
#define C_LIST_CHAR c_list_create(&c_char_t)
#define C_LIST_SCHAR c_list_create(&c_signed_char_t)
#define C_LIST_UCHAR c_list_create(&c_unsigned_char_t)
#define C_LIST_FLOAT c_list_create(&c_float_t)
#define C_LIST_DOUBLE c_list_create(&c_double_t)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __C_LIST_H__