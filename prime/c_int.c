
#include "c_def.h"
#include "c_internal.h"

__c_static __c_inline size_t c_int_size(void)
{
    return sizeof(int);
}

__c_static __c_inline void c_int_create(c_ref_t obj)
{
    *((int*)obj) = 0;
}

__c_static __c_inline void c_int_copy(c_ref_t dst, const c_ref_t src)
{
    *((int*)dst) = *((int*)src);
}

__c_static __c_inline void c_int_destroy(c_ref_t obj)
{
    __c_unuse(obj);
}

__c_static __c_inline c_ref_t c_int_assign(c_ref_t dst, const c_ref_t src)
{
    *((int*)dst) = *((int*)src);
    return dst;
}

__c_static __c_inline bool c_int_less(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(int*)lhs) < *((int*)rhs);
}

__c_static __c_inline bool c_int_equal(const c_ref_t lhs, const c_ref_t rhs)
{
    return (*(int*)lhs) == (*(int*)rhs);
}

c_containable_t* c_get_int_type_info(void)
{
    static c_containable_t type_info = {
        .size = c_int_size,
        .create = c_int_create,
        .copy = c_int_copy,
        .destroy = c_int_destroy,
        .assign = c_int_assign,
        .less = c_int_less,
        .equal = c_int_equal
    };

    return &type_info;
}
