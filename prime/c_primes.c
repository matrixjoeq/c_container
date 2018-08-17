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

#include "c_prime_internal.h"

__C_TYPE_OPERATIONS(char, char, 0)
__C_GET_TYPE_INFO(char)

__C_TYPE_OPERATIONS(double, double, 0.0f)
__C_GET_TYPE_INFO(double)

__C_TYPE_OPERATIONS(float, float, 0.0f)
__C_GET_TYPE_INFO(float)

__C_TYPE_OPERATIONS(int, int, 0)
__C_GET_TYPE_INFO(int)

__C_TYPE_OPERATIONS(long, long, 0l)
__C_GET_TYPE_INFO(long)

__C_TYPE_OPERATIONS(short, short, 0)
__C_GET_TYPE_INFO(short)

__C_TYPE_OPERATIONS(signed char, schar, 0)
__C_GET_TYPE_INFO(schar)

__C_TYPE_OPERATIONS(signed int, sint, 0)
__C_GET_TYPE_INFO(sint)

__C_TYPE_OPERATIONS(signed long, slong, 0l)
__C_GET_TYPE_INFO(slong)

__C_TYPE_OPERATIONS(signed short, sshort, 0)
__C_GET_TYPE_INFO(sshort)

__C_TYPE_OPERATIONS(unsigned char, uchar, 0u)
__C_GET_TYPE_INFO(uchar)

__C_TYPE_OPERATIONS(unsigned int, uint, 0u)
__C_GET_TYPE_INFO(uint)

__C_TYPE_OPERATIONS(unsigned long, ulong, 0ul)
__C_GET_TYPE_INFO(ulong)

__C_TYPE_OPERATIONS(unsigned short, ushort, 0u)
__C_GET_TYPE_INFO(ushort)
