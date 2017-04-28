# c_container
C container is a STL-like library which implements generic containers in C language.
It also implements most of the algorithms in STL algorithm, which can be applied to 
containers.

This library is intended to be helpful for embeded software development, which may
still use C language nowadays.

C containers:
(1) Sequence containers: list, forward list, vector, deque
(2) Associative containers: set, map, multiset, multimap

C container adapters:
(1) stack, whose default backend is deque
(2) queue, whose default backend is deque
(3) priority queue, whose default backend is vector

Note:
1. C deque is implemented very much like C vector, except that it has spare space in 
both header and tail. By this way, iterator operations are as simple as vector.
Algorithms used on deque will be as fast as vector. But it loses the flexibility in
memory management. As deque size grows, much larger consecutive memory is required.
Which may cause memory allocation failure if runs on small memory devices.

2. Like STL containers, each C container has its own iterator type and operation set.
Unlike STL iterators, C iterators are implemented by inheritance.
All the C iterators have the same base iterator as its first field in the structure,
which makes it possible to convert any iterators to base iterator type and pass the
iterators to algorithms.

