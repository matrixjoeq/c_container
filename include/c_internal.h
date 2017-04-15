
#ifndef __C_INTERNAL_H__
#define __C_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef GTEST_UNIT_TEST
#define __c_static static
#define __c_inline __attribute__((always_inline)) inline
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
#define __array_foreach(__array, __index) for (unsigned int __index = 0; __index < __array_length(__array); ++__index)
#define __c_max(x, y) (((x) < (y)) ? (y) : (x))

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_INTERNAL_H__
