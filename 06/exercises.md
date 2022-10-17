## Exercise 1

Recall the linked list class from [last class](../05/exercises.md).
The first bug was that the copy assignment was doing a shallow copy instead of a deep copy.
The second bug was that the move constructor did not reset the other `list`'s `next` pointer to null, resulting in a double free.
The third bug was that the move assignment did not `delete` the existing `next` pointer, leading to a memory leak.

Try to insert these bugs into the [`list.cpp`](list.cpp) file, which is an implementation that uses `unique_ptr`.
Which bugs are you able to replicate?
Why or why not?

## Exercise 2

Consider a class of nodes for possibly cyclic directed graphs.
What kind of smart pointer would be most suitable for representing each node we have an edge to?
