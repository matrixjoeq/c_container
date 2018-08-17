/**
 * MIT License
 *
 * Copyright (c) 2017-2018 MatrixJoeQ
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

#include "c_util.h"
#include "c_internal.h"

void validate_type_info(const c_type_info_t* type_info)
{
    __c_assert(type_info->size, "Type must have size function.");
    __c_assert(type_info->create, "Type must have create function.");
    __c_assert(type_info->copy, "Type must have copy function.");
    __c_assert(type_info->destroy, "Type must have destroy function.");
    __c_assert(type_info->assign, "Type must have assign function.");
}

void validate_type_info_ex(const c_type_info_t* type_info)
{
    validate_type_info(type_info);
    __c_assert(type_info->less, "Type must have less function.");
    __c_assert(type_info->equal, "Type must have equal function.");
}
