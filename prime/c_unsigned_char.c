
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_unsigned_char_size(void)
{
    return sizeof(unsigned char);
}

__c_static __c_inline void c_unsigned_char_create(c_ref_t obj)
{
    *((unsigned char*)obj) = 0u;
}

__c_static __c_inline void c_unsigned_char_copy(c_ref_t dst, const c_ref_t src)
{
    *((unsigned char*)dst) = *((unsigned char*)src);
}

__c_static __c_inline void c_unsigned_char_destroy(c_ref_t obj)
{
    __c_unuse(obj);
}

__c_static __c_inline c_ref_t c_unsigned_char_assign(c_ref_t dst, const c_ref_t src)
{
    *((unsigned char*)dst) = *((unsigned char*)src);
    return dst;
}

__c_static __c_inline bool c_unsigned_char_less(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(unsigned char*)lhs) < *((unsigned char*)rhs);
}

__c_static __c_inline bool c_unsigned_char_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(unsigned char*)lhs) == (*(unsigned char*)rhs);
}

c_containable_t* c_get_uchar_type_info(void)
{
    static c_containable_t type_info = {
        .size = c_unsigned_char_size,
        .create = c_unsigned_char_create,
        .copy = c_unsigned_char_copy,
        .destroy = c_unsigned_char_destroy,
        .assign = c_unsigned_char_assign,
        .less = c_unsigned_char_less,
        .equal = c_unsigned_char_equal
    };

    return &type_info;
}
