# Class 9 summary

Questions that I answered in class:
- Are C arrays never used now: basically, since `array` is better for fixed-length arrays, and `vector` is better for variable-length ones.
- Vectors under the hood: The standard does not require it be implemented in a specific way, but most implementations use an internal buffer which gets doubled in size when it runs out of room.
- Advantage of using iterators over indexing, for containers that support it: Not much perforamnce benefit, since if the container supports indexing, then it will be able to do it quickly. The main benefit is using containers in a uniform way, which can be useful for writing generic functions (as we'll see in the next class's notes).
Many functions also have an iterator interface, like `insert` and `erase`.
- What is an iterator, and compare to Java iterators: An iterator is a value that is kind of like a reference to an element.
It doesn't have to be an object, for example for `vector` a pointer would work as the iterator.
In the end we don't really know or care how it is implemented, we just want the operations it supports (dereferencing, incrementing, etc.).
Compared to Java iterators, the motivations for having them seems very similar, but the interfaces are very different, with the C++ iterators having a "pointer-like" interface.
e.g. instead of `hasNext` you need to compare against the "end" iterator, like you would with a pointer.
- Most common containers in practice: `vector` for sure, and for the associative containers `unordered_map` and `unordered_set` are popular as well.
- Why `unordered_set` over `set`: `unordered_set` is faster. The main reason to use `set` is if you need the fact that keys are already sorted.
- Are containers on the heap or stack: The container itself is on the heap/stack depending on how you created it (normal variable vs. `new`), but the elements inside the container are typically on the heap.
This doesn't mean you need to manage them directly, the container will manage them for you, so you basically don't have to think about the details of the heap/stack.
- Why use `push_back` over `emplace_back`, since `emplace_back` can just call the copy constructor: Really interesting question! I didn't know in class, but after doing some searching, it seems that there are some differences, though they are quite subtle: https://stackoverflow.com/questions/22080290/are-there-any-cases-where-it-is-incorrect-to-replace-push-back-with-emplace-back.
I would use `push_back` if you already have the object created, and `emplace_back` if you are creating the object on the spot.

Questions I didn't get to in class:
- Writing custom containers: You can, but most common container types are already covered by the standard library. If you do want to write one you should try to stick to the same interface, so we can abstract over the container type using templates (see class 10 notes!).
- Erasing elements: Calling `erase` will invalidate the iterator you called it on, and possibly more iterators. The safe thing to do that should work in all cases is to use the return value of `erase`.
- Iterator over a for or for-each loop: It depends on your use case. If you need the iterator, e.g. if you want to use `insert` inside the loop, then you should use a `for`. If you only want to go through the elements of the container, then a for each loop sounds perfect.
- How are `multi...` containers implemented: it's not specified by the standard, so implementers are free to implement them however they want. I think something like a red-black tree would work, by storing each duplicate separately doing a few more checks to look for nearby duplicate keys.
- `auto next_it {it + 1}; l.erase(it); // then do something to next_it`: this depends on what kind of iterator it is. For a `list` iterator it would be ok, but for a `vector` iterator it wouldn't be, due to iterator invalidation. The `erase` would shift all later elements, making `next_it` invalid.
- `noexcept` and move constructors: if you are defining the move constructor manually, you need to label it as `noexcept` yourself.
See the [notes about `noexcept`](https://github.com/CIS1900/2022-fall/tree/main/06#noexcept)
- Implementation and efficiency: The C++ standard does not mandate implementation, so the implementers of the standard library are free to do whatever they like, which is why I don't really talk about implementation too much, since they can differ between compilers. The standard does often require efficiency (big O) for certain functions, which constrains some implementations, like `list` being a doubly-linked list, or `unordered_map` being a hash table.
The asymptotic complexity is fairly standard, with sequence container runtime specified in the notes, and the associative container runtimes being the standard ones for self-balancing BSTs and hash tables.
- why is `map` a BST rather than a hash table: because they wanted to offer both data structures. I guess they could have made `map` be the hash table, and `ordered_map` the BST. I would actually prefer that since the hash table implementation is typically preferred, and this way it seems more like the "default" choice.
- Custom comparators for `set` and `map`: We'll talk about this in 3 weeks, when we talk about function objects (called functors in C++).
- `array` vs `vector`: The main difference is that `vector`s are resizable. You should prefer `array` if you know that you only need a constant amount of space, and it will save on space since `vector` needs to keep a slightly larger buffer than necessary.
Another advantage is that you can use the `get` member function to access elements of the array, which will be checked at compile time to be in bounds or not, completely avoiding the risk of out of bounds accesses.
- Iterating through `unordered_set`/`_map`: The iteration returns a fixed, but unspecified order (i.e. it probably won't change randomly, though I wouldn't rely on this). It would probably be based on the hash values.
- does `erase` take care of rearranging the container: yes, you don't have to manually do anything. `erase` will take care of things like shifting values (for `vector`) or fixing pointers (for `list`)
- Is it good practice to have `using namespace std` everywhere like in the examples: It's not really good practice, I just do it because it's convenient. But it's not a huge deal if you're doing it in a .cpp file.
- Which container would be best to implement a `stack`: `stack` uses `deque` by default. It depends on the usual tradeoffs between these containers, but you could use `list` or `vector` as well.
- How package management works in C++: it doesn't really work. There are many different competing package managers, and no real consensus on which to use. https://github.com/microsoft/vcpkg is one popular one. Many people (including me) still install and upgrade libraries manually.
- Why did the code in `random.cpp` from the project code use `vector`: We need some container to store `width * height - 1` elements. `array` doesn't work since it can only store a constant (known at compile time) amount of elements, so `vector` is basically the next default choice. Other containers like `list` or `deque` could have worked too.
Using a `unique_ptr<pair<int, int> []>` directly wouldn't have worked because I needed `begin` and `end` for calling the `shuffle` function.

The exercise solutions are here: [`exercise.cpp`](exercise.cpp)
It's more common and idiomatic in C++ to write functions like this to take iterator arguments.
That way you can abstract over what kind of operations you can do (e.g. special case for random access iterators) at the iterator level instead of at the container level, and so you can just apply your algorithm to a portion of the container, instead of the entire container.
This is the case for the functions in this week's assignment.
