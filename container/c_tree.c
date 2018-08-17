/**
 * MIT License
 *
 * Copyright (c) 2017 MatrixJoeQ
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "c_internal.h"
#include "c_tree.h"

typedef bool __rb_tree_color_type;

struct __c_tree_node {
    __rb_tree_color_type color;
    struct __c_tree_node* parent;
    struct __c_tree_node* left;
    struct __c_tree_node* right;
    c_ref_t value;
};

struct __c_tree {
    const c_type_info_t* key_type;
    const c_type_info_t* value_type;
    const c_type_info_t* mapped_type;
    c_key_of_value key_of_value;
    c_compare key_comp;
    c_tree_node_t* header;
    size_t node_count;
};

static const __rb_tree_color_type s_rb_tree_color_red = false;
static const __rb_tree_color_type s_rb_tree_color_black = true;

__c_static c_tree_iterator_t __create_iterator(const c_type_info_t* value_type, c_tree_node_t* node);
__c_static c_tree_iterator_t __create_reverse_iterator(const c_type_info_t* value_type, c_tree_node_t* node);

__c_static __c_inline bool __is_header(c_tree_node_t* node)
{
    return (node ? (node->parent->parent == node &&
                    node->color == s_rb_tree_color_red) : false);
}

__c_static __c_inline bool __is_root(c_tree_node_t* node)
{
    return (node ? (node->parent->parent == node &&
                    node->color == s_rb_tree_color_black) : false);
}

__c_static __c_inline bool __is_left(c_tree_node_t* node)
{
    return (node ? node->parent->left == node : false);
}

__c_static __c_inline bool __is_right(c_tree_node_t* node)
{
    return (node ? node->parent->right == node : false);
}

__c_static __c_inline bool __is_black(c_tree_node_t* node)
{
    return (node ? (node->color == s_rb_tree_color_black) : true);
}

__c_static __c_inline bool __is_red(c_tree_node_t* node)
{
    return (node ? (node->color == s_rb_tree_color_red) : false);
}

__c_static __c_inline bool __has_left(c_tree_node_t* node)
{
    return (node ? node->left != 0 : false);
}

__c_static __c_inline bool __has_right(c_tree_node_t* node)
{
    return (node ? node->right != 0 : false);
}

__c_static __c_inline c_tree_node_t* __left(c_tree_node_t* node)
{
    return node->left;
}

__c_static __c_inline c_tree_node_t* __right(c_tree_node_t* node)
{
    return node->right;
}

__c_static __c_inline c_tree_node_t* __parent(c_tree_node_t* node)
{
    return node->parent;
}

__c_static __c_inline c_tree_node_t* __grand_parent(c_tree_node_t* node)
{
    return node->parent->parent;
}

__c_static __c_inline c_tree_node_t* __uncle(c_tree_node_t* node)
{
    return (__is_left(__parent(node)) ? __right(__grand_parent(node))
                                      : __left(__grand_parent(node)));
}

__c_static __c_inline c_tree_node_t* __sibling(c_tree_node_t* node)
{
    return (__is_left(node) ? __right(__parent(node))
                            : __left(__parent(node)));
}

__c_static __c_inline __rb_tree_color_type __color(c_tree_node_t* node)
{
    return node->color;
}

__c_static __c_inline c_ref_t __value(c_tree_node_t* node)
{
    return node->value;
}

__c_static __c_inline c_tree_node_t* __header(c_tree_t* tree)
{
    return tree->header;
}

__c_static __c_inline c_tree_node_t* __root(c_tree_t* tree)
{
    return tree->header->parent;
}

__c_static __c_inline c_tree_node_t* __leftmost(c_tree_t* tree)
{
    return tree->header->left;
}

__c_static __c_inline c_tree_node_t* __rightmost(c_tree_t* tree)
{
    return tree->header->right;
}

__c_static __c_inline void __set_root(c_tree_t* tree, c_tree_node_t* root)
{
    tree->header->parent = root;
}

__c_static __c_inline void __set_leftmost(c_tree_t* tree, c_tree_node_t* leftmost)
{
    tree->header->left = leftmost;
}

__c_static __c_inline void __set_rightmost(c_tree_t* tree, c_tree_node_t* rightmost)
{
    tree->header->right = rightmost;
}

__c_static __c_inline c_tree_node_t* __minimum(c_tree_node_t* node)
{
    if (!node) return 0;

    while (node->left)
        node = node->left;

    return node;
}

__c_static __c_inline c_tree_node_t* __maximum(c_tree_node_t* node)
{
    if (!node) return 0;

    while (node->right)
        node = node->right;

    return node;
}

__c_static __c_inline void __rotate_right(c_tree_node_t* node, c_tree_node_t* root)
{
    if (!node || !node->left) return;

    c_tree_node_t* x = node;
    c_tree_node_t* y = x->left;

    x->left = y->right;
    if (y->right)
        y->right->parent = x;

    if (x == root)
        x->parent->parent = y;
    else if (__is_left(x))
        x->parent->left = y;
    else if (__is_right(x))
        x->parent->right = y;

    y->parent = x->parent;
    y->right = x;
    x->parent = y;
}

__c_static __c_inline void __rotate_left(c_tree_node_t* node, c_tree_node_t* root)
{
    if (!node || !node->right) return;

    c_tree_node_t* x = node;
    c_tree_node_t* y = x->right;

    x->right = y->left;
    if (y->left)
        y->left->parent = x;

    if (x == root)
        x->parent->parent = y;
    else if (__is_left(x))
        x->parent->left = y;
    else if (__is_right(x))
        x->parent->right = y;

    y->parent = x->parent;
    y->left = x;
    x->parent = y;
}

__c_static __c_inline c_tree_node_t* __create_node(c_tree_t* tree, c_ref_t value)
{
    assert(tree);

    const c_type_info_t* value_type = tree->value_type;
    assert(value_type);

    c_tree_node_t* node = (c_tree_node_t*)malloc(sizeof(c_tree_node_t));
    if (!node) return 0;

    node->value = malloc(value_type->size());
    if (!node->value) {
        __c_free(node);
        return 0;
    }

    node->parent = 0;
    node->left   = 0;
    node->right  = 0;
    node->color  = s_rb_tree_color_red;

    if (value)
        value_type->copy(node->value, value);
    else {
        if (tree->mapped_type) {
            ((c_pair_t*)(node->value))->first_type = tree->key_type;
            ((c_pair_t*)(node->value))->second_type = tree->mapped_type;
        }
        value_type->create(node->value);
    }

    return node;
}

__c_static __c_inline void __destroy_node(c_tree_t* tree, c_tree_node_t* node)
{
    assert(tree);
    assert(node);

    tree->value_type->destroy(node->value);
    __c_free(node->value);
    __c_free(node);
}

__c_static void __erase(c_tree_t* tree, c_tree_node_t* node) // erase node and it's children
{
    while (node) {
        __erase(tree, node->right);
        c_tree_node_t* left = node->left;
        __destroy_node(tree, node);
        node = left;
    }
}

__c_static size_t __count(c_tree_t* tree, c_tree_node_t* node, c_ref_t key)
{
    size_t n = 0;
    while (node) {
        if (tree->key_comp(key, tree->key_of_value(node->value)))
            node = node->left;
        else {
            if (!tree->key_comp(tree->key_of_value(node->value), key)) {
                n += __count(tree, node->left, key);
                n += __count(tree, node->right, key);
                return ++n;
            }

            node = node->right;
        }
    }
    return 0;
}

__c_static __c_inline void __rebalance_insert(c_tree_t* tree, c_tree_node_t* node)
{
    if (!tree || !node) return;

    while (__root(tree) != node && __is_red(__parent(node))) {
        if (__is_left(__parent(node))) {
            if (__is_red(__uncle(node))) {
                __parent(node)->color = s_rb_tree_color_black;
                __uncle(node)->color = s_rb_tree_color_black;
                __grand_parent(node)->color = s_rb_tree_color_red;
                node = __grand_parent(node);
            }
            else {
                if (__is_right(node)) {
                    node = __parent(node);
                    __rotate_left(node, __root(tree));
                }

                __parent(node)->color = s_rb_tree_color_black;
                __grand_parent(node)->color = s_rb_tree_color_red;
                __rotate_right(__grand_parent(node), __root(tree));
            }
        }
        else {
            if (__is_red(__uncle(node))) {
                __parent(node)->color = s_rb_tree_color_black;
                __uncle(node)->color = s_rb_tree_color_black;
                __grand_parent(node)->color = s_rb_tree_color_red;
                node = __grand_parent(node);
            }
            else {
                if (__is_left(node)) {
                    node = __parent(node);
                    __rotate_right(node, __root(tree));
                }

                __parent(node)->color = s_rb_tree_color_black;
                __grand_parent(node)->color = s_rb_tree_color_red;
                __rotate_left(__grand_parent(node), __root(tree));
            }
        }
    }

    __root(tree)->color = s_rb_tree_color_black;
}

__c_static __c_inline c_tree_node_t* __rebalance_erase(c_tree_t* tree, c_tree_node_t* node)
{
    if (!tree || !node) return 0;

    c_tree_node_t* erase_node = node;
    c_tree_node_t* replace_node = 0;
    c_tree_node_t* replace_parent = 0;
    if (!__has_left(erase_node)) {
        // erase_node has right child or no child
        // replace_node may be null
        replace_node = __right(erase_node);
    }
    else {
        if (!__has_right(erase_node)) {
            // erase_node has left child
            replace_node = __left(erase_node);
            assert(replace_node);
        }
        else {
            // erase_node has both children
            // replace_node may be null
            erase_node = __minimum(__right(erase_node));
            replace_node = __right(erase_node);
        }
    }
    assert(erase_node);

    if (erase_node == node) { // replace erase_node with replace_node
        if (__root(tree) == erase_node) {
            __set_root(tree, replace_node);
        }
        else {
            if (__is_left(erase_node)) {
                __parent(erase_node)->left = replace_node;
            }
            else {
                __parent(erase_node)->right = replace_node;
            }
        }

        if (__leftmost(tree) == erase_node) {
            __set_leftmost(tree, replace_node ? __minimum(replace_node)
                           : __parent(erase_node));
        }

        if (__rightmost(tree) == erase_node) {
            __set_rightmost(tree, replace_node ? __maximum(replace_node)
                            : __parent(erase_node));
        }

        if (replace_node) {
            replace_node->parent = __parent(erase_node);
        }

        replace_parent = __parent(erase_node);
    }
    else { // erase_node is node's successor
        if (__root(tree) == node) {
            __set_root(tree, erase_node);
        }
        else {
            if (__is_left(node)) {
                __parent(node)->left = erase_node;
            }
            else {
                __parent(node)->right = erase_node;
            }
        }

        erase_node->left = __left(node);
        __left(node)->parent = erase_node;
        if (erase_node != __right(node)) {
            erase_node->right = __right(node);
            __right(node)->parent = erase_node;
            __parent(erase_node)->left = replace_node;
            if (replace_node) {
                replace_node->parent = __parent(erase_node);
            }

            replace_parent = __parent(erase_node);
        }
        else {
            replace_parent = erase_node;
        }

        // swap color of node and node's successor, since node has both children
        // node's successor will take place of node, change its color to make sure
        // black node number of the left child of node is not affected
        __rb_tree_color_type color = erase_node->color;
        erase_node->color = node->color;
        node->color = color;

        erase_node->parent = __parent(node);
        erase_node = node;
    }

    // rebalance when erase_node is black
    if (__is_black(erase_node)) {
        while (replace_node != __root(tree) && __is_black(replace_node)) {
            if (replace_node) {
                assert(__parent(replace_node) == replace_parent);
                assert(__left(replace_parent) == replace_node || __right(replace_parent) == replace_node);
            }

            // replace_node may be null
            if (replace_node == __left(replace_parent)) {
                c_tree_node_t* replace_sibling = __right(replace_parent);
                if (__is_red(replace_sibling)) {
                    replace_sibling->color = s_rb_tree_color_black;
                    replace_parent->color = s_rb_tree_color_red;
                    __rotate_left(replace_parent, __root(tree));
                    replace_sibling = __right(replace_parent);
                }

                if ((!__has_left(replace_sibling) || __is_black(__left(replace_sibling))) &&
                        (!__has_right(replace_sibling) || __is_black(__right(replace_sibling)))) {
                    replace_sibling->color = s_rb_tree_color_red;
                    replace_node = replace_parent;
                    replace_parent = __parent(replace_parent);
                }
                else {
                    if (!__has_right(replace_sibling) || __is_black(__right(replace_sibling))) {
                        if (__has_left(replace_sibling)) {
                            __left(replace_sibling)->color = s_rb_tree_color_black;
                        }
                        replace_sibling->color = s_rb_tree_color_red;
                        __rotate_right(replace_sibling, __root(tree));
                        replace_sibling = __right(replace_parent);
                    }

                    replace_sibling->color = replace_parent->color;
                    replace_parent->color = s_rb_tree_color_black;

                    if (__has_right(replace_sibling)) {
                        __right(replace_sibling)->color = s_rb_tree_color_black;
                    }

                    __rotate_left(replace_parent, __root(tree));
                    break;
                }
            }
            else {
                c_tree_node_t* replace_sibling = __left(replace_parent);
                if (__is_red(replace_sibling)) {
                    replace_sibling->color = s_rb_tree_color_black;
                    replace_parent->color = s_rb_tree_color_red;
                    __rotate_right(replace_parent, __root(tree));
                    replace_sibling = __left(replace_parent);
                }

                if ((!__has_left(replace_sibling) || __is_black(__left(replace_sibling))) &&
                        (!__has_right(replace_sibling) || __is_black(__right(replace_sibling)))) {
                    replace_sibling->color = s_rb_tree_color_red;
                    replace_node = replace_parent;
                    replace_parent = __parent(replace_parent);
                }
                else {
                    if (!__has_left(replace_sibling) || __is_black(__left(replace_sibling))) {
                        if (__has_right(replace_sibling)) {
                            __right(replace_sibling)->color = s_rb_tree_color_black;
                        }
                        replace_sibling->color = s_rb_tree_color_red;
                        __rotate_left(replace_sibling, __root(tree));
                        replace_sibling = __left(replace_parent);
                    }

                    replace_sibling->color = replace_parent->color;
                    replace_parent->color = s_rb_tree_color_black;

                    if (__has_left(replace_sibling)) {
                        __left(replace_sibling)->color = s_rb_tree_color_black;
                    }

                    __rotate_right(replace_parent, __root(tree));
                    break;
                }
            }
        }

        if (replace_node) {
            replace_node->color = s_rb_tree_color_black;
        }
    }

    return erase_node;
}

__c_static __c_inline c_tree_node_t* __insert(
    c_tree_t* tree, c_tree_node_t* hint, c_tree_node_t* parent, c_ref_t value)
{
    assert(tree);
    assert(parent);
    assert(value);

    c_tree_node_t* node = __create_node(tree, value);
    if (!node) return 0;

    c_compare key_comp = tree->key_comp;
    c_key_of_value key_of_value = tree->key_of_value;
    c_tree_node_t* header = __header(tree);
    if (parent == __header(tree) || hint ||
        key_comp(key_of_value(value), key_of_value(parent->value))) {
        parent->left = node;

        if (parent == header) {
            header->parent = node;
            header->right = node;
        }
        else if (parent == __leftmost(tree)) {
            header->left = node;
        }
    }
    else {
        parent->right = node;

        if (parent == __rightmost(tree)) {
            header->right = node;
        }
    }

    node->parent = parent;

    __rebalance_insert(tree, node);
    ++(tree->node_count);

    return node;
}

__c_static __c_inline size_t __black_count(c_tree_node_t* bottom, c_tree_node_t* top)
{
    assert(bottom);
    assert(top);

    size_t n = 0;
    c_tree_node_t* node = bottom;

    while (node != top) {
        if (__is_black(node)) ++n;
        node = __parent(node);
    }

    if (__is_black(node)) ++n;

    return n;
}

__c_static __c_inline bool is_tree_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_BIDIRECTION &&
            iter->iterator_type == C_ITER_TYPE_TREE);
}

__c_static __c_inline bool is_tree_reverse_iterator(c_iterator_t* iter)
{
    return (iter != 0 &&
            iter->iterator_category == C_ITER_CATE_BIDIRECTION &&
            iter->iterator_type == C_ITER_TYPE_TREE_REVERSE);
}

__c_static void iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && is_tree_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_tree_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_tree_iterator_t));
    }
}

__c_static c_iterator_t* iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (is_tree_iterator(dst) && is_tree_iterator(src) && dst != src) {
        ((c_tree_iterator_t*)dst)->node = ((c_tree_iterator_t*)src)->node;
    }
    return dst;
}

__c_static c_iterator_t* iter_increment(c_iterator_t* iter)
{
    if (is_tree_iterator(iter)) {
        c_tree_iterator_t* _iter = (c_tree_iterator_t*)iter;
        if (_iter->node->right)
            _iter->node = __minimum(_iter->node->right);
        else {
            c_tree_node_t* parent = _iter->node->parent;
            while (parent->right == _iter->node) {
                _iter->node = parent;
                parent = parent->parent;
            }

            // in the case of node is header and parent is root
            if (parent != _iter->node->right)
                _iter->node = parent;
        }
    }
    return iter;
}

__c_static c_iterator_t* iter_decrement(c_iterator_t* iter)
{
    if (is_tree_iterator(iter)) {
        c_tree_iterator_t* _iter = (c_tree_iterator_t*)iter;
        if (__is_header(_iter->node))
            _iter->node = _iter->node->right;
        else if (_iter->node->left)
            _iter->node = __maximum(_iter->node->left);
        else {
            c_tree_node_t* parent = _iter->node->parent;
            while (parent->left == _iter->node) {
                _iter->node = parent;
                parent = parent->parent;
            }

            _iter->node = parent;
        }
    }
    return iter;
}

__c_static c_iterator_t* iter_post_increment(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (is_tree_iterator(iter)) {
        if (*tmp == 0)
            iter_alloc_and_copy(tmp, iter);
        else {
            assert(is_tree_iterator(*tmp));
            iter_assign(*tmp, iter);
        }
        iter_increment(iter);
    }
    return *tmp;
}

__c_static c_iterator_t* iter_post_decrement(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (is_tree_iterator(iter)) {
        if (*tmp == 0)
            iter_alloc_and_copy(tmp, iter);
        else {
            assert(is_tree_iterator(*tmp));
            iter_assign(*tmp, iter);
        }
        iter_decrement(iter);
    }
    return *tmp;
}

__c_static c_ref_t iter_dereference(c_iterator_t* iter)
{
    if (is_tree_iterator(iter)) {
        return ((c_tree_iterator_t*)iter)->node->value;
    }
    return 0;
}

__c_static bool iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!is_tree_iterator(x) || !is_tree_iterator(y)) return false;
    return ((c_tree_iterator_t*)x)->node == ((c_tree_iterator_t*)y)->node;
}

__c_static bool iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !iter_equal(x, y);
}

__c_static void iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (is_tree_iterator(iter)) {
        if (n > 0) {
            while (n--) iter_increment(iter);
        }
        else {
            while (n++) iter_decrement(iter);
        }
    }
}

__c_static ptrdiff_t iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!is_tree_iterator(first) || !is_tree_iterator(last)) return 0;

    c_tree_iterator_t x = __create_iterator(first->value_type, ((c_tree_iterator_t*)first)->node);

    ptrdiff_t n = 0;
    while (!iter_equal((c_iterator_t*)(&x), last)) {
        iter_increment((c_iterator_t*)(&x));
        ++n;
    }

    return n;
}

__c_static void reverse_iter_alloc_and_copy(c_iterator_t** self, c_iterator_t* other)
{
    if (self && !(*self) && is_tree_reverse_iterator(other)) {
        *self = (c_iterator_t*)malloc(sizeof(c_tree_iterator_t));
        if (*self) memcpy(*self, other, sizeof(c_tree_iterator_t));
    }
}

__c_static c_iterator_t* reverse_iter_assign(c_iterator_t* dst, c_iterator_t* src)
{
    if (is_tree_reverse_iterator(dst) && is_tree_reverse_iterator(src) && dst != src) {
        ((c_tree_iterator_t*)dst)->node = ((c_tree_iterator_t*)src)->node;
    }
    return dst;
}

__c_static c_iterator_t* reverse_iter_increment(c_iterator_t* iter)
{
    if (is_tree_reverse_iterator(iter)) {
        c_tree_iterator_t x = __create_iterator(iter->value_type, ((c_tree_iterator_t*)iter)->node);
        iter_decrement((c_iterator_t*)(&x));
        ((c_tree_iterator_t*)iter)->node = x.node;
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_decrement(c_iterator_t* iter)
{
    if (is_tree_reverse_iterator(iter)) {
        c_tree_iterator_t x = __create_iterator(iter->value_type, ((c_tree_iterator_t*)iter)->node);
        iter_increment((c_iterator_t*)(&x));
        ((c_tree_iterator_t*)iter)->node = x.node;
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_post_increment(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (is_tree_reverse_iterator(iter)) {
        if (*tmp == 0)
            reverse_iter_alloc_and_copy(tmp, iter);
        else {
            assert(is_tree_reverse_iterator(*tmp));
            reverse_iter_assign(*tmp, iter);
        }
        c_tree_iterator_t x = __create_iterator(iter->value_type, ((c_tree_iterator_t*)iter)->node);
        iter_decrement((c_iterator_t*)(&x));
        ((c_tree_iterator_t*)iter)->node = x.node;
    }
    return *tmp;
}

__c_static c_iterator_t* reverse_iter_post_decrement(c_iterator_t* iter, c_iterator_t** tmp)
{
    assert(tmp);
    if (is_tree_reverse_iterator(iter)) {
        if (*tmp == 0)
            reverse_iter_alloc_and_copy(tmp, iter);
        else {
            assert(is_tree_reverse_iterator(*tmp));
            reverse_iter_assign(*tmp, iter);
        }
        c_tree_iterator_t x = __create_iterator(iter->value_type, ((c_tree_iterator_t*)iter)->node);
        iter_increment((c_iterator_t*)(&x));
        ((c_tree_iterator_t*)iter)->node = x.node;
    }
    return *tmp;
}

__c_static c_ref_t reverse_iter_dereference(c_iterator_t* iter)
{
    if (is_tree_reverse_iterator(iter)) {
        c_tree_iterator_t x = __create_iterator(iter->value_type, ((c_tree_iterator_t*)iter)->node);
        iter_decrement((c_iterator_t*)(&x));
        return x.node->value;
    }
    return 0;
}

__c_static bool reverse_iter_equal(c_iterator_t* x, c_iterator_t* y)
{
    if (!is_tree_reverse_iterator(x) || !is_tree_reverse_iterator(y)) return false;
    return ((c_tree_iterator_t*)x)->node == ((c_tree_iterator_t*)y)->node;
}

__c_static bool reverse_iter_not_equal(c_iterator_t* x, c_iterator_t* y)
{
    return !reverse_iter_equal(x, y);
}

__c_static void reverse_iter_advance(c_iterator_t* iter, ptrdiff_t n)
{
    if (is_tree_reverse_iterator(iter)) {
        if (n > 0) {
            while (n--) reverse_iter_increment(iter);
        }
        else {
            while (n++) reverse_iter_decrement(iter);
        }
    }
}

__c_static ptrdiff_t reverse_iter_distance(c_iterator_t* first, c_iterator_t* last)
{
    if (!is_tree_reverse_iterator(first) || !is_tree_reverse_iterator(last)) return 0;

    c_tree_iterator_t x = __create_reverse_iterator(first->value_type, ((c_tree_iterator_t*)first)->node);

    ptrdiff_t n = 0;
    while (!reverse_iter_equal((c_iterator_t*)(&x), last)) {
        reverse_iter_increment((c_iterator_t*)(&x));
        ++n;
    }

    return n;
}

static c_iterator_operation_t s_iter_ops = {
    .alloc_and_copy = iter_alloc_and_copy,
    .assign = iter_assign,
    .increment = iter_increment,
    .decrement = iter_decrement,
    .post_increment = iter_post_increment,
    .post_decrement = iter_post_decrement,
    .dereference = iter_dereference,
    .equal = iter_equal,
    .not_equal = iter_not_equal,
    .less = 0,
    .advance = iter_advance,
    .distance = iter_distance
};

__c_static __c_inline c_tree_iterator_t __create_iterator(
    const c_type_info_t* value_type, c_tree_node_t* node)
{
    assert(value_type);
    assert(node);

    c_tree_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_BIDIRECTION,
            .iterator_type = C_ITER_TYPE_TREE,
            .iterator_ops = &s_iter_ops,
            .value_type = value_type
        },
        .node = node
    };
    return iter;
}

static c_iterator_operation_t s_reverse_iter_ops = {
    .alloc_and_copy = reverse_iter_alloc_and_copy,
    .assign = reverse_iter_assign,
    .increment = reverse_iter_increment,
    .decrement = reverse_iter_decrement,
    .post_increment = reverse_iter_post_increment,
    .post_decrement = reverse_iter_post_decrement,
    .dereference = reverse_iter_dereference,
    .equal = reverse_iter_equal,
    .not_equal = reverse_iter_not_equal,
    .less = 0,
    .advance = reverse_iter_advance,
    .distance = reverse_iter_distance
};

__c_static __c_inline c_tree_iterator_t __create_reverse_iterator(
    const c_type_info_t* value_type, c_tree_node_t* node)
{
    assert(value_type);
    assert(node);

    c_tree_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_BIDIRECTION,
            .iterator_type = C_ITER_TYPE_TREE_REVERSE,
            .iterator_ops = &s_reverse_iter_ops,
            .value_type = value_type
        },
        .node = node
    };
    return iter;
}

c_tree_t* c_tree_create(const c_type_info_t* key_type,
                        const c_type_info_t* value_type,
                        const c_type_info_t* mapped_type,
                        c_key_of_value key_of_value,
                        c_compare key_comp)
{
    c_tree_t* tree = (c_tree_t*)malloc(sizeof(c_tree_t));
    if (!tree) return 0;

    tree->header = (c_tree_node_t*)malloc(sizeof(c_tree_node_t));
    if (!__header(tree)) {
        __c_free(tree);
        return 0;
    }

    tree->header->color = s_rb_tree_color_red;
    tree->header->left = __header(tree);
    tree->header->right = __header(tree);
    tree->header->parent = 0;
    tree->header->value = 0;

    tree->key_type = key_type;
    tree->value_type = value_type;
    tree->mapped_type = mapped_type;
    tree->key_of_value = key_of_value;
    tree->key_comp = key_comp;
    tree->node_count = 0;

    return tree;
}

void c_tree_destroy(c_tree_t* tree)
{
    c_tree_clear(tree);
    __c_free(tree);
}

c_tree_iterator_t c_tree_begin(c_tree_t* tree)
{
    assert(tree);
    return __create_iterator(tree->value_type, __leftmost(tree));
}

c_tree_iterator_t c_tree_end(c_tree_t* tree)
{
    assert(tree);
    return __create_iterator(tree->value_type, __header(tree));
}

c_tree_iterator_t c_tree_rbegin(c_tree_t* tree)
{
    assert(tree);
    return __create_reverse_iterator(tree->value_type, __header(tree));
}

c_tree_iterator_t c_tree_rend(c_tree_t* tree)
{
    assert(tree);
    return __create_reverse_iterator(tree->value_type, __leftmost(tree));
}

bool c_tree_empty(c_tree_t* tree)
{
    return tree ? tree->node_count == 0 : true;
}

size_t c_tree_size(c_tree_t* tree)
{
    return tree ? tree->node_count : 0;
}

size_t c_tree_max_size(void)
{
    return (-1);
}

void c_tree_clear(c_tree_t* tree)
{
    if (!tree) return;

    __erase(tree, __root(tree));
    c_tree_node_t* header = __header(tree);
    header->parent = 0;
    header->left = header;
    header->right = header;
    tree->node_count = 0;
}

c_tree_iterator_t c_tree_insert_unique_value(c_tree_t* tree, c_ref_t value)
{
    assert(tree);
    assert(value);

    c_compare key_comp = tree->key_comp;
    c_key_of_value key_of_value = tree->key_of_value;
    c_tree_node_t* y = __header(tree);
    c_tree_node_t* x = __root(tree);
    bool comp = true;
    while (x) {
        y = x;
        comp = key_comp(key_of_value(value), key_of_value(x->value));
        x = comp ? x->left : x->right;
    }

    c_tree_iterator_t z = __create_iterator(tree->value_type, y);
    if (comp) {
        if (z.node == __leftmost(tree))
            return __create_iterator(tree->value_type, __insert(tree, 0, y, value));
        else
            C_ITER_DEC(&z);
    }

    if (key_comp(key_of_value(z.node->value), key_of_value(value)))
        return __create_iterator(tree->value_type, __insert(tree, 0, y, value));

    return z;
}

c_tree_iterator_t c_tree_insert_unique(c_tree_t* tree, c_tree_iterator_t hint, c_ref_t value)
{
    assert(tree);
    assert(value);

    c_compare key_comp = tree->key_comp;
    c_key_of_value key_of_value = tree->key_of_value;
    if (hint.node == __leftmost(tree)) { // begin()
        if (tree->node_count > 0 && key_comp(key_of_value(value), key_of_value(hint.node->value)))
            // insert to the left
            return __create_iterator(tree->value_type, __insert(tree, hint.node, hint.node, value));
    }
    else if (hint.node == __header(tree)) { // end()
        // tree must be non-empty, otherwise go to the first if statement
        if (key_comp(key_of_value(__rightmost(tree)->value), key_of_value(value)))
            // insert to the right
            return __create_iterator(tree->value_type, __insert(tree, 0, __rightmost(tree), value));
    }
    else {
        c_tree_iterator_t before = hint;
        C_ITER_DEC(&before);
        if (key_comp(key_of_value(before.node->value), key_of_value(value)) &&
            key_comp(key_of_value(value), key_of_value(hint.node->value))) {
            if (!__has_right(before.node))
                // insert to before's right
                return __create_iterator(tree->value_type, __insert(tree, 0, before.node, value));
            else
                // insert to hint's left
                return __create_iterator(tree->value_type, __insert(tree, hint.node, hint.node, value));
        }
    }

    return c_tree_insert_unique_value(tree, value);
}

void c_tree_insert_unique_range(c_tree_t* tree,
                                c_iterator_t* __c_input_iterator first,
                                c_iterator_t* __c_input_iterator last)
{
    if (!tree || !first || !last) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last)) {
        c_tree_insert_unique_value(tree, C_ITER_DEREF(__first));
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)
}

void c_tree_insert_unique_from(c_tree_t* tree, c_ref_t first_value, c_ref_t last_value)
{
    if (!tree || !first_value || !last_value) return;

    c_ref_t value = first_value;
    while (value != last_value) {
        c_tree_insert_unique_value(tree, value);
        value += tree->value_type->size();
    }
}

c_tree_iterator_t c_tree_insert_equal_value(c_tree_t* tree, c_ref_t value)
{
    assert(tree);
    assert(value);

    c_tree_node_t* y = __header(tree);
    c_tree_node_t* x = __root(tree);

    c_compare key_comp = tree->key_comp;
    c_key_of_value key_of_value = tree->key_of_value;
    while (x) {
        y = x;
        x = key_comp(key_of_value(value), key_of_value(x->value)) ? x->left : x->right;
    }

    return __create_iterator(tree->value_type, __insert(tree, 0, y, value));
}

c_tree_iterator_t c_tree_insert_equal(c_tree_t* tree, c_tree_iterator_t hint, c_ref_t value)
{
    assert(tree);
    assert(value);

    c_compare key_comp = tree->key_comp;
    c_key_of_value key_of_value = tree->key_of_value;
    if (hint.node == __leftmost(tree)) { // begin()
        if (tree->node_count > 0 && !key_comp(key_of_value(hint.node->value), key_of_value(value)))
            // insert to the left
            return __create_iterator(tree->value_type, __insert(tree, hint.node, hint.node, value));
    }
    else if (hint.node == __header(tree)) { // end()
        // tree must be non-empty, otherwise go to the first if statement
        if (!key_comp(key_of_value(value), key_of_value(__rightmost(tree)->value)))
            // insert to the right
            return __create_iterator(tree->value_type, __insert(tree, 0, __rightmost(tree), value));
    }
    else {
        c_tree_iterator_t before = hint;
        C_ITER_DEC(&before);
        if (!key_comp(key_of_value(value), key_of_value(before.node->value)) &&
            !key_comp(key_of_value(hint.node->value), key_of_value(value))) {
            if (!__has_right(before.node))
                // insert to before's right
                return __create_iterator(tree->value_type, __insert(tree, 0, before.node, value));
            else
                // insert to hint's left
                return __create_iterator(tree->value_type, __insert(tree, hint.node, hint.node, value));
        }
    }

    return c_tree_insert_equal_value(tree, value);
}

void c_tree_insert_equal_range(c_tree_t* tree,
                               c_iterator_t* __c_input_iterator first,
                               c_iterator_t* __c_input_iterator last)
{
    if (!tree || !first || !last) return;
    assert(C_ITER_AT_LEAST(first, C_ITER_CATE_INPUT));
    assert(C_ITER_AT_LEAST(last, C_ITER_CATE_INPUT));

    __C_ALGO_BEGIN_2(first, last)

    while (C_ITER_NE(__first, __last)) {
        c_tree_insert_equal_value(tree, C_ITER_DEREF(__first));
        C_ITER_INC(__first);
    }

    __C_ALGO_END_2(first, last)
}

void c_tree_insert_equal_from(c_tree_t* tree, c_ref_t first_value, c_ref_t last_value)
{
    if (!tree || !first_value || !last_value) return;

    c_ref_t value = first_value;
    while (value != last_value) {
        c_tree_insert_equal_value(tree, value);
        value += tree->value_type->size();
    }
}

c_tree_iterator_t c_tree_erase(c_tree_t* tree, c_tree_iterator_t pos)
{
    assert(tree);

    c_tree_iterator_t next = pos;
    C_ITER_INC(&next);

    c_tree_node_t* erase_node = __rebalance_erase(tree, pos.node);

    __destroy_node(tree, erase_node);
    --(tree->node_count);

    return next;
}

size_t c_tree_erase_key(c_tree_t* tree, c_ref_t key)
{
    if (!tree || !key) return 0;

    c_tree_iterator_t lower = c_tree_lower_bound(tree, key);
    c_tree_iterator_t upper = c_tree_upper_bound(tree, key);
    size_t n = (size_t)C_ITER_DISTANCE(&lower, &upper);
    c_tree_erase_range(tree, lower, upper);

    return n;
}

void c_tree_erase_range(c_tree_t* tree, c_tree_iterator_t first, c_tree_iterator_t last)
{
    if (!tree) return;

    if (first.node == __leftmost(tree) && last.node == __header(tree))
        c_tree_clear(tree);
    else {
        while (first.node != last.node)
            first = c_tree_erase(tree, first);
    }
}

void c_tree_erase_from(c_tree_t* tree, c_ref_t first_key, c_ref_t last_key)
{
    if (!tree || !first_key || !last_key) return;

    c_ref_t key = first_key;
    while (key != last_key) {
        c_tree_erase_key(tree, key);
        key += tree->value_type->size();
    }
}

void c_tree_swap(c_tree_t* tree, c_tree_t* other)
{
    if (!tree || !other) return;
    c_tree_t tmp = *tree;
    *tree = *other;
    *other = tmp;
}

c_tree_iterator_t c_tree_find(c_tree_t* tree, c_ref_t key)
{
    if (c_tree_empty(tree) || !key) return c_tree_end(tree);

    c_tree_node_t* node = __root(tree);
    c_compare key_comp = tree->key_comp;
    c_key_of_value key_of_value = tree->key_of_value;
    while (node) {
        if (key_comp(key, key_of_value(node->value)))
            node = node->left;
        else {
            if (!key_comp(key_of_value(node->value), key))
                return __create_iterator(tree->value_type, node);

            node = node->right;
        }
    }

    return c_tree_end(tree);
}

size_t c_tree_count(c_tree_t* tree, c_ref_t key)
{
    if (c_tree_empty(tree) || !key) return 0;

    return __count(tree, __root(tree), key);
}

c_tree_iterator_t c_tree_lower_bound(c_tree_t* tree, c_ref_t key)
{
    assert(tree);
    assert(key);

    c_tree_node_t* y = __header(tree);
    c_tree_node_t* x = __root(tree);

    c_compare key_comp = tree->key_comp;
    c_key_of_value key_of_value = tree->key_of_value;
    while (x != 0) {
        if (!key_comp(key_of_value(x->value), key)) {
            y = x;
            x = __left(x);
        }
        else
            x = __right(x);
    }

    // return last node which is not less than key
    return __create_iterator(tree->value_type, y);
}

c_tree_iterator_t c_tree_upper_bound(c_tree_t* tree, c_ref_t key)
{
    assert(tree);
    assert(key);

    c_tree_node_t* y = __header(tree);
    c_tree_node_t* x = __root(tree);

    c_compare key_comp = tree->key_comp;
    c_key_of_value key_of_value = tree->key_of_value;
    while (x != 0) {
        if (key_comp(key, key_of_value(x->value))) {
            y = x;
            x = __left(x);
        }
        else
            x = __right(x);
    }

    // return last node which is greater than key
    return __create_iterator(tree->value_type, y);
}

void c_tree_equal_range(c_tree_t* tree, c_ref_t key,
                        c_tree_iterator_t** lower,
                        c_tree_iterator_t** upper)
{
    if (!tree || !key || !lower || !upper) return;

    if (*lower == 0)
        *lower = (c_tree_iterator_t*)malloc(sizeof(c_tree_iterator_t));
    if (*upper == 0)
        *upper = (c_tree_iterator_t*)malloc(sizeof(c_tree_iterator_t));

    if (*lower == 0 || *upper == 0) {
        __c_free(*lower);
        __c_free(*upper);
        return;
    }

    c_tree_iterator_t _lower = c_tree_lower_bound(tree, key);
    c_tree_iterator_t _upper = c_tree_upper_bound(tree, key);

    (*lower)->node = _lower.node;
    (*upper)->node = _upper.node;
}

bool c_tree_rb_verify(c_tree_t* tree)
{
    if (!tree) return false;

    if (tree->node_count == 0 || c_tree_begin(tree).node == c_tree_end(tree).node) {
        return (tree->node_count == 0 &&
                c_tree_begin(tree).node == c_tree_end(tree).node &&
                tree->header->left == tree->header &&
                tree->header->right == tree->header &&
                tree->header->parent == 0 &&
                tree->header->color == s_rb_tree_color_red);
    }

    if (!__is_red(__header(tree)) || !__is_black(__root(tree)) ||
        __parent(__header(tree)) != __root(tree) || __parent(__root(tree)) != __header(tree))
        return false;

    size_t black_num = __black_count(__leftmost(tree), __root(tree));
    c_tree_iterator_t first = c_tree_begin(tree);
    c_tree_iterator_t last = c_tree_end(tree);
    c_compare key_comp = tree->key_comp;
    c_key_of_value key_of_value = tree->key_of_value;
    assert(last.node);
    while (first.node != last.node) {
        assert(first.node);
        c_tree_node_t* left = __left(first.node);
        c_tree_node_t* right = __right(first.node);

        if (__is_red(first.node)) {
            if ((left && !__is_black(left)) ||
                (right && !__is_black(right)))
            return false;
        }

        if (left && key_comp(key_of_value(first.node->value), key_of_value(left->value)))
            return false;

        if (right && key_comp(key_of_value(right->value), key_of_value(first.node->value)))
            return false;

        if (!left && !right && __black_count(first.node, __root(tree)) != black_num)
            return false;

        C_ITER_INC(&first);
    }

    if (__leftmost(tree) != __minimum(__root(tree)) ||
        __rightmost(tree) != __maximum(__root(tree)))
        return false;

    return true;
}
