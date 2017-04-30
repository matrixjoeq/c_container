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
    c_containable_t* type_info;
    c_compare key_comp;
    c_tree_node_t* header;
    size_t node_count;
};

static const __rb_tree_color_type s_rb_tree_color_red = false;
static const __rb_tree_color_type s_rb_tree_color_black = true;

__c_static c_tree_iterator_t __create_iterator(c_containable_t* type_info, c_tree_node_t* node);
__c_static c_tree_iterator_t __create_reverse_iterator(c_containable_t* type_info, c_tree_node_t* node);

__c_static __c_inline bool __is_header(c_tree_node_t* node)
{
    return (node ? (node->parent->parent == node &&
                    node->color == s_rb_tree_color_red) : false);
}
/*
__c_static __c_inline bool __is_root(c_tree_node_t* node)
{
    return (node ? (node->parent->parent == node &&
                    node->color == s_rb_tree_color_black) : false);
}
*/
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
/*
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
*/
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

    c_containable_t* type_info = tree->type_info;
    assert(type_info);

    c_tree_node_t* node = (c_tree_node_t*)malloc(sizeof(c_tree_node_t));
    if (!node) return 0;

    node->value = malloc(type_info->size());
    if (!node->value) {
        __c_free(node);
        return 0;
    }

    node->parent = 0;
    node->left   = 0;
    node->right  = 0;
    node->color  = s_rb_tree_color_red;

    if (value)
        type_info->copy(node->value, value);
    else
        type_info->create(node->value);

    return node;
}

__c_static __c_inline void __destroy_node(c_tree_t* tree, c_tree_node_t* node)
{
    assert(tree);
    assert(node);

    tree->type_info->destroy(node->value);
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

__c_static __c_inline c_tree_node_t* __insert(c_tree_t* tree, c_tree_node_t* parent, c_ref_t value)
{
    assert(tree);
    assert(parent);

    c_tree_node_t* node = __create_node(tree, value);
    if (!node) return 0;

    c_compare key_comp = tree->key_comp;
    c_tree_node_t* header = tree->header;
    if (parent == tree->header || key_comp(value, parent->value)) {
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

    c_tree_iterator_t x = __create_iterator(first->type_info, ((c_tree_iterator_t*)first)->node);

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
        c_tree_iterator_t x = __create_iterator(iter->type_info, ((c_tree_iterator_t*)iter)->node);
        iter_decrement((c_iterator_t*)(&x));
        ((c_tree_iterator_t*)iter)->node = x.node;
    }
    return iter;
}

__c_static c_iterator_t* reverse_iter_decrement(c_iterator_t* iter)
{
    if (is_tree_reverse_iterator(iter)) {
        c_tree_iterator_t x = __create_iterator(iter->type_info, ((c_tree_iterator_t*)iter)->node);
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
        c_tree_iterator_t x = __create_iterator(iter->type_info, ((c_tree_iterator_t*)iter)->node);
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
        c_tree_iterator_t x = __create_iterator(iter->type_info, ((c_tree_iterator_t*)iter)->node);
        iter_increment((c_iterator_t*)(&x));
        ((c_tree_iterator_t*)iter)->node = x.node;
    }
    return *tmp;
}

__c_static c_ref_t reverse_iter_dereference(c_iterator_t* iter)
{
    if (is_tree_reverse_iterator(iter)) {
        c_tree_iterator_t x = __create_iterator(iter->type_info, ((c_tree_iterator_t*)iter)->node);
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

    c_tree_iterator_t x = __create_reverse_iterator(first->type_info, ((c_tree_iterator_t*)first)->node);

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
    .advance = iter_advance,
    .distance = iter_distance
};

__c_static __c_inline c_tree_iterator_t __create_iterator(
    c_containable_t* type_info, c_tree_node_t* node)
{
    assert(type_info);
    assert(node);

    c_tree_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_BIDIRECTION,
            .iterator_type = C_ITER_TYPE_TREE,
            .iterator_ops = &s_iter_ops,
            .type_info = type_info
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
    .advance = reverse_iter_advance,
    .distance = reverse_iter_distance
};

__c_static __c_inline c_tree_iterator_t __create_reverse_iterator(
    c_containable_t* type_info, c_tree_node_t* node)
{
    assert(type_info);
    assert(node);

    c_tree_iterator_t iter = {
        .base_iter = {
            .iterator_category = C_ITER_CATE_BIDIRECTION,
            .iterator_type = C_ITER_TYPE_TREE_REVERSE,
            .iterator_ops = &s_reverse_iter_ops,
            .type_info = type_info
        },
        .node = node
    };
    return iter;
}

c_tree_t* c_tree_create(c_containable_t* type_info, c_compare comp)
{
    c_tree_t* tree = (c_tree_t*)malloc(sizeof(c_tree_t));
    if (!tree) return 0;

    tree->header = (c_tree_node_t*)malloc(sizeof(c_tree_node_t));
    if (!tree->header) {
        __c_free(tree);
        return 0;
    }

    tree->header->color = s_rb_tree_color_red;
    tree->header->left = tree->header;
    tree->header->right = tree->header;
    tree->header->parent = 0;
    tree->header->value = 0;

    tree->type_info = type_info;
    tree->key_comp = comp;
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
    return __create_iterator(tree->type_info, __leftmost(tree));
}

c_tree_iterator_t c_tree_end(c_tree_t* tree)
{
    assert(tree);
    return __create_iterator(tree->type_info, tree->header);
}

c_tree_iterator_t c_tree_rbegin(c_tree_t* tree)
{
    assert(tree);
    return __create_reverse_iterator(tree->type_info, tree->header);
}

c_tree_iterator_t c_tree_rend(c_tree_t* tree)
{
    assert(tree);
    return __create_reverse_iterator(tree->type_info, __leftmost(tree));
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
    c_tree_node_t* header = tree->header;
    header->parent = 0;
    header->left = header;
    header->right = header;
    tree->node_count = 0;
}

c_tree_iterator_t c_tree_insert_unique_value(c_tree_t* tree, c_ref_t value)
{
    assert(tree);
    assert(value);

    c_tree_node_t* y = tree->header;
    c_tree_node_t* x = __root(tree);
    bool comp = true;
    while (x) {
        y = x;
        comp = tree->key_comp(value, x->value);
        x = comp ? x->left : x->right;
    }

    c_tree_iterator_t z = __create_iterator(tree->type_info, y);
    if (comp) {
        if (z.node == __leftmost(tree)) {
            return __create_iterator(tree->type_info, __insert(tree, y, value));
        }
        else {
            iter_decrement((c_iterator_t*)(&z));
        }
    }

    if (tree->key_comp(z.node->value, value)) {
        return __create_iterator(tree->type_info, __insert(tree, y, value));
    }

    return z;
}

void c_tree_insert_unique_range(c_tree_t* tree, c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last)
{
    if (!tree || !first || !last) return;

    while (C_ITER_NE(first, last)) {
        c_tree_insert_unique_value(tree, C_ITER_DEREF(first));
        C_ITER_INC(first);
    }
}

void c_tree_insert_unique_from(c_tree_t* tree, c_ref_t first_value, c_ref_t last_value)
{
    if (!tree || !first_value || !last_value) return;

    c_ref_t value = first_value;
    while (value != last_value) {
        c_tree_insert_unique_value(tree, value);
        value += tree->type_info->size();
    }
}

c_tree_iterator_t c_tree_insert_equal_value(c_tree_t* tree, c_ref_t value)
{
    assert(tree);
    assert(value);

    c_tree_node_t* y = tree->header;
    c_tree_node_t* x = __root(tree);

    while (x) {
        y = x;
        x = tree->key_comp(value, x->value) ? x->left : x->right;
    }

    return __create_iterator(tree->type_info, __insert(tree, y, value));
}

void c_tree_insert_equal_range(c_tree_t* tree, c_iterator_t* __c_input_iterator first, c_iterator_t* __c_input_iterator last)
{
    if (!tree || !first || !last) return;

    while (C_ITER_NE(first, last)) {
        c_tree_insert_equal_value(tree, C_ITER_DEREF(first));
        C_ITER_INC(first);
    }
}

void c_tree_insert_equal_from(c_tree_t* tree, c_ref_t first_value, c_ref_t last_value)
{
    if (!tree || !first_value || !last_value) return;

    c_ref_t value = first_value;
    while (value != last_value) {
        c_tree_insert_equal_value(tree, value);
        value += tree->type_info->size();
    }
}

void c_tree_erase(c_tree_t* tree, c_tree_iterator_t pos)
{
    assert(tree);
    c_tree_node_t* erase_node = __rebalance_erase(tree, pos.node);

    __destroy_node(tree, erase_node);
    --(tree->node_count);
}

size_t c_tree_erase_key(c_tree_t* tree, c_ref_t key)
{
    if (!tree || !key) return 0;

    size_t n = 0;
    c_tree_iterator_t found = c_tree_find(tree, key);
    c_tree_iterator_t last = c_tree_end(tree);
    while (C_ITER_NE(&found, &last)) {
        c_tree_erase(tree, found);
        ++n;
        found = c_tree_find(tree, key);
    }

    return n;
}

void c_tree_erase_from(c_tree_t* tree, c_ref_t first_key, c_ref_t last_key)
{
    if (!tree || !first_key || !last_key) return;

    c_ref_t key = first_key;
    while (key != last_key) {
        c_tree_erase_key(tree, key);
        key += tree->type_info->size();
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
    while (node) {
        if (tree->key_comp(key, node->value)) {
            node = node->left;
        }
        else {
            if (!tree->key_comp(node->value, key)) {
                return __create_iterator(tree->type_info, node);
            }

            node = node->right;
        }
    }

    return c_tree_end(tree);
}

