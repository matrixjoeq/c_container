
#ifndef __C_INTERNAL_H__
#define __C_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef GTEST_UNIT_TEST
#define __c_static static
#ifdef __GNUC__
#define __c_inline __attribute__((always_inline)) inline
#else
#define __c_inline inline
#endif
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

#define __c_assert(cond, msg) assert((cond) && (msg))
#define __c_unuse(x) (void)(x)
#define __array_length(__array) sizeof(__array) / sizeof(__array[0])
#define __array_foreach(__array, __index) for (unsigned int __index = 0; __index < __array_length(__array); ++__index)
#define __c_max(x, y) (((x) < (y)) ? (y) : (x))

// algorithm macro
#define __c_iter_get_shadow(x) \
    c_iterator_t* __##x = 0; \
    C_ITER_COPY(&__##x, x); \

#define __c_iter_put_shadow(x) \
    __c_free(__##x); \

#define __C_ALGO_BEGIN_1(x) \
    __c_iter_get_shadow(x)

#define __C_ALGO_END_1(x) \
    __c_iter_put_shadow(x)

#define __C_ALGO_BEGIN_2(x, y) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \

#define __C_ALGO_END_2(x, y) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \

#define __C_ALGO_BEGIN_3(x, y, z) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \
    __c_iter_get_shadow(z) \

#define __C_ALGO_END_3(x, y, z) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \
    __c_iter_put_shadow(z) \

#define __C_ALGO_BEGIN_4(x, y, z, u) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \
    __c_iter_get_shadow(z) \
    __c_iter_get_shadow(u) \

#define __C_ALGO_END_4(x, y, z, u) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \
    __c_iter_put_shadow(z) \
    __c_iter_put_shadow(u) \

#define __C_ALGO_BEGIN_5(x, y, z, u, v) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \
    __c_iter_get_shadow(z) \
    __c_iter_get_shadow(u) \
    __c_iter_get_shadow(v) \

#define __C_ALGO_END_5(x, y, z, u, v) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \
    __c_iter_put_shadow(z) \
    __c_iter_put_shadow(u) \
    __c_iter_put_shadow(v) \

#define __C_ALGO_BEGIN_6(x, y, z, u, v, w) \
    __c_iter_get_shadow(x) \
    __c_iter_get_shadow(y) \
    __c_iter_get_shadow(z) \
    __c_iter_get_shadow(u) \
    __c_iter_get_shadow(v) \
    __c_iter_get_shadow(w) \

#define __C_ALGO_END_6(x, y, z, u, v, w) \
    __c_iter_put_shadow(x) \
    __c_iter_put_shadow(y) \
    __c_iter_put_shadow(z) \
    __c_iter_put_shadow(u) \
    __c_iter_put_shadow(v) \
    __c_iter_put_shadow(w) \

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_INTERNAL_H__
