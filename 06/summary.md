# Class 6 summary

I covered fewer questions in class than normal, to make more time for working on exercises and the assignment.

I went over the Canvas quiz, since most of the answers were wrong.
I did a pollev quiz which asked which operations can throw exceptions:
- dereferencing a pointer
- dividing by zero
- calling `new`
- accessing an array at an out of bounds index

In fact, only calling `new` will throw an exception.
The other operations can fail, but when they do it's an instance of undefined behavior (or something like NaN or infinity if you're dividing floats), and your whole program is now compromised.
In languages like Java that do throw exceptions when things go wrong, the program must check to see if something has gone wrong.
When a reference is dereferenced, the Java program will first check if the reference is null, and if so, throw a null pointer exception, and otherwise actually do the dereference.
This is a good deal of overhead when you are doing many possibly-dangerous operations, all of which requires an extra hidden check.
In C++, these checks are up to the user.
If you dereference a pointer, you better know that it will work or your program will blow up.
In Java, that would throw an exception and you could instead recover.

I would guess that the only reason that `new` throws is that it has to do a check for whether there is enough memory no matter what, so the check doesn't add any additional overhead.
In general, only functions written by others or from the standard library will throw, and you should consult documentation to see what can be thrown.
Most built-in operations will not throw.

This leads to Canvas quiz question 1:
```c++
integer & operator=(const integer & other)
{
    *p = other.get();
    return *this;
}
```
This has a no-throw guarantee.
While it could fail due to a bad pointer dereference, that failure would not throw an exception.

Question 2 asks
```
Suppose we are coding a movie database, and trying to model a user's favorite movie. What kind of smart pointer would be most suitable for modeling a pointer to a movie in the user class?
```
The right answer is `weak_ptr` because with a movie database, the existence of a movie should not depend on whether users have it as their favorite movie.
If we used `unique_ptr` or `shared_ptr`, the movie would be deleted if nobody has it as their favorite.

Question asks whether a function-try-block is necessary for the following copy constructor:
```c++
integer(const integer & other) : p {new int {other.get()}} {}
```
The answer is no, since if the initializer list did throw, the function-try-catch would not have anything to do.
The example in the notes had another pointer to delete, but this example does not have any other members that need to be cleaned up.

Someone asked how we would handle this code if it failed.
The `new` would throw an exception and propagate out to the caller of this constructor, whoever tried to create an `integer`.
It is their responsibility to catch the exception and handle it.
Inside `integer`, there's nothing we can do.
We *need* this memory to be a proper `integer`, and there's nothing we can do to fix this error.

Questions from Canvas:
- Throwing inside a try block vs throwing not in a try block: If there is no enclosing try-catch block with a suitable catch clause, it will eventually pop every off the stack at main and terminate the program with a message saying something like "terminate called after throwing an instance of <whatever type was thrown as an exception>".
If there is an enclosing try-catch block with a suitable catch, it will run the code in that catch and then continue at the line after that try-catch block.
- Example of no exception safety: The `integer` class from the notes has an invariant: that its pointer is valid and points to an `int`. If, instead of allocating that `int` in the constructor, we added a member function which just calls `new` to allocate the memory, then that function would have no exception safety. If the `new` call throws, then the entire object is broken, and if you try to call `get()` on it, it will result in dereferencing a null pointer.
- Move and swap: This is not really necessary, since move operations typically cannot fail and throw and exception. I mostly used it to reduce amount of repeated code. Indeed, it actually slows things down since it does more operations (one move for the temporary object plus the swap, which is like a move) instead of just one move.
- RAII: The name is pretty bad, but RAII is just the concept of replacing functions like `open()`/`close()` for managing resources with object lifetimes, or constructor/destructor.
This is a bit more elegant and convenient, since now you don't have to clean up your resources manually since the destructor will do it manually.
But this is also essential to handle exceptions.
Since there is no `finally`, the only cleanup code that is guaranteed to run no matter how an object goes out of scope is the destructor.
- Smart pointers in general: smart pointers are RAII classes for managing pointers.
Dynamic memory uses `new`/`delete`, which is exactly that `open`/`close` pattern above, which doesn't work with exceptions.

- Should I ever use a raw pointer over a smart pointer: The main case is when you have to use a C library which expects raw pointers. You may also see people use raw pointers for temporary ownership similar to how `weak_ptr` is used. That can be more convenient if the main owner is a `unique_ptr`, but I recommend just changing it to a `shared_ptr` and using the `weak_ptr` to "borrow" the pointer.
Sometimes you may have to use other code that chose to use a `unique_ptr` and this may not be controllable.
- If we don't know how many owners there are, should we use a `unique_ptr` over a `shared_ptr`: You need to know how many owners there are! If it is this unclear then you will likely run into issues.
If you mean that it's a case like multiple objects may share ownership of another object, and it's determined at runtime, then `shared_ptr` sounds perfect for this purpose.
- Purpose of `weak_ptr`: `weak_ptr`s model temporary ownership, where this pointer should not control when the memory is deleted, but can still temporarily be used to access the memory.
They are mostly used for two reasons:
1. to avoid dangling pointers (e.g. exercise 2, the movie database example).
Dangling pointers are pointers that still exist to memory that has been freed.
When you try to temporarily acquire the `weak_ptr` for use using `lock()`, it does a check to see if the original memory is still valid, avoiding dangling pointers.
2. to break cycles in `shared_ptr`s. Without `weak_ptr`, `shared_ptr`s would not be able to represent cyclic data structures without memory leaks, since their reference counts would never go down to zero.

More specific questions or questions asked later that I didn't get to:
- Why implicitly defined functions are `noexcept` by default: It's more that implicitly defined functions as "as `noexcept` as possible", they are `noexcept` if everything they depend on (the members' corresponding functions) are `noexcept`, which is the most "precise" characterization you can get.
- How to avoid memory leaks during copying: The annoying answer is to use smart pointers. But in general when dealing with dynamic memory the key is to think about ownership. Basically all pointer bugs are a result of improper sharing. Think about who owns each pointer, meaning who is responsible for freeing that memory. Smart pointers take care of much of this for you, which is why they are preferred.
- How to provide a custom deleter: see the `Deleter` argument for [unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr). This is a *template argument*, and we will talk about this in a later class.
- When would we need to write a function-try-block: To handle the case when there can be exceptions in the initializer list, and to do cleanup with that happens (see question 3 on the canvas quiz).
- Can we ask the system if there is enough memory left for a copy, in the context of copy-and-swap: This is possible but very OS-specific. e.g. see this [SO question](https://stackoverflow.com/questions/2513505/how-to-get-available-memory-c-g).
Typically in cases when you might be very memory starved you know exactly what sort of machine you are going to run your code on, so you can rely on those specific solutions.
If you are writing portable code that should run anywhere you can probably assume that it has sufficient memory.
In practice, few programmers care about `new` throwing, unless they are coding for a specific system with extreme memory constraints.
- Other examples of exception safety guarantees: e.g. the string `resize` function resizes the string it is called on, and can throw if the size you pass is too big. If it throws, the function has no effect on the string it was called on, which is a strong guarantee (and is mentioned in the [documentation](https://en.cppreference.com/w/cpp/string/basic_string/resize)).
If the implementation instead started resizing and then threw, that would be a basic guarantee. The object has been changed, but not in a way where it will "break" things.
- When something needs a try-catch: To check if something can throw, you should consult documentation. For the standard library, the docs will state what, if anything, they can throw.
- When would you not throw in a constructor: If the error does not completely break the object, you could recover in the constructor and successfully construct an object.
For example, for file streams they do not throw if the filename is invalid, but they just become an "invalid" stream object, which can be reset later with a new filename.
- Benefit of custom swap vs. `std::swap`: The benefit is just the small performance boost. `swap` can be thought of as a sibling of move operations, something you might want to write if you deem your move operations to be too slow for `std::swap` to be acceptable.
Another reason is that your class might not have move operations, in which case `std::swap` wouldn't work, and you would have to implement your own `swap` if you need it.
- How copy & swap works: The point is to not change the original object being assigned to if something goes wrong, so this first makes a copy to a temporary object. That's the only step which can throw, so if it does nothing in the original object has changed. If it succeeds, we then do a swap (which cannot throw) to change the original object. The swap puts the original members into the temporary, which will clean up those members on destruction.
- How to tell a function's exception safety level: The documentation should say. If it doesn't, then you may have to look at its implementation.
- When should we use `setjmp`: in C++, never. In C, this could be useful if you are trying to emulate exception handling and jumping to a handler.
- Swap under the hood: It's implemented as 3 move operations, described in the notes. This means that `std::swap` will only work for classes with move operations defined.
- Difference between try-catch in constructors vs elsewhere: There's not really a difference, assuming you're talking about try-catch, and not function-try-blocks.
- Release assignment before class: I am leaning towards not doing this, mostly because I really want students to look at the assignment together with their partner, and not on their own during or before class.

Exercise 1:
None of the three bugs are possible if we're using `unique_ptr`s properly.
A shallow copy is not possible, since `unique_ptr`s cannot be copied---their copy constructor/assignment are both disabled using `= delete`.
Not resetting the other object's pointer on a move is also not possible, since the class moves a `unique_ptr`, which will take care of resetting the other `unique_ptr`'s internal pointer.
Lastly, not deleting our old memory before updating the `unique_ptr` is also not possible, since changing the internal pointer of a `unique_ptr` will clean up the old memory on its own.
Of course, it is possible to misuse `unique_ptr` and get bugs, for example by using `.release()` and not deleting the returned pointer, or by using a stale pointer obtained from `.get()`.

Exercise 2:
Using `unique_ptr` wouldn't work, since then we wouldn't be able to have multiple objects refer to the same node.
With `shared_ptr`, we would be able to have multiple pointers to the same object, but then we run into the problem of cycles, and the memory leaks that result.
One solution would be to break cycles using `weak_ptr`s, but it's unclear when to insert `weak_ptr`s instead of `shared_ptr`s for a general setting.
The solution I talked about in class was to hold all the nodes in the graph in `main` (or maybe some sort of `graph` class) using `shared_ptr`, and have each node reference other nodes using member variables of type `weak_ptr<node>`.
In this approach, no node owns another, but we can still have nodes see others.
