

#ifndef __C_ALGORITHM_H__
#define __C_ALGORITHM_H__

#include "c_def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// non-modifying sequence operations
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

// modifying sequence operations
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
// Returns the iterator equal to first + (last - n_first)
void algo_rotate(c_iterator_t* __c_forward_iterator first,
                 c_iterator_t* __c_forward_iterator n_first,
                 c_iterator_t* __c_forward_iterator last,
                 c_iterator_t** __c_forward_iterator rotated);
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
#define c_algo_unique_by(x, y, n, p)            algo_unique_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(n), (p))
#define c_algo_unique_copy_by(x, y, d, c, p)    algo_unique_copy_by(C_ITER_T(x), C_ITER_T(y), C_ITER_T(d), C_ITER_PTR(c), (p))

#define c_algo_unique(x, y, n)                  c_algo_unique_by((x), (y), (n), __c_get_equal(x))
#define c_algo_unique_copy(x, y, d, c)          c_algo_unique_copy_by((x), (y), (d), (c), __c_get_equal(x))


// partition operations
// sorting operations
// binary search operations (on sorted ranges)
// set operations (on sorted ranges)
// heap operations
bool algo_is_heap_by(c_iterator_t* __c_random_iterator first,
                     c_iterator_t* __c_random_iterator last,
                     c_compare comp);
void algo_is_heap_until_by(c_iterator_t* __c_random_iterator first,
                           c_iterator_t* __c_random_iterator last,
                           c_iterator_t** __c_random_iterator until,
                           c_compare comp);
void algo_make_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp);
void algo_push_heap_by(c_iterator_t* __c_random_iterator first,
                       c_iterator_t* __c_random_iterator last,
                       c_compare comp);
void algo_pop_heap_by(c_iterator_t* __c_random_iterator first,
                      c_iterator_t* __c_random_iterator last,
                      c_compare comp);
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

// minimum/maximum operations
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

// min/max helpers
#define c_algo_max_element_by(x, y, m, c)       algo_max_element_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(m), (c))
#define c_algo_min_element_by(x, y, m, c)       algo_min_element_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(m), (c))
#define c_algo_minmax_element_by(x, y, s, g, c) algo_minmax_element_by(C_ITER_T(x), C_ITER_T(y), C_ITER_PTR(s), C_ITER_PTR(g), (c))

#define c_algo_max(x, y) (((x) < (y)) ? (y) : (x))
#define c_algo_min(x, y) (((x) < (y)) ? (x) : (y))
#define c_algo_max_element(x, y, m)             c_algo_max_element_by((x), (y), (m), __c_get_less(x))
#define c_algo_min_element(x, y, m)             c_algo_min_element_by((x), (y), (m), __c_get_less(x))
#define c_algo_minmax_element(x, y, s, g)       c_algo_minmax_element_by((x), (y), (s), (g), __c_get_less(x))

// numeric operations
// operations on uninitialized memory

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __C_ALGORITHM_H__
