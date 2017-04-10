
#ifndef __C_INTERNAL_H__
#define __C_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef GTEST_UNIT_TEST
#define __c_static static
#define __c_inline inline
#else
#define __c_static
#define __c_inline
#endif

// general macro
#define __c_free(x) \
    do { \
        if ((x)) { \
            free((x)); \
            (x) = NULL; \
        } \
    } while (0)

#define __c_unuse(x) (void)(x)
#define __array_length(__array) sizeof(__array)/sizeof(__array[0])
#define __array_foreach(__array, __index) for (uint32_t __index = 0; __index < __array_length(__array); ++__index)

// for container
#define CREATE_DATA(x, type, value) \
    type* x = (type*) malloc(sizeof(type)); \
    memcpy(x, &(value), sizeof(type))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_INTERNAL_H__
