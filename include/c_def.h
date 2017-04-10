
#ifndef __C_DEF_H__
#define __C_DEF_H__

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef void* c_ref_t;
typedef void (*c_unary_func)(c_ref_t);

// return true if data matches condition
typedef bool (*c_unary_predicate)(c_ref_t);
typedef bool (*c_binary_predicate)(c_ref_t, c_ref_t);

// return true if compare(lhs, rhs)
typedef bool (*c_compare)(c_ref_t lhs, c_ref_t rhs);

typedef struct __c_containable {
    // size information
    size_t (*size)(void);
    // default constructor
    void (*create)(c_ref_t obj);
    // copy constructor
    void (*copy)(c_ref_t dst, const c_ref_t src);
    // destructor
    void (*destroy)(c_ref_t obj);
    // operator=
    c_ref_t (*assign)(c_ref_t dst, const c_ref_t src);
    // operator<
    bool (*less)(const c_ref_t lhs, const c_ref_t rhs);
    // operator==
    bool (*equal)(const c_ref_t lhs, const c_ref_t rhs);
} c_containable_t;

typedef struct __c_iterator {
    // operator++
    struct __c_iterator* (*increment)(struct __c_iterator*);
    // operator--
    struct __c_iterator* (*decrement)(struct __c_iterator*);

    // TODO: there seems no way to implement the post-operators in C language
    // because post-operators return a temporary iterator object
    // if it is a c_iterator_t object, it loses container specific informations
    // if it is a c_iterator_t pointer, it must be allocated on heap and requires free after use, which is not good
    // the most reasonable way it NOT to provide post-operators in C language
    // operator++(int)
    //struct __c_iterator (*post_increment)(struct __c_iterator*);
    // operator--(int)
    //struct __c_iterator (*post_decrement)(struct __c_iterator*);

    // operator*
    c_ref_t (*dereference)(struct __c_iterator*);

    // operator==
    bool (*equal)(struct __c_iterator*, struct __c_iterator*);

    // operator!=
    bool (*not_equal)(struct __c_iterator*, struct __c_iterator*);

    c_containable_t* type_info;
} c_iterator_t;

extern const c_containable_t c_int_t;
extern const c_containable_t c_signed_int_t;
extern const c_containable_t c_unsigned_int_t;
extern const c_containable_t c_short_t;
extern const c_containable_t c_signed_short_t;
extern const c_containable_t c_unsigned_short_t;
extern const c_containable_t c_long_t;
extern const c_containable_t c_signed_long_t;
extern const c_containable_t c_unsigned_long_t;
extern const c_containable_t c_char_t;
extern const c_containable_t c_signed_char_t;
extern const c_containable_t c_unsigned_char_t;
extern const c_containable_t c_float_t;
extern const c_containable_t c_double_t;

#define C_REF_T(x) (c_ref_t)(x)
#define C_ITERATOR_T(x) (c_iterator_t*)(x)

#define C_DEREF_INT(x) (*(int*)(x))
#define C_DEREF_SINT(x) (*(signed int*)(x))
#define C_DEREF_UINT(x) (*(unsigned int*)(x))
#define C_DEREF_SHORT(x) (*(short*)(x))
#define C_DEREF_SSHORT(x) (*(signed short*)(x))
#define C_DEREF_USHORT(x) (*(unsigned short*)(x))
#define C_DEREF_LONG(x) (*(long*)(x))
#define C_DEREF_SLONG(x) (*(signed long*)(x))
#define C_DEREF_ULONG(x) (*(unsigned long*)(x))
#define C_DEREF_CHAR(x) (*(char*)(x))
#define C_DEREF_SCHAR(x) (*(signed char*)(x))
#define C_DEREF_UCHAR(x) (*(unsigned char*)(x))
#define C_DEREF_FLOAT(x) (*(float*)(x))
#define C_DEREF_DOUBLE(x) (*(double*)(x))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_DEF_H__
