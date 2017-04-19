
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_char_size(void)
{
    return sizeof(char);
}

__c_static __c_inline void c_char_create(c_ref_t obj)
{
    *((char*)obj) = 0;
}

__c_static __c_inline void c_char_copy(c_ref_t dst, const c_ref_t src)
{
    *((char*)dst) = *((char*)src);
}

__c_static __c_inline void c_char_destroy(c_ref_t obj)
{
    __c_unuse(obj);
}

__c_static __c_inline c_ref_t c_char_assign(c_ref_t dst, const c_ref_t src)
{
    *((char*)dst) = *((char*)src);
    return dst;
}

__c_static __c_inline bool c_char_less(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(char*)lhs) < *((char*)rhs);
}

__c_static __c_inline bool c_char_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(char*)lhs) == (*(char*)rhs);
}

c_containable_t* c_get_char_type_info(void)
{
    static c_containable_t type_info = {
        .size = c_char_size,
        .create = c_char_create,
        .copy = c_char_copy,
        .destroy = c_char_destroy,
        .assign = c_char_assign,
        .less = c_char_less,
        .equal = c_char_equal
    };

    return &type_info;
}
