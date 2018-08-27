# C Container
C container is a STL-like library which implements generic containers in C language.  It also implements most of the algorithms in STL algorithm, which can be applied to containers.  This library is intended to be helpful for embedded software development, which may still use C language nowadays.

Containers:
 - Sequence containers: list, forward list, vector, deque
 - Associative containers: set, map, multiset, multimap

Container adapters:
 - stack, whose default backend is deque
 - queue, whose default backend is deque
 - priority queue, whose default backend is vector

#### Notes
 - To implement the most generic containers, elements are all passed by reference, i.e. void* in C language.

 - C deque is implemented very much like C vector, except that it has spare space in both header and tail. By this way, iterator operations are as simple as vector.  Algorithms used on deque will be as fast as vector.  But it loses the flexibility in memory management.  As deque size grows, much larger consecutive memory is required, which may cause memory allocation failure if runs on small memory devices.

 - Like STL containers, each C container has its own iterator type and operation set.  Unlike STL iterators, C iterators are implemented by inheritance.  All the C iterators have the same base iterator as its first field in the structure, which makes it possible to convert any iterators to base iterator type and pass the
iterators to algorithms.

## Build from source
In root directory of source code
```sh
$ mkdir build
$ cd build
$ cmake ../
$ make
```
To build unit tests, it requires Google test can be found.  Make sure you installed it before building.
If cross-compiling for embedded device, change the toolchain used in common.cmake.

License
----

MIT