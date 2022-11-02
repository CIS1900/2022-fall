# Class 9

These notes will cover a very useful part of the C++ standard library: containers.
Along with containers, we'll cover the main way containers are used: via iterators.

## Sequence Containers

First we'll cover *sequence containers*, containers where the ordering of elements is determined by the user, for example in an array.

### `vector`

A `vector` (in the `<vector>` header) provides a resizable sequence of contiguous elements, like an array.
This means that accessing elements in a `vector` is just as fast as it is with an array.
Because of this, `vector` is by far the most useful and commonly-seen container class.
This class can be thought of as similar to an `ArrayList` in Java.

All standard library containers provide the `.size()` and `.empty()` member functions for checking the number of elements and whether that number is 0, respectively.
`vector` also allows users to access elements using `[...]` as with an array.
It further provides `.push_back(i)` to add a new element `i` to the back of the `vector` (by copy/move, just as with other function arguments), increasing the size of the `vector` by one.

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/vector.cpp#L1-L18

As seen in this example, just as with normal arrays, `vector`s (and all other standard library containers) support the range-based for loop.

`vector` is typically implemented as doubling the size when its internal "buffer" array is full.
This leads to many copies/moves being made, when the buffer is resized and previous objects need to moved over to the new buffer.
`vector` chooses whether to do a copy or a move in this order of preference:
1. `noexcept` move constructor
2. copy constructor
3. (non-`noexcept`) move constructor

This is because a move constructor that throws is more dangerous than a copy constructor that throws, since the move constructor can modify the object being moved, whereas the copy constructor leaves the other object unchanged.
For more details, see [this SO question](https://stackoverflow.com/questions/28627348/noexcept-and-copy-move-constructors).

To slightly improve performance, there is an alternative to `push_back` to adding new elements to a `vector` called `emplace_back`.
`emplace_back` takes the arguments of an object constructor and initializes the object *in* the `vector`, avoiding the copy or move necessary when creating the object then using `.push_back`:

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/emplace.cpp#L1-L25

This example also shows what happens when the internal array needs to be resized.

Using `[i]` to access elements of the `vector` directly accesses the underlying array, without checking if the index is out of range.
`vector`s also provide `.at(i)` which accesses the element after checking that `i` is in range, and throws `out_of_range` if it is not.
There is, of course, some overhead to doing this.
Tests show that it is around 10%, but this will differ on different machines and architectures.

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/range.cpp#L1-L24

On my machine, the undefined behavior from the out of bounds access using `v[10]` runs without issue, which is very scary.
This can easily lead to bugs occuring later on, if the programmer never notices this and the undefined behavior starts behaving differently on a different machine or on a different run.

Finally, like other containers, `vector`s provide several standard operations:
- `operator==` checks if all the members are equal and in the same order, using `==` on the members.
- `operator<` checks whether the elements are in lexicographic order, using `<` on the members.
- `swap`, copy/move constructors/assignments are all defined, which use the corresponding operations on its members.

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/operations.cpp#L1-L36

Of course, for these operations to work, the corresponding operations on the type being held inside the `vector` must be defined, like in the `test` class above.

Note that in these examples we have sometimes initialized `vector`s with their contents inside `{}`, like an array.
This differs from how we've been initializing objects.
`vector<int> v {1, 2, 3}` does not call a constructor of `vector` with arguments 1, 2, and 3, but it rather calls a different argument which takes the whole `{1, 2, 3}` as one argument of `initializer_list`, which initializes the contents of the `vector`.
This [std::initializer_list](https://en.cppreference.com/w/cpp/utility/initializer_list) should not be confused with the [member initializer list](https://en.cppreference.com/w/cpp/language/constructor) in the constructor that we've seen before.
This approach is common for container classes.
If you do want to call a constructor of a container class with arguments 1, 2, and 3, you need to use `()`: `vector<int> v(1, 2, 3)` (though no constructor of that type exists for `vector`).

### `array`

An `array` (in the `<array>` header) serves as a replacement for built-in fixed-length arrays ("C arrays").
Benefits to using `array` are that it stores its size (using `.size()`, like other containers), doesn't implicitly convert to a pointer like a C array, and has other convenience functions like bounds-checked access using `.at(i)`.
There is *no* runtime overhead at all to using `array` over C arrays, since the `array` operations can be translated to operations over a C array at compile time.
If you are using plain C fixed-length arrays on the stack, then you should be using `array` instead!

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/array.cpp#L1-L34

This code is also an example of template argument deduction, where some type arguments (in `<>`) can be omitted if the compiler can deduce them.
Here, the number of elements and their type can be deduced.

### `list`

`list` (in the `<list>` header) provides a doubly-linked list.
In addition to the common operations that other containers provide, `list`s also allow you to push or emplace to the front:

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/list.cpp#L1-L16

As usual with a linked list, direct access to elements via `[...]` is not available.

There is also a class for singly-linked lists, `forward_list`, but this class is meant for extremely resource-constrained environments that cannot handle the overhead of the second pointer at each node in `list`.
For this reason, this class also does not provide `.size()`, saving memory by not storing the size of the list.

### `deque`

A double-ended queue, or deque (pronounced "deck"), has two usual implementations: as a doubly linked list and as an array that can be extended in either direction.
Since `list` already provides a doubly-linked list, `deque` (in the `<deque>` header) provides the other implementation.
This class is similar to `vector`, providing fast access to any element and fast insertion and deletion to both the front and the back.
The underlying implementation typically uses multiple arrays, so it is not strictly better than `vector`.
Elements are not guaranteed to be contiguous, and require multiple pointer dereferences to access the array it is in, as opposed to just one dereference for a `vector`.
It also typically needs more memory than a `vector` to store the additional implementation details.

### Container Adapters

C++ also provides some containers which are an alternative interface on top of one of the above containers.
These are called container *adapters*.
`stack` and `queue` (in `<stack>` and `<queue>` respectively) are fairly simple and use a `deque` by default, though `list` can also work for either, and a `vector` is suitable for `stack`.
`priority_queue` (which is also in `<queue>`) implements a priority queue, providing constant time access for the largest element, and logarithmic time for deleting the largest and inserting other values.
This uses a `vector` by default, though `deque` is also acceptable.
Implementation-wise, a `priority_queue` uses the underlying container to define a heap.

The underlying implementation can be specified as the second template argument.
For example, `stack<int, list<int>> s` would create a stack of `int`s that uses a `list<int>` as the underlying container.

### Comparison of Sequence Containers

| Container | Insertion | Access | Deletion |
| --- | --- | --- | --- |
| `vector` | O(1) at back<br>O(n) otherwise | O(1) | O(1) at back<br>O(n) otherwise |
| `deque` | O(1) at front and back<br>O(n) otherwise | O(1) | O(1) at front and back<br>O(n) otherwise |
| `list`/`forward_list` | O(1) after reaching the element | O(1) at front and back (back only for `list`)<br>O(n) otherwise | O(1) after reaching the element |
| `priority_queue` | O(log n) | O(1) for the max | O(log n) for the max |

## Iterators

How do we actually use a `list`?
We know how to insert or delete from the front and back, but linked lists should support fast inserting anywhere in the middle of it, as long as you have already moved to that spot in the list.
Similarly, if you've looked at the documentation, you might have seen that other containers like `vector` support insertion in the middle of it, but this interface does not take an index to the position to insert at.
For these sorts of operations, C++ uses *iterators*.

Iterators are objects that represent an element from a sequence.
Each container has a type member `::iterator` (e.g. `vector<int>::iterator`) for the type of its iterator.
Iterators have a similar interface to pointers, allowing for "dereferencing" via `*` and `->` to access the element they represent.
To make an iterator, every container has the `.begin()` member function which returns an iterator to the first element of the container.
To check whether you've arrived at the end of the container, every container also has the `.end()` member function which returns an iterator to the element following the last element in the container---a nonexistent element which represents the end.
See the image on [this page](https://en.cppreference.com/w/cpp/container/vector/begin) for a visual representation.
Finally, one can move to the next element in the sequence using `++`:

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/iterator1.cpp#L1-L16

As a side note, the range-based for loop (e.g. `for (auto i : l)`) uses iterators and thus works for any container that provides them.

There can also be `const` iterators, which disallow using the iterator to change the element it refers to.
The type of the const iterator is given by the `::const_iterator` member and these iterators can be created using `.cbegin()` and `.cend()`.

Further, remember that iterators represent elements from a *sequence*, not a *container*.
This means that iterators do not always have to follow the container's order.
Containers also provide reverse iterators, which allow one to iterate through the container from back to front, where `++` goes "backwards" in the original container order.
Iterators to these can be created using `.rbegin()` and `.rend()` (and `.crbegin()` and `.crend()` for the `const` versions).
The image on [this page](https://en.cppreference.com/w/cpp/container/vector/rbegin) shows a visual representation.

### Types of Iterators

Iterators of different containers have different capabilities.
Iterators of a `vector`, for example, should be able to move forward an arbitrary number of elements, since `vectors` are contiguous blocks of memory.
On the other hand, iterators of a `list` would not be able to do this in constant time, and would rather require using `++` repeatedly.

These capabilities form a hierarchy, though not a class hierarchy.
While subtyping would be a good fit here, there would be a performance hit due to the many virtual calls.
Rather, each iterator type is a completely independent class.
To write functions that can take any kind of iterator as an argument, we'll have to use template arguments, which we'll see next class.
The same is true of the container classes, which also do not form any class hierarchy.

Here is a simplified description of the iterator hierarchy:
- All iterators can be incremented (`++`) to go to the next element of its sequence, by compared for equality (`==`, `!=`), and access the element it represents (`*`, `->`).
The iterators that can *only* do these things are called *forward iterators*.
- If the iterator has the additional capability of being decremented (`--`) to go backwards in the sequence, they are called *bidirectional iterators*.
- The most powerful kind of iterators are *random-access iterators*. These can go forwards or backwards in the sequence for any number of steps in constant time (using `+`, `-`, `+=`, `-=`), and can be compared for inequality (`<`, `<=`, `>`, `>=`) to check which iterator comes before another in the same sequence.

For example, `forward_list` provides forward iterators, `list` provides bidirectional iterators, and `vector` and `deque` provide random-access iterators.
The kind of iterator that each container provides should make sense, since they reflect how each container is implemented.

While some of these operations are not available for all kinds of iterators, some of them can be simulated.
The `<iterator>` header provides functions like `advance(it, n)`, which advances the `it` iterator forward `n` times.
For a random access iterator, it will choose to use `+=` and run quickly, but for a forward iterator it will use `++` `n` times and run much slower.
As another example, `distance(first, last)` returns the distance between two elements of a sequence.
It just uses `-` for random access iterators, but for other kinds of iterators it will count the number of `++` necessary to get from `first` to `last` (using `==` to check when to stop).
This is undefined behavior if `last` is not actually reachable from `first`.

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/iterator2.cpp#L1-L14

### Iterator Invalidation

All sequence containers provide the `.insert(...)` and `.erase(...)` member functions, which take iterator arguments to determine where to insert into or erase from the container.
These operations must be used with care, however, since they can *invalidate* existing iterators into the container.
For example, erasing an element in the middle of a `vector` requires shifting everything after it down to take up the erased room, so any iterators that represent those elements would become invalid, and using them would be undefined behavior.

In this following example, we erase the element represented by an iterator from a `list`, which obviously would invalidate that iterator.
To deal with this, we use the return value of `erase`, which returns an iterator to the element after the erased element.
To prevent the element following the erased element from being skipped in the loop, the loop only increments `it` if nothing was erased.

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/iterator3.cpp#L1-L27

See [this chart](https://en.cppreference.com/w/cpp/container#Iterator_invalidation) for a summary of the invalidation rules.
When using operations that modify the container, check the documentation to see if the return value  provides a way to deal with potential iterator invalidation, like with `erase` above.

## Associative Containers

Associative containers are those that store a collection of key-value pairs (or just keys without a value) which are not ordered by the user.
The simplest implementation of an associative container is just a sequence container of `pair`s of keys and values.
However, this is clearly not very efficient, and the C++-provided associative containers will be much better.

### `map`

A `map` (in the `<map>` header) is an associative container that is typically implemented as a balanced binary search tree, like a red-black tree.
`[]` returns a reference to the value corresponding to a key if the key exists in the map.
If it doesn't yet exist, it will be added with a corresponding default-initialized value.

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/map1.cpp#L1-L21

Other familiar member functions are also available, like a checked access using `.at(i)`, `.size()`, `.empty()`, and `.erase()`.
`.insert(...)` and `.emplace(...)` are also available, though they do not take an iterator to the location to insert at, since `map`s maintain their own order.
Iterators are also available for `map`s, and iterating through a `map` will always go in sorted order.

To use a custom class for the key type, this class should be comparable using `operator<`.
`map` will *only* use this single operation.
If neither element is less than the other (i.e. `!(a < b) && !(b < a)`) then the `map` will consider `a` and `b` equivalent.
In the following example a wrong implementation of `operator<` results in two objects not being considered equal when they are in fact equal:

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/map2.cpp#L1-L45

Other ordering operations (like `>` instead of `<`) can also be specified for a `map`, which we'll see when we talk about function objects in a few classes.

### `set`

A `set` (in the `<set>` header) is essentially the same as `map`, but with only keys and no values.
Rather than `[]` or `.at(i)`, the `.count()` or the `.contains()` member functions are more useful here.

### `unordered_map`

`map` uses a BST, which has logarithmic complexity for its operations.
Hash tables can do better than this, with amortized constant-time operations.
This is what `unordered_map` (in the `<unordered_map>` header) implements.
The public interface is basically identical to `map`, though an ordering operation is not required.
Rather, a hashing function is used for the key type, which is already provided for all the built-in and standard library types.
Unlike other languages where the hashing function is a member function, C++ does not have a common base class for all classes.
This approach requires virtual calls for common functionality like calling the hash function, which is additional performance overhead.
As with custom comparison operations, we'll cover custom hash functions when we talk about function objects in a few classes.

https://github.com/CIS1900/2022-fall/blob/900cc0884992eacce339efeef7eabbb81cd12a8d/09/unordered_map.cpp#L1-L21

Like its name suggests, `unordered_map` is not ordered, so iterating through one using iterators does not always go in sorted order.
Prefer `unordered_map` to `map` for the improved performance, unless the sorted order iteration is useful for your use case or writing a good hash function for your key type is too difficult.

### `unordered_set`

Similar to `set`, `unordered_set` (in `<unordered_set>`) is the key-only version of `unordered_map`.

### `multi`-versions

In addition to these four associative containers, each of them have a version which supports multiple instances of keys: `multimap`, `multiset`, `unordered_multimap`, and `unordered_multiset`.
These classes are found in the corresponding non-`multi` headers, not in their own headers.

For the `multimap` and `unordered_multimap`, instead of using `[]` or `.at(i)` (which are not available), member functions like `.equal_range(key)` are more useful.
This function returns a pair of iterators representing the range of elements that match `key`.
