
#include <string.h>
#include "c_internal.h"
#include "c_iterator.h"

bool type_info_equal(c_containable_t* x, c_containable_t* y)
{
    return (!x || !y) ? false : (memcmp(x, y, sizeof(*x)) == 0);
}

void c_iter_copy(c_iterator_t** iter, c_iterator_t* other)
{
    if (other) other->iterator_ops->alloc_and_copy(iter, other);
}

c_iterator_t* c_iter_increment(c_iterator_t* iter)
{
    return (!iter) ? 0 : iter->iterator_ops->increment(iter);
}

c_iterator_t* c_iter_decrement(c_iterator_t* iter)
{
    return (!iter) ? 0 : iter->iterator_ops->decrement(iter);
}

c_ref_t c_iter_dereference(c_iterator_t* iter)
{
    return (!iter) ? 0 : iter->iterator_ops->dereference(iter);
}

bool c_iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!x || !y) return false;
    if (x->iterator_category != y->iterator_category ||
        x->iterator_type != y->iterator_type) {
        return false;
    }

    return x->iterator_ops->equal(x, y);
}

bool c_iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !c_iter_equal(x, y);
}

void c_iter_advance(c_iterator_t* iter, size_t n)
{
    if (!iter) return;
    iter->iterator_ops->advance(iter, n);
}

size_t c_iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    return (!first || !last) ? 0 : first->iterator_ops->distance(first, last);
}

bool c_iter_category_at_least(c_iterator_t* iter, c_iterator_category_t cate)
{
    return iter ? iter->iterator_category >= cate : false;
}

bool c_iter_category_exact(c_iterator_t* iter, c_iterator_category_t cate)
{
    return iter ? iter->iterator_category == cate : false;
}
