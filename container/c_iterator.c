
#include "c_internal.h"
#include "c_iterator.h"

bool type_info_equal(c_containable_t* x, c_containable_t* y)
{
    if (!x || !y) return false;
    return (x->assign == y->assign &&
            x->copy == y->copy &&
            x->create == y->create &&
            x->destroy == y->destroy &&
            x->equal == y->equal &&
            x->less == y->less &&
            x->size == y->size);
}

c_iterator_t* c_iter_increment(c_iterator_t* iter)
{
    return iter->increment(iter);
}

c_iterator_t* c_iter_decrement(c_iterator_t* iter)
{
    return iter->decrement(iter);
}

c_ref_t c_iter_dereference(c_iterator_t* iter)
{
    return iter->dereference(iter);
}

bool c_iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (x->iterator_category != y->iterator_category ||
        x->iterator_type != y->iterator_type) {
        return false;
    }

    return x->equal(x, y);
}

bool c_iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (x->iterator_category != y->iterator_category ||
        x->iterator_type != y->iterator_type) {
        return true;
    }

    return x->not_equal(x, y);
}
