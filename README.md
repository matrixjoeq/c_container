# c_container
This is a STL-like C container library which implements generic containers.
It also provides basic algorithms which can be applied to containers.

C container includes:
(1) Sequence container: list, forward list, vector

Like STL containers, each C container has its own iterator type and operation set.
Unlike STL iterators, C iterators are implemented by inheritance.
All the C iterators have the same base iterator as its first field in the structure,
which makes it possible to convert any iterators to base iterator type and pass the
iterators to algorithms.

C container provides container adapters:
(1) stack, whose default backend is list
(2) queue, whose default backend is list
(3) priority queue, whose default backend is vector

C container library implements most of the algorithms in STL algorithm.

