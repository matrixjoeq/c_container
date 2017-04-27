

#ifndef __C_ALGORITHM_H__
#define __C_ALGORITHM_H__

#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*************************************/
/* non-modifying sequence operations */
/*************************************/
// Checks if unary predicate returns true for all elements in the range [first, last).
bool algo_all_of(c_iterator_t* __c_input_iterator first,
                 c_iterator_t* __c_input_iterator last,
                 c_unary_predicate pred);

// Checks if unary predicate returns true for at least one element in the range [first, last).
bool algo_any_of(c_iterator_t* __c_input_iterator first,
                 c_iterator_t* __c_input_iterator last,
                 c_unary_predicate pred);

// Checks if unary predicate returns true for no elements in the range [first, last).
bool algo_none_of(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_unary_predicate pred);

// Applies the given function to the result of dereferencing every iterator in the range [first, last), in order.
// Returns the number of elements in range [first, last).
size_t algo_for_each(c_iterator_t* __c_input_iterator first,
                     c_iterator_t* __c_input_iterator last,
                     c_unary_func op);

// Applies the given function to the result of dereferencing every iterator in the range [first, first + n), in order.
// Sets reached to first + n.
void algo_for_each_n(c_iterator_t* __c_input_iterator first,
                     size_t n,
                     c_iterator_t** __c_input_iterator reached,
                     c_unary_func op);

// Returns the number of elements in the range [first, last) if the elements that are equal to value.
size_t algo_count(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_ref_t value);

// Returns the number of elements in the range [first, last) if the elements make predicate returns true.
size_t algo_count_if(c_iterator_t* __c_input_iterator first,
                     c_iterator_t* __c_input_iterator last,
                     c_unary_predicate pred);

// Returns if there's mismatching pair of elements from two ranges: one defined by [first, last)
// and another defined by [first2, first2 + (last - first)).
// Elements are considered to be mismatching if predicate returns false.
// Sets mismatch1 to the first mismatching positin in [first, last).
// Sets mismatch2 to the first mismatching position in [first2, first2 + (last - first)).
// If no mismatching, mismatch1 is set to last, mismatch2 is set to first2 + (last - first).
bool algo_mismatch_by(c_iterator_t* __c_input_iterator first,
                      c_iterator_t* __c_input_iterator last,
                      c_iterator_t* __c_input_iterator first2,
                      c_iterator_t** __c_input_iterator mismatch1,
                      c_iterator_t** __c_input_iterator mismatch2,
                      c_binary_predicate pred);

// Returns true if the range [first, last) is equal (which means predicate returns true)
// to the range [first2, first2 + (last1 - first1)), and false otherwise
bool algo_equal_by(c_iterator_t* __c_input_iterator first,
                   c_iterator_t* __c_input_iterator last,
                   c_iterator_t* __c_input_iterator first2,
                   c_binary_predicate pred);

// Returns true if there's an element makes predicate return true in the range [first, last).
// Sets found to the first element that satisfies specific criteria.
bool algo_find_by(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_iterator_t** __c_input_iterator found,
                  c_ref_t value,
                  c_binary_predicate pred);

// Returns true if there's an element makes predicate return true in the range [first, last).
// Sets found to the first element that satisfies specific criteria.
bool algo_find_if(c_iterator_t* __c_input_iterator first,
                  c_iterator_t* __c_input_iterator last,
                  c_iterator_t** __c_input_iterator found,
                  c_unary_predicate pred);

// Returns true if there's an element makes predicate return false in the range [first, last).
// Sets found to the first element that doesn't satisfies specific criteria.
bool algo_find_if_not(c_iterator_t* __c_input_iterator first,
                      c_iterator_t* __c_input_iterator last,
                      c_iterator_t** __c_input_iterator found,
                      c_unary_predicate pred);

// Searches the range [first, last) for any of the elements in the range [s_first, s_last).
// Elements are compared using the given binary predicate.
// Returns true if any element is found.
// Sets found to the first element satisfies specific criteria.
bool algo_find_first_of_by(c_iterator_t* __c_input_iterator first,
                           c_iterator_t* __c_input_iterator last,
                           c_iterator_t* __c_forward_iterator s_first,
                           c_iterator_t* __c_forward_iterator s_last,
                           c_iterator_t** __c_input_iterator found,
                           c_binary_predicate pred);

// Searches the range [first, last) for any of the elements in the range [s_first, s_last).
// Elements are compared using the given binary predicate.
// Returns true if any element is found.
// Sets found to the last element satisfies specific criteria.
bool algo_find_last_of_by(c_iterator_t* __c_input_iterator first,
                          c_iterator_t* __c_input_iterator last,
                          c_iterator_t* __c_forward_iterator s_first,
                          c_iterator_t* __c_forward_iterator s_last,
                          c_iterator_t** __c_input_iterator found,
                          c_binary_predicate pred);

// Searches the range [first, last) for two consecutive identical elements.
// Returns true if there are such kind of elements.
// Sets found to the first element satisfies specific criteria.
bool algo_adjacent_find_by(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t** __c_forward_iterator found,
                           c_binary_predicate pred);

// Searches for the first occurrence of the subsequence of elements [s_first, s_last)
// in the range [first, last).
// Returns true if subsequence is found.
// Sets found to the first element satisfies specific criteria.
bool algo_search_by(c_iterator_t* __c_forward_iterator first,
                    c_iterator_t* __c_forward_iterator last,
                    c_iterator_t* __c_forward_iterator s_first,
                    c_iterator_t* __c_forward_iterator s_last,
                    c_iterator_t** __c_forward_iterator found,
                    c_binary_predicate pred);

// Searches for the first occurrence of the subsequence of elements [s_first, s_last)
// in the range [first, last).
// Returns true if subsequence is found.
// Sets found to the last element satisfies specific criteria.
bool algo_search_last_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t* __c_forward_iterator s_first,
                         c_iterator_t* __c_forward_iterator s_last,
                         c_iterator_t** __c_forward_iterator found,
                         c_binary_predicate pred);

// Searches the range [first, last) for the first sequence of count identical elements,
// each satisfies specific criteria.
// Returns true if subsequence is found.
// Sets found to the first element satisfies specific criteria.
bool algo_search_n_by(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      size_t n,
                      c_ref_t value,
                      c_iterator_t** __c_forward_iterator found,
                      c_binary_predicate pred);

// Searches the range [first, last) for the first sequence of count identical elements,
// each satisfies specific criteria.
// Returns true if subsequence is found.
// Sets found to the last element satisfies specific criteria.
bool algo_search_last_n_by(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           size_t n,
                           c_ref_t value,
                           c_iterator_t** __c_forward_iterator found,
                           c_binary_predicate pred);

// non-modifying helpers
#define c_algo_all_of(x, y, p)              algo_all_of(C_ITER_T(x), C_ITER_T(y), (p))
#define c_algo_any_of(x, y, p)              algo_any_of(C_ITER_T(x), C_ITER_T(y), (p))
#define c_algo_none_of(x, y, p)             algo_none_of(C_ITER_T(x), C_ITER_T(y), (p))
#define c_algo_for_each(x, y, f)            algo_for_each(C_ITER_T(x), C_ITER_T(y), (f))
#define c_algo_for_each_n(x, n, r, f)       algo_for_each_n(C_ITER_T(x), (n), C_ITER_PTR(r), (f))
#define c_algo_count(x, y, v)               algo_count(C_ITER_T(x), C_ITER_T(y), C_REF_T(v))
#define c_algo_count_if(x, y, p)            algo_count_if(C_ITER_T(x), C_ITER_T(y), (p))
#define c_algo_mismatch_by(x, y, z, mx, my, p) \
    algo_mismatch_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(z), C_ITER_PTR(mx), C_ITER_PTR(my), (p))
#define c_algo_equal_by(x, y, z, p)         algo_equal_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(z), (p))
#define c_algo_find_by(x, y, f, v, p)       algo_find_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(f), C_REF_T(v), (p))
#define c_algo_find_if(x, y, f, p)          algo_find_if(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(f), (p))
#define c_algo_find_if_not(x, y, f, p)      algo_find_if_not(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(f), (p))
#define c_algo_find_first_of_by(x, y, sx, sy, f, p) \
    algo_find_first_of_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(sx), C_ITER_T(sy), C_ITER_PTR(f), (p))
#define c_algo_find_last_of_by(x, y, sx, sy, f, p) \
    algo_find_last_of_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(sx), C_ITER_T(sy), C_ITER_PTR(f), (p))
#define c_algo_adjacent_find_by(x, y, f, p) algo_adjacent_find_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(f), (p))
#define c_algo_search_by(x, y, sx, sy, f, p) \
    algo_search_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(sx), C_ITER_T(sy), C_ITER_PTR(f), (p))
#define c_algo_search_last_by(x, y, sx, sy, f, p) \
    algo_search_last_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(sx), C_ITER_T(sy), C_ITER_PTR(f), (p))
#define c_algo_search_n_by(x, y, n, v, f, p) \
    algo_search_n_by(C_ITER_T(x), C_ITER_T(y), (n), C_REF_T(v), C_ITER_PTR(f), (p))
#define c_algo_search_last_n_by(x, y, n, v, f, p) \
    algo_search_last_n_by(C_ITER_T(x), C_ITER_T(y), (n), C_REF_T(v), C_ITER_PTR(f), (p))

#define c_algo_mismatch(x, y, z, mx, my)        c_algo_mismatch_by((x), (y), (z), (mx), (my), __c_get_equal(x))
#define c_algo_equal(x, y, z)                   c_algo_equal_by((x), (y), (z), __c_get_equal(x))
#define c_algo_find(x, y, f, v)                 c_algo_find_by((x), (y), (f), (v), __c_get_equal(x))
#define c_algo_find_first_of(x, y, sx, sy, f)   c_algo_find_first_of_by((x), (y), (sx), (sy), (f), __c_get_equal(x))
#define c_algo_find_last_of(x, y, sx, sy, f)    c_algo_find_last_of_by((x), (y), (sx), (sy), (f), __c_get_equal(x))
#define c_algo_adjacent_find(x, y, f)           c_algo_adjacent_find_by((x), (y), (f), __c_get_equal(x))
#define c_algo_search(x, y, sx, sy, f)          c_algo_search_by((x), (y), (sx), (sy), (f), __c_get_equal(x))
#define c_algo_search_last(x, y, sx, sy, f)     c_algo_search_last_by((x), (y), (sx), (sy), (f), __c_get_equal(x))
#define c_algo_search_n(x, y, n, v, f)          c_algo_search_n_by((x), (y), (n), (v), (f), __c_get_equal(x))
#define c_algo_search_last_n(x, y, n, v, f)     c_algo_search_last_n_by((x), (y), (n), (v), (f), __c_get_equal(x))

/*********************************/
/* modifying sequence operations */
/*********************************/
// Copies the elements in the range, defined by [first, last), to another range beginning at d_first.
// The behavior is undefined if d_first is within the range [first, last).
// In this case, algo_copy_backward may be used instead.
// Returns the number of elements copied.
// Sets d_last to the element in the destination range, one past the last element copied.
size_t algo_copy(c_iterator_t* __c_bidirection_iterator first,
                 c_iterator_t* __c_bidirection_iterator last,
                 c_iterator_t* __c_bidirection_iterator d_first,
                 c_iterator_t** __c_bidirection_iterator d_last);

// Only copies the elements for which the predicate returns true.
// The order of the elements that are not removed is preserved.
// The behavior is undefined if the source and the destination ranges overlap.
// Returns the number of elements copied.
// Sets d_last to the element in the destination range, one past the last element copied.
size_t algo_copy_if(c_iterator_t* __c_bidirection_iterator first,
                    c_iterator_t* __c_bidirection_iterator last,
                    c_iterator_t* __c_bidirection_iterator d_first,
                    c_iterator_t** __c_bidirection_iterator d_last,
                    c_unary_predicate pred);

// Copies the elements from the range, defined by [first, last), to another range ending at d_last.
// The elements are copied in reverse order (the last element is copied first), but their relative order is preserved.
// The behavior is undefined if d_last is within (first, last].
// algo_copy must be used instead of algo_copy_backward in that case.
// Returns number of elements copied.
// Sets d_last_copied to the last element copied.
size_t algo_copy_backward(c_iterator_t* __c_bidirection_iterator first,
                          c_iterator_t* __c_bidirection_iterator last,
                          c_iterator_t* __c_bidirection_iterator d_last,
                          c_iterator_t** __c_bidirection_iterator d_last_copied);

// TODO:
//size_t algo_move(c_iterator_t* __c_input_iterator first,
//                 c_iterator_t* __c_input_iterator last,
//                 c_iterator_t* __c_output_iterator d_first,
//                 c_iterator_t** __c_output_iterator_t d_last_moved);
//size_t algo_move_backward(c_iterator_t* __c_bidirection_iterator first,
//                          c_iterator_t* __c_bidirection_iterator last,
//                          c_iterator_t* __c_bidirection_iterator d_last,
//                          c_iterator_t** __c_bidirection_iterator d_last_moved);

// Assigns the given value to the elements in the range [first, last).
// Returns number of elements filled.
size_t algo_fill(c_iterator_t* __c_forward_iterator first,
                 c_iterator_t* __c_forward_iterator last,
                 c_ref_t value);

// Assigns the given value to the first count elements in the range beginning at first if n > 0.
// Does nothing otherwise.
// Sets last to one past the last element assigned if n > 0, first otherwise.
void algo_fill_n(c_iterator_t* __c_forward_iterator first,
                 size_t n,
                 c_ref_t value,
                 c_iterator_t** __c_forward_iterator last);

// Applies the given function to a range and stores the result in another range, beginning at d_first.
// Returns the number of elements transformed.
size_t algo_transform(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      c_iterator_t* __c_forward_iterator d_first,
                      c_unary_func op);

// Assigns each element in range [first, last) a value generated by the given function gen.
// Returns the number of elements generated.
size_t algo_generate(c_iterator_t* __c_forward_iterator first,
                     c_iterator_t* __c_forward_iterator last,
                     c_generator_emplace gen);

// Assigns values, generated by given function gen, to the first count elements in the range
// beginning at first, if count > 0. Does nothing otherwise.
// Sets last to one past the last element assigned if n > 0, first otherwise.
void algo_generate_n(c_iterator_t* __c_forward_iterator first,
                     size_t n,
                     c_generator_emplace gen,
                     c_iterator_t** __c_forward_iterator last);

// Removes all elements equal to value from the range [first, last).
// Returns the number of elements removed.
// Sets new_last to a past-the-end iterator for the new end of the range.
size_t algo_remove(c_iterator_t* __c_forward_iterator first,
                   c_iterator_t* __c_forward_iterator last,
                   c_ref_t value,
                   c_iterator_t** __c_forward_iterator new_last);

// Remove all elements satisfying specific criteria from range [first, last).
// Returns the number of elements removed.
// Sets new_last to a past-the-end iterator for the new end of the range.
size_t algo_remove_if(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      c_iterator_t** __c_forward_iterator new_last,
                      c_unary_predicate pred);

// Copies elements from the range [first, last), to another range beginning at d_first,
// omitting the elements equal to value. Source and destination ranges cannot overlap.
// Returns the number of elements copied.
// Sets d_last to iterator to the element past the last element copied.
size_t algo_remove_copy(c_iterator_t* __c_forward_iterator first,
                        c_iterator_t* __c_forward_iterator last,
                        c_iterator_t* __c_forward_iterator d_first,
                        c_ref_t value,
                        c_iterator_t** __c_forward_iterator d_last);

// Copies elements from the range [first, last), to another range beginning at d_first,
// omitting the elements which satify specific criteria. Source and destination ranges cannot overlap.
// Returns the number of elements copied.
// Sets d_last to the element past the last element copied.
size_t algo_remove_copy_if(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t* __c_forward_iterator d_first,
                           c_iterator_t** __c_forward_iterator d_last,
                           c_unary_predicate pred);

// Replaces all elements that are equal to old_value with new_value in the range [first, last).
// Returns the number of elements replaced.
size_t algo_replace(c_iterator_t* __c_forward_iterator first,
                    c_iterator_t* __c_forward_iterator last,
                    c_ref_t old_value,
                    c_ref_t new_value);

// Replaces all elements for which predicate returns true with new_value in the range [first, last).
// Returns the number of elements replaced.
size_t algo_replace_if(c_iterator_t* __c_forward_iterator first,
                       c_iterator_t* __c_forward_iterator last,
                       c_unary_predicate pred,
                       c_ref_t new_value);

// Copies the all elements from the range [first, last) to another range beginning at d_first
// replacing all elements that are equal to old_value with new_value.
// The source and destination ranges cannot overlap.
// Returns the number of elements copied.
// Sets d_last to the element past the last element copied.
size_t algo_replace_copy(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t* __c_forward_iterator d_first,
                         c_ref_t old_value,
                         c_ref_t new_value,
                         c_iterator_t** __c_forward_iterator d_last);

// Copies the all elements from the range [first, last) to another range beginning at d_first
// replacing all elements for which predicate returns true with new_value.
// The source and destination ranges cannot overlap.
// Returns the number of elements copied.
// Sets d_last to the element past the last element copied.
size_t algo_replace_copy_if(c_iterator_t* __c_forward_iterator first,
                            c_iterator_t* __c_forward_iterator last,
                            c_iterator_t* __c_forward_iterator d_first,
                            c_unary_predicate pred,
                            c_ref_t new_value,
                            c_iterator_t** __c_forward_iterator d_last);

// Exchanges the given values.
void algo_swap(c_containable_t* type_info,
               c_ref_t x,
               c_ref_t y);

// Exchanges elements between range [first, last) and another range starting at first2.
// Returns the number of elements swapped.
size_t algo_swap_range(c_iterator_t* __c_forward_iterator first,
                       c_iterator_t* __c_forward_iterator last,
                       c_iterator_t* __c_forward_iterator first2);

// Swaps the values of the elements the given iterators are pointing to.
void algo_iter_swap(c_iterator_t* __c_forward_iterator x,
                    c_iterator_t* __c_forward_iterator y);

// Reverses the order of the elements in the range [first, last)
// Behaves as if applying algo_iter_swap to every pair of iterators
// first+i, (last-i) - 1 for each non-negative i < (last-first)/2.
// Returns the number of elements reversed.
size_t algo_reverse(c_iterator_t* __c_bidirection_iterator first,
                    c_iterator_t* __c_bidirection_iterator last);

// Copies the elements from the range [first, last) to another range
// beginning at d_first in such a way that the elements in the new range are in reverse order.
// Behaves as if by executing the assignment *(d_first + (last - first) - 1 - i) = *(first + i)
// once for each non-negative i < (last - first).
// If the source and destination ranges (that is, [first, last) and [d_first, d_first+(last-first)) respectively)
// overlap, the behavior is undefined.
// Returns the number of elements copied.
// Sets d_last to the element past the last element copied.
size_t algo_reverse_copy(c_iterator_t* __c_bidirection_iterator first,
                         c_iterator_t* __c_bidirection_iterator last,
                         c_iterator_t* __c_forward_iterator d_first,
                         c_iterator_t** __c_forward_iterator d_last);

// Performs a left rotation on a range of elements.
// Specifically, algo_rotate swaps the elements in the range [first, last)
// in such a way that the element n_first becomes the first element of the new range and n_first - 1 becomes the last element.
// A precondition of this function is that [first, n_first) and [n_first, last) are valid ranges.
// Sets rotate_point to the iterator equal to first + (last - n_first)
void algo_rotate(c_iterator_t* __c_forward_iterator first,
                 c_iterator_t* __c_forward_iterator n_first,
                 c_iterator_t* __c_forward_iterator last,
                 c_iterator_t** __c_forward_iterator rotate_point);

// Copies the elements from the range [first, last), to another range
// beginning at d_first in such a way, that the element n_first becomes the first element
// of the new range and n_first - 1 becomes the last element.
// Returns the number of elements copied.
// Sets d_last to the element past the last element copied.
size_t algo_rotate_copy(c_iterator_t* __c_forward_iterator first,
                        c_iterator_t* __c_forward_iterator n_first,
                        c_iterator_t* __c_forward_iterator last,
                        c_iterator_t* __c_forward_iterator d_first,
                        c_iterator_t** __c_forward_iterator d_last);

// Reorders the elements in the given range [first, last) such that
// each possible permutation of those elements has equal probability of appearance.
void algo_random_shuffle(c_iterator_t* __c_random_iterator first,
                         c_iterator_t* __c_random_iterator last);
void algo_random_shuffle_by(c_iterator_t* __c_random_iterator first,
                            c_iterator_t* __c_random_iterator last,
                            c_random_func r);

// Eliminates all but the first element from every consecutive group of equivalent elements
// from the range [first, last).
// Removing is done by shifting the elements in the range in such a way that
// elements to be erased are overwritten. Relative order of the elements that remain is preserved
// and the physical size of the container is unchanged.
// Iterators pointing to an element between the new logical end and the physical end of the range
// are still dereferenceable, the elements themselves still have original values.
// A call to unique is typically followed by a call to a container's erase method,
// which erases the values from the new logical end and reduces the physical size of the container to match its new logical size.
// Returns the number of elements after elimination.
// Sets new_last to a past-the-end iterator for the new logical end of the range.
size_t algo_unique_by(c_iterator_t* __c_forward_iterator first,
                      c_iterator_t* __c_forward_iterator last,
                      c_iterator_t** __c_forward_iterator new_last,
                      c_binary_predicate pred);

// Copies the elements from the range [first, last), to another range
// beginning at d_first in such a way that there are no consecutive equal elements.
// Only the first element of each group of equal elements is copied.
// Returns the number of elements copied.
// Sets d_last to the element past the last written element.
size_t algo_unique_copy_by(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t* __c_forward_iterator d_first,
                           c_iterator_t** __c_forward_iterator d_last,
                           c_binary_predicate pred);

// modifying helpers
#define c_algo_copy(x, y, d, c)                 algo_copy(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), C_ITER_PTR(c))
#define c_algo_copy_if(x, y, d, c, p)           algo_copy_if(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), C_ITER_PTR(c), (p))
#define c_algo_copy_backward(x, y, d, c)        algo_copy_backward(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), C_ITER_PTR(c))
#define c_algo_fill(x, y, v)                    algo_fill(C_ITER_T(x), C_ITER_T(y), C_REF_T(v))
#define c_algo_fill_n(x, n, v, f)               algo_fill_n(C_ITER_T(x), (n), C_REF_T(v), C_ITER_PTR(f))
#define c_algo_transform(x, y, d, f)            algo_transform(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), (f))
#define c_algo_generate(x, y, g)                algo_generate(C_ITER_T(x), C_ITER_T(y), (g))
#define c_algo_generate_n(x, n, g, l)           algo_generate_n(C_ITER_T(x), (n), (g), C_ITER_PTR(l))
#define c_algo_remove(x, y, v, n)               algo_remove(C_ITER_T(x), C_ITER_T(y), C_REF_T(v), C_ITER_PTR(n))
#define c_algo_remove_if(x, y, n, p)            algo_remove_if(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(n), (p))
#define c_algo_remove_copy(x, y, d, v, c)       algo_remove_copy(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), C_REF_T(v), C_ITER_PTR(c))
#define c_algo_remove_copy_if(x, y, d, c, p)    algo_remove_copy_if(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), C_ITER_PTR(c), (p))
#define c_algo_replace(x, y, o, n)              algo_replace(C_ITER_T(x), C_ITER_T(y), C_REF_T(o), C_REF_T(n))
#define c_algo_replace_if(x, y, p, n)           algo_replace_if(C_ITER_T(x), C_ITER_T(y), (p), C_REF_T(n))
#define c_algo_replace_copy(x, y, d, o, n, c)   algo_replace_copy(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), C_REF_T(o), C_REF_T(n), C_ITER_PTR(c))
#define c_algo_replace_copy_if(x, y, d, p, n, c)algo_replace_copy_if(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), (p), C_REF_T(n), C_ITER_PTR(c))
#define c_algo_swap(t, x, y)                    algo_swap((t), C_REF_T(x), C_REF_T(y))
#define c_algo_swap_range(x, y, z)              algo_swap_range(C_ITER_T(x), C_ITER_T(y), C_ITER_T(z))
#define c_algo_iter_swap(x, y)                  algo_iter_swap(C_ITER_T(x), C_ITER_T(y))
#define c_algo_reverse(x, y)                    algo_reverse(C_ITER_T(x), C_ITER_T(y))
#define c_algo_reverse_copy(x, y, d, c)         algo_reverse_copy(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), C_ITER_PTR(c))
#define c_algo_rotate(x, n, y, r)               algo_rotate(C_ITER_T(x), C_ITER_T(n), C_ITER_T(y), C_ITER_PTR(r))
#define c_algo_rotate_copy(x, n, y, d, c)       algo_rotate_copy(C_ITER_T(x), C_ITER_T(n), C_ITER_T(y), C_ITER_T(d), C_ITER_PTR(c))
#define c_algo_random_shuffle(x, y)             algo_random_shuffle(C_ITER_T(x), C_ITER_T(y))
#define c_algo_random_shuffle_by(x, y, r)       algo_random_shuffle_by(C_ITER_T(x), C_ITER_T(y), (r))
#define c_algo_unique_by(x, y, n, p)            algo_unique_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(n), (p))
#define c_algo_unique_copy_by(x, y, d, c, p)    algo_unique_copy_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), C_ITER_PTR(c), (p))

#define c_algo_unique(x, y, n)                  c_algo_unique_by((x), (y), (n), __c_get_equal(x))
#define c_algo_unique_copy(x, y, d, c)          c_algo_unique_copy_by((x), (y), (d), (c), __c_get_equal(x))

/************************/
/* partition operations */
/************************/
// Returns true if all elements in the range [first, last) that satisfy the predicate
// appear before all elements that don't. Also returns true if [first, last) is empty.
bool algo_is_partitioned(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_unary_predicate pred);

// Reorders the elements in the range [first, last) in such a way that all elements for which
// the predicate returns true precede the elements for which predicate returns false.
// Relative order of the elements is not preserved.
// Sets second_first to the first element of the second group.
void algo_partition(c_iterator_t* __c_forward_iterator first,
                    c_iterator_t* __c_forward_iterator last,
                    c_iterator_t** __c_forward_iterator second_first,
                    c_unary_predicate pred);

// Copies the elements from the range [first, last) to two different ranges
// depending on the value returned by the predicate. The elements, that satisfy the predicate,
// are copied to the range beginning at d_first_true. The rest of the elements are copied to the range beginning at d_first_false.
// The behavior is undefined if the input range overlaps either of the output ranges.
// Returns the number of total elements copied to both two ranges.
// Sets d_last_true to the end of the d_first_true range.
// Sets d_last_false to the end of the d_first_false range.
size_t algo_partition_copy(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_iterator_t* __c_forward_iterator d_first_true,
                           c_iterator_t* __c_forward_iterator d_first_false,
                           c_iterator_t** __c_forward_iterator d_last_true,
                           c_iterator_t** __c_forward_iterator d_last_false,
                           c_unary_predicate pred);

// Examines the partitioned (as if by algo_partition) range [first, last)
// and locates the end of the first partition, that is, the first element that
// does not satisfy the given predicate or last if all elements satisfy predicate.
// Sets first_last to past the end of the first partition within [first, last) or last if all elements satisfy predicate.
void algo_partition_point(c_iterator_t* __c_forward_iterator first,
                          c_iterator_t* __c_forward_iterator last,
                          c_iterator_t** __c_forward_iterator first_last,
                          c_unary_predicate pred);

// partition helpers
#define c_algo_is_partitioned(x, y, p)          algo_is_partitioned(C_ITER_T(x), C_ITER_T(y), (p))
#define c_algo_partition(x, y, s, p)            algo_partition(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(s), (p))
#define c_algo_partition_copy(x, y, ft, ff, lt, lf, p) \
    algo_partition_copy(C_ITER_T(x), C_ITER_T(y), C_ITER_T(ft), C_ITER_T(ff), C_ITER_PTR(lt), C_ITER_PTR(lf), (p))
#define c_algo_partition_point(x, y, l, p)      algo_partition_point(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(l), (p))

/**********************/
/* sorting operations */
/**********************/
// Checks if the elements in range [first, last) are sorted in non-descending order.
// Elements are compared using the given binary comparison function comp.
bool algo_is_sorted_by(c_iterator_t* __c_forward_iterator first,
                       c_iterator_t* __c_forward_iterator last,
                       c_compare comp);

// Examines the range [first, last) and finds the largest range beginning at first
// in which the elements are sorted in ascending order.
// Elements are compared using the given binary comparison function comp.
// Sets until to the upper bound of the largest range beginning at first in which
// the elements are sorted in ascending order. That is, the last iterator it for which range [first, it) is sorted.
void algo_is_sorted_until_by(c_iterator_t* __c_forward_iterator first,
                             c_iterator_t* __c_forward_iterator last,
                             c_iterator_t** __c_forward_iterator until,
                             c_compare comp);

// sorting helpers
#define c_algo_is_sorted_by(x, y, c)            algo_is_sorted_by(C_ITER_T(x), C_ITER_T(y), (c))
#define c_algo_is_sorted_until_by(x, y, u, c)   algo_is_sorted_until_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(u), (c))

#define c_algo_is_sorted(x, y)                  c_algo_is_sorted_by((x), (y), __c_get_less(x))
#define c_algo_is_sorted_until(x, y, u)         c_algo_is_sorted_until_by((x), (y), (u), __c_get_less(x))

/***********************************************/
/* binary search operations (on sorted ranges) */
/***********************************************/
// Sets bound to an iterator pointing to the first element in the range [first, last)
// that is not less than (i.e. greater or equal to) value.
// The range [first, last) must be at least partially ordered, i.e. partitioned with respect
// to the expression comp(element, value).
// A fully-sorted range meets this criterion, as does a range resulting from a call to algo_partition.
void algo_lower_bound_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_ref_t value,
                         c_iterator_t** __c_forward_iterator bound,
                         c_compare comp);

// Sets bound to an iterator pointing to the first element in the range [first, last)
// that is greater than value.
// The range [first, last) must be at least partially ordered, i.e. partitioned with respect
// to the expression !comp(value, element).
// A fully-sorted range meets this criterion, as does a range resulting from a call to algo_partition.
void algo_upper_bound_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_ref_t value,
                         c_iterator_t** __c_forward_iterator bound,
                         c_compare comp);

// Checks if an element equivalent to value appears within the range [first, last).
// For algo_binary_search to succeed, the range [first, last) must be at least partially ordered,
// i.e. it must satisfy all of the following requirements:
// (1) partitioned with respect to element < value or comp(element, value)
// (2) partitioned with respect to !(value < element) or !comp(value, element)
// (3) for all elements, if element < value or comp(element, value) is true
//     then !(value < element) or !comp(value, element) is also true
// A fully-sorted range meets these criteria, as does a range resulting from a call to algo_partition.
bool algo_binary_search_by(c_iterator_t* __c_forward_iterator first,
                           c_iterator_t* __c_forward_iterator last,
                           c_ref_t value,
                           c_compare comp);

// Returns a range containing all elements equivalent to value in the range [first, last).
// The range [first, last) must be partitioned with respect to comparison with value,
// i.e. it must satisfy all of the following requirements:
// (1) partitioned with respect to element < value or comp(element, value)
// (2) partitioned with respect to !(value < element) or !comp(value, element)
// (3) for all elements, if element < value or comp(element, value) is true
//     then !(value < element) or !comp(value, element) is also true
// A fully-sorted range meets these criteria, as does a range resulting from a call to algo_partition.
// The returned range is defined by two iterators, one pointing to the first element
// that is not less than value and another pointing to the first element greater than value.
// The first iterator may be alternatively obtained with algo_lower_bound(), the second - with algo_upper_bound().
void algo_equal_range_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_ref_t value,
                         c_iterator_t** __c_forward_iterator lower_bound,
                         c_iterator_t** __c_forward_iterator upper_bound,
                         c_compare comp);

// binary helpers
#define c_algo_lower_bound_by(x, y, v, b, c)    algo_lower_bound_by(C_ITER_T(x), C_ITER_T(y), C_REF_T(v), C_ITER_PTR(b), (c))
#define c_algo_upper_bound_by(x, y, v, b, c)    algo_upper_bound_by(C_ITER_T(x), C_ITER_T(y), C_REF_T(v), C_ITER_PTR(b), (c))
#define c_algo_binary_search_by(x, y, v, c)     algo_binary_search_by(C_ITER_T(x), C_ITER_T(y), C_REF_T(v), (c))
#define c_algo_equal_range_by(x, y, v, l, u, c) algo_equal_range_by(C_ITER_T(x), C_ITER_T(y), C_REF_T(v), C_ITER_PTR(l), C_ITER_PTR(u), (c))

#define c_algo_lower_bound(x, y, v, b)          c_algo_lower_bound_by((x), (y), (v), (b), __c_get_less(x))
#define c_algo_upper_bound(x, y, v, b)          c_algo_upper_bound_by((x), (y), (v), (b), __c_get_less(x))
#define c_algo_binary_search(x, y, v)           c_algo_binary_search_by((x), (y), (v), __c_get_less(x))
#define c_algo_equal_range(x, y, v, l, u)       c_algo_equal_range_by((x), (y), (v), (l), (u), __c_get_less(x))

/*************************************/
/* set operations (on sorted ranges) */
/*************************************/

/*******************/
/* heap operations */
/*******************/
// Checks if the elements in range [first, last) are a max heap.
// Elements are compared using comp.
bool algo_is_heap_by(c_iterator_t* __c_random_iterator first,
                     c_iterator_t* __c_random_iterator last,
                     c_compare comp);

// Examines the range [first, last) and finds the largest range beginning at first which is a max heap.
// Elements are compared using comp.
// Sets until to the upper bound of the largest range beginning at first which is a max heap.
// That is, the last iterator it for which range [first, it) is a max heap.
void algo_is_heap_until_by(c_iterator_t* __c_random_iterator first,
                           c_iterator_t* __c_random_iterator last,
                           c_iterator_t** __c_random_iterator until,
                           c_compare comp);

// Constructs a max heap in the range [first, last).
// Elements are compared using comp.
void algo_make_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp);

// Inserts the element at the position last-1 into the max heap defined by the range [first, last-1).
// Elements are compared using comp.
void algo_push_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp);

// Swaps the value in the position first and the value in the position last-1 and
// makes the subrange [first, last-1) into a max heap.
// This has the effect of removing the first (largest) element from the heap defined by the range [first, last).
// Elements are compared using comp.
void algo_pop_heap_by(c_iterator_t* __c_random_iterator first,
                      c_iterator_t* __c_random_iterator last,
                      c_compare comp);

// Converts the max heap [first, last) into a sorted range in ascending order.
// The resulting range no longer has the heap property.
// Elements are compared using comp.
void algo_sort_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp);

// heap helpers
#define c_algo_is_heap_by(x, y, c)              algo_is_heap_by(C_ITER_T(x), C_ITER_T(y), c)
#define c_algo_is_heap_until_by(x, y, z, c)     algo_is_heap_until_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(z), (c))
#define c_algo_make_heap_by(x, y, c)            algo_make_heap_by(C_ITER_T(x), C_ITER_T(y), (c))
#define c_algo_push_heap_by(x, y, c)            algo_push_heap_by(C_ITER_T(x), C_ITER_T(y), (c))
#define c_algo_pop_heap_by(x, y, c)             algo_pop_heap_by(C_ITER_T(x), C_ITER_T(y), (c))
#define c_algo_sort_heap_by(x, y, c)            algo_sort_heap_by(C_ITER_T(x), C_ITER_T(y), (c))

#define c_algo_is_heap(x, y)                    c_algo_is_heap_by((x), (y), __c_get_less(x))
#define c_algo_is_heap_until(x, y, z)           c_algo_is_heap_until_by((x), (y), (z), __c_get_less(x))
#define c_algo_make_heap(x, y)                  c_algo_make_heap_by((x), (y), __c_get_less(x))
#define c_algo_push_heap(x, y)                  c_algo_push_heap_by((x), (y), __c_get_less(x))
#define c_algo_pop_heap(x, y)                   c_algo_pop_heap_by((x), (y), __c_get_less(x))
#define c_algo_sort_heap(x, y)                  c_algo_sort_heap_by((x), (y), __c_get_less(x))

/******************************/
/* minimum/maximum operations */
/******************************/
// Returns the greater of the given values.
// The return value is a copy of the greater one which allocated on the heap.
// It must be deallocated after use.
c_ref_t algo_max_by(c_containable_t* type_info,
                    c_ref_t x,
                    c_ref_t y,
                    c_compare comp);

// Returns the smaller of the given values.
// The return value is a copy of the smaller one which allocated on the heap.
// It must be deallocated after use.
c_ref_t algo_min_by(c_containable_t* type_info,
                    c_ref_t x,
                    c_ref_t y,
                    c_compare comp);

// Finds the greatest element in the range [first, last).
// Elements are compared using the given binary comparison function.
// Sets max to the maximum element.
void algo_max_element_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t** __c_forward_iterator max,
                         c_compare comp);

// Finds the smallest element in the range [first, last).
// Elements are compared using the given binary comparison function.
// Sets min to the minimum element.
void algo_min_element_by(c_iterator_t* __c_forward_iterator first,
                         c_iterator_t* __c_forward_iterator last,
                         c_iterator_t** __c_forward_iterator mix,
                         c_compare comp);

// Finds the greatest and smallest element in the range [first, last).
// Elements are compared using the given binary comparison function.
// Sets min to the minimum element.
// Sets max to the maximum element.
void algo_minmax_element_by(c_iterator_t* __c_forward_iterator first,
                            c_iterator_t* __c_forward_iterator last,
                            c_iterator_t** __c_forward_iterator min,
                            c_iterator_t** __c_forward_iterator max,
                            c_compare comp);

// If v compares less than hi, returns the larger of v and lo,
// otherwise returns the smaller of v and hi. Uses comp to compare the values.
// The behavior is undefined if the value of lo is greater than hi.
// The return value is a copy of the chosen one which allocated on the heap.
// It must be deallocated after use.
c_ref_t algo_clamp_by(c_containable_t* type_info,
                      c_ref_t v,
                      c_ref_t lo,
                      c_ref_t hi,
                      c_compare comp);

// min/max helpers
#define c_algo_max_by(t, x, y, c)               algo_max_by((t), C_REF_T(x), C_REF_T(y), (c))
#define c_algo_min_by(t, x, y, c)               algo_min_by((t), C_REF_T(x), C_REF_T(y), (c))
#define c_algo_max_element_by(x, y, m, c)       algo_max_element_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(m), (c))
#define c_algo_min_element_by(x, y, m, c)       algo_min_element_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(m), (c))
#define c_algo_minmax_element_by(x, y, s, g, c) algo_minmax_element_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(s), C_ITER_PTR(g), (c))
#define c_algo_clamp_by(t, v, l, h, c)          algo_clamp_by((t), C_REF_T(v), C_REF_T(l), C_REF_T(h), (c))

#define c_algo_max(t, x, y)                     c_algo_max_by((t), (x), (y), (t)->less)
#define c_algo_min(t, x, y)                     c_algo_min_by((t), (x), (y), (t)->less)
#define c_algo_max_element(x, y, m)             c_algo_max_element_by((x), (y), (m), __c_get_less(x))
#define c_algo_min_element(x, y, m)             c_algo_min_element_by((x), (y), (m), __c_get_less(x))
#define c_algo_minmax_element(x, y, s, g)       c_algo_minmax_element_by((x), (y), (s), (g), __c_get_less(x))
#define c_algo_clamp(t, v, l, h)                c_algo_clamp_by((t), (v), (l), (h), (t)->less)

/**********************/
/* numeric operations */
/**********************/

/**************************************/
/* operations on uninitialized memory */
/**************************************/

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_ALGORITHM_H__
