
#include <string.h>
#include "c_internal.h"
#include "c_iterator.h"

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
    if (strcmp(x->iterator_category, y->iterator_category) ||
        strcmp(x->iterator_type, y->iterator_type)) {
        return false;
    }

    return x->equal(x, y);
}

bool c_iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (strcmp(x->iterator_category, y->iterator_category) ||
        strcmp(x->iterator_type, y->iterator_type)) {
        return true;
    }

    return x->not_equal(x, y);
}
