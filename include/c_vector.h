

#ifndef __C_VECTOR_H__
#define __C_VECTOR_H__

#include <stdbool.h>
#include <stddef.h>
#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct __c_vector;
struct __c_backend_vector;

typedef struct __c_vector c_vector_t;
typedef struct __c_backend_vector c_backend_vector_t;

typedef struct __c_vector_iterator {
    c_iterator_t base_iter;
    c_ref_t pos;
} c_vector_iterator_t;

/**
 * constructor/destructor
 */
c_vector_t* c_vector_create(const c_containable_t* type_info);
void c_vector_destroy(c_vector_t* vector);

/**
 * element access
 */
c_ref_t c_vector_at(c_vector_t* vector, size_t pos);
c_ref_t c_vector_front(c_vector_t* vector);
c_ref_t c_vector_back(c_vector_t* vector);
c_ref_t c_vector_data(c_vector_t* vector);

/**
 * iterators
 */
c_vector_iterator_t c_vector_begin(c_vector_t* vector);
c_vector_iterator_t c_vector_rbegin(c_vector_t* vector);
c_vector_iterator_t c_vector_end(c_vector_t* vector);
c_vector_iterator_t c_vector_rend(c_vector_t* vector);

/**
 * capacity
 */
bool c_vector_empty(c_vector_t* vector);
size_t c_vector_size(c_vector_t* vector);
size_t c_vector_max_size(void);
void c_vector_reserve(c_vector_t* vector, size_t new_cap);
size_t c_vector_capacity(c_vector_t* vector);
void c_vector_shrink_to_fit(c_vector_t* vector);

/**
 * modifiers
 */
void c_vector_clear(c_vector_t* vector);
c_vector_iterator_t c_vector_insert(c_vector_t* vector, c_vector_iterator_t pos, const c_ref_t data);
c_vector_iterator_t c_vector_insert_n(c_vector_t* vector, c_vector_iterator_t pos, size_t count, const c_ref_t data);
c_vector_iterator_t c_vector_insert_range(c_vector_t* vector, c_vector_iterator_t pos, c_iterator_t first, c_iterator_t last);
c_vector_iterator_t c_vector_erase(c_vector_t* vector, c_vector_iterator_t pos);
c_vector_iterator_t c_vector_erase_range(c_vector_t* vector, c_vector_iterator_t first, c_vector_iterator_t last);
void c_vector_push_back(c_vector_t* vector, const c_ref_t data);
void c_vector_pop_back(c_vector_t* vector);
void c_vector_resize(c_vector_t* vector, size_t count);
void c_vector_resize_with_value(c_vector_t* vector, size_t count, const c_ref_t data);
void c_vector_swap(c_vector_t* vector, c_vector_t* other);

/**
 * backend
 */
c_backend_container_t* c_vector_create_backend(const c_containable_t* type_info);

/**
 * helpers
 */
#define C_VECTOR_INT c_vector_create(&c_int_t)
#define C_VECTOR_SINT c_vector_create(&c_signed_int_t)
#define C_VECTOR_UINT c_vector_create(&c_unsigned_int_t)
#define C_VECTOR_SHORT c_vector_create(&c_short_t)
#define C_VECTOR_SSHORT c_vector_create(&c_signed_short_t)
#define C_VECTOR_USHORT c_vector_create(&c_unsigned_short_t)
#define C_VECTOR_LONG c_vector_create(&c_long_t)
#define C_VECTOR_SLONG c_vector_create(&c_signed_long_t)
#define C_VECTOR_ULONG c_vector_create(&c_unsigned_long_t)
#define C_VECTOR_CHAR c_vector_create(&c_char_t)
#define C_VECTOR_SCHAR c_vector_create(&c_signed_char_t)
#define C_VECTOR_UCHAR c_vector_create(&c_unsigned_char_t)
#define C_VECTOR_FLOAT c_vector_create(&c_float_t)
#define C_VECTOR_DOUBLE c_vector_create(&c_double_t)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __C_VECTOR_H__
