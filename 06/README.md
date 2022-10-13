# Class 6

In these notes we'll continue on from last time, and talk about what happens to objects that manage resources when things go wrong, and how this influences many parts of C++.

## Exceptions

### Exception handling in C

In C, when things go wrong there are several ways that the code can inform you of this, but there are significant weaknesses to using all three in C++.

The first is error codes, but these are easy to ignore.
For example, did you know `printf` returns the number of characters printed, and returns a negative number if something goes wrong, like if stdout is inaccessible?
These are possible to simply ignore, which can lead to serious errors being simply overlooked.
Programmers often ignore these return codes, and if they didn't ignore them, their code would be full of if statements and be much harder to read.

The second way is a global variable named [`errno`](https://man7.org/linux/man-pages/man3/errno.3.html) (error number), in the C header `errno.h`.
This variable may be set by system calls or library functions to indicate, via an error code, that something went wrong.
You can then look up the error code in a table to see what the error is.
Since this is just a single variable, if two errors occur then the second error code will overwrite the first one, so it is important to check after each operation.
Most programmers rarely use this, and it is again easy to ignore.

Lastly, C provides the [`setjmp.h`](https://en.wikipedia.org/wiki/Setjmp.h) header, which allows the programmer to jump to an arbitrary spot in the code.
It is effectively a `goto`, where the spot you want to jump to can be determined dynamically, rather than having to be specified directly in the code.
The functions in this header can be used to simulate exceptions in other languages, by setting the "exception handler" as the spot to jump to, and by jumping to it if an exception is raised.
Since this is a C library, it naturally does not have specific handling of objects, and would not properly destroy the objects that are no longer in scope due to jumping to a different part of the code.

### Exceptions in C++

C++ supports exceptions, similar to those found in many other languages like Java.

To throw an exception, signifying that something has gone wrong, use `throw e;`.
When this happens, the stack is *unwound*, which means that stack frames are popped one by one, and execution moves "up" the call stack.
As the stack is being unwound, it looks for the first place where execution is inside a `try` block with an associated `catch` clause that can catch `e` according to its type (similar mechanism as a function call).
This `catch` clause is an *exception handler*, which should handle the error that caused the exception, and continue from that point.
This is known as termination semantics, where the code that threw is just "terminated", and we abandon it, though you can repeat it if you wish using some additional code.
Alternatively, there are resumption semantics, where the exception handler is meant to fix whatever went wrong and the throwing code is "resumed".
Some other languages support this model, but C++ is not one of them.
If there is no appropriate `catch`, then the stack continues to be unwound until an appropriate `catch` is found, or if there is nothing left to unwind, in which case the program will crash.

https://github.com/CIS1900/2022-fall/blob/5312b53b9930a6616fdb9a81c3733450737a386b/06/exceptions.cpp#L1-L34

As we see in this example, unlike other languages languages, in C++ we can throw any type, not just exception types.
The `<stdexcept>` header does contain a base `exception` class and some subclasses representing some common exception types.
This class provides a `.what()` member function, which returns an error message associated with the exception.

Each `catch` clause is like a function, where we provide the type of exception we can handle.
Like with functions, we can `catch` by value or reference.
Catching by value results in a copy, just like with functions, and also suffers from object slicing in the case when you are throwing a derived class and are catching a base class.
For these reasons, you should almost always catch by `const` reference, to avoid the copy (since it's unlikely you want the thrown object to be copied), and to be able to use the thrown object polymorphically if there is inheritance in the mix.
Multiple `catch` clauses are processed in order from top to bottom, and the first matching one is used.
You can also use a special `catch(...)` clause which catches any exception, so it should be the last `catch` clause.
Inside the catch clause, you can rethrow an exception (or a different object) to restart the process, unwinding the stack at the current spot to find another suitable exception handler.
You can also use `throw;` to rethrow the caught exception, which is useful for the `catch(...)` case, where the exception itself is unnamed.

Note that there is no `finally` clause as in Java.
Any cleanup that might need to be done when an exception is thrown should be handled by a destructor.
One very important invariant in C++ is that whenever an object is destroyed, like when it goes out of scope or when a pointer is `delete`d, its destructor will be called.
This includes when the stack is unwound due to an exception and variables go out of scope, so destructors can be relied upon to always be called.
However, this means that destructors should never `throw`!
If an exception is thrown *while* stack unwinding is happening due to another thrown exception, your program will crash.

### `noexcept`

If a function should not ever throw, it can be labeled `noexcept`.
If the `noexcept` function does end up throwing, instead of propagating the exception, the program will instead terminate, even if there is an exception handler available somewhere.

https://github.com/CIS1900/2022-fall/blob/5312b53b9930a6616fdb9a81c3733450737a386b/06/noexcept.cpp#L1-L21

Implicitly-defined functions are `noexcept` by default, unless one of the functions that they automatically call (like the copy constructor of a member variable, for the implicitly-defined copy constructor) is not `noexcept`.
If you are defining these functions yourself and they are not supposed to throw, you should label it as `noexcept` yourself.

## Exception Safety

There are several levels of "how safe" your function is, with regards to exceptions.
These levels are useful as a contract to users of your function, so they know what to expect if your function throws an exception.

Here are the different levels of exception safety, from the strongest, or most guarantees, to the weakest, or least guarantees.
1. *No-throw guarantee*: This says that the function will always succeed, and never throw any exceptions.
2. *Strong guarantee*: This level says that if an exception is thrown, then the operation being performed is guaranteed to have no side effects.
The failed operation is "rolled back", as if it never happened.
3. *Basic guarantee*: This says that if an exception is thrown, the operation is guaranteed to not leak or break invariants, but it may have side effects.
For example, the values being stored may change, but it will not crash due to a memory issue.
4. *No exception safety*: No guarantees are made here.
You may as well not use exceptions at this point, since you can't even guarantee that the code doesn't break when the exception is thrown.

See the [wikipedia article](https://en.wikipedia.org/wiki/Exception_safety) for more details.

## Copy/move and Swap

If a class has a lot of memory to copy in its copy assignment, it would have a basic guarantee, since it could succeed at copying some memory, and run out of memory to allocate (and throwing an exception) in the middle.
This results in a half-copied object, but will probably not break the object, leading to a basic guarantee for this copy assignment.

One classic way of getting a copy that has a strong guarantee is called *copy and swap*.
Instead of making the copy directly into the object, we first copy into a third object, and then swap the contents of that object with the contents of our original object.
If the copy fails, the original object being copied into is not affected, only the temporary object is, and that temporary object can be discarded without issue.
The swap never occurs since an exception was thrown and would skip the rest of the copy assignment.
An example with this, using the `integer` example from last class, is as follows:

https://github.com/CIS1900/2022-fall/blob/5312b53b9930a6616fdb9a81c3733450737a386b/06/swap1.cpp#L1-L74

We use `std::swap` to swap members of the two objects, (found in the `<utility>` header, and some other standard headers) which is `noexcept`.
So now the only possible source of exceptions is the copy/move constructor, and if it does throw, the original object is unchanged, giving us a strong guarantee.

We also do the same with move assignment in the above code, a *move and swap*, but for a different reason.
The move assignment cannot fail, since move operations are typically safe, just moving pointers around and not allocating any new memory.
The reason we use *move and swap* is to reduce redundancy.
The previous implementation has the copying and moving logic repeated, in both the copy/move constructor and the copy/move assignment.
Using copy/move and swap, we eliminate that repeated code, consolidating it in the copy/move constructor.

Note that the only difference between the two assignment operators is that the copy assignment makes the temporary using the copy constructor, and the move assignment makes the temporary using the move constructor.
Since we need a temporary anyways, we can just do this by passing by value!
If the value being assigned from is an lvalue, then C++ will choose to copy, and if it is an rvalue, then C++ will choose to move, which is exactly what we were doing before.

https://github.com/CIS1900/2022-fall/blob/5312b53b9930a6616fdb9a81c3733450737a386b/06/swap2.cpp#L1-L61

This code now only has one assignment operator, which makes a temporary using either a copy or a move, and then swapping using it.

Overall, using the copy/move and swap technique is a tradeoff.
There is additional time and space overhead in creating this third object, but it gives us better exception safety.
Depending on your use case, this may or may not be useful.

### `swap`

You may be wondering if we must swap the members of our objects in the code above, rather than just swapping the objects themselves (like `swap(*this, i)`).
The standard library `std::swap(T a, T b)` is implemented as something like `T temp {move(a)}; a = move(b); b = move(temp);`.
That is, `std::swap` is defined using move operations (and will work for any type that has move operations).
If we then tried to define our move assignment by swapping the entire `integer`, this would result in an infinite loop.
`operator=` would call `swap`, which would call `operator=`, which would call `swap`, and so on.

We can instead write our own `swap`, which can be a bit faster than the 3 moves implementation of `std::swap`.
Other functions also use `swap`, like the standard library sorting algorithms.
The definition of our custom `swap` is simple, and it is the same as what used to be in the assignment operator, just swapping all the member variables of our object.
Now in the definition of `operator=`, we can call our `swap` to swap the two `integer` objects:

https://github.com/CIS1900/2022-fall/blob/5312b53b9930a6616fdb9a81c3733450737a386b/06/swap3.cpp#L1-L68

Now that we have our custom `swap` and `std::swap`, how does C++ know which to use?
If `std::swap` was imported using `using`, then both can be called using `swap`.
When a situation like this occurs, C++ chooses the function in the "current" namespace using a process called [ADL](https://en.cppreference.com/w/cpp/language/adl).
Our custom `swap` is associated to `integer`, but the standard library `std::swap` is in another namespace, so even though it was imported, our custom `swap` is chosen.
Code in the standard library often does what we did in our definition of `swap`, where we write `using std::swap`, then call `swap`.
This first looks for a custom `swap` for that class, and if it doesn't exist, falls back to `std::swap`.

`swap` should be defined as a free function but also needs access to the members of your class, so it should be a `friend` of your class.
In the code above, we define the `friend` function in the class itself, which is a shortcut for writing the code in a single file.
This is equivalent to declaring it as a `friend` and defining it outside of the class, which wouldn't work for multiple files since then the definition would be in the header, leading to multiple definition issues.

## RAII: Resource Acquisition Is Initialization

RAII is a technique in C++ where resources are bound to the lifetimes of objects.

Once a resource is acquired via the constructor, it is guaranteed to be released via the destructor.
Instead of using functions like `open()` and `close()`, the resource is instead tied to the object lifetime---creation and destruction---allowing us to leverage the guarantees that C++ gives us about objects, and avoiding resource leaks.

In addition to convenience, this is the only way to prevent resource leaks in the presence of exceptions, as mentioned earlier.
While many projects simply prohibit programmers from throwing exceptions, it's harder than that to avoid exceptions.
Most standard libraries use exceptions, and even if you avoid those, built-in mechanisms like `new` can throw exceptions.

As an example of RAII in practice, the standard library provides a [`mutex`](https://en.cppreference.com/w/cpp/thread/mutex) class for mutual exclusion to be used for concurrency.
While `mutex` provides `lock()` and `unlock()` member functions, if an exception is thrown before the unlock, it would never run and the `mutex` would get stuck in the locked state, possibly resulting in a deadlock.
The RAII solution to this is to use an object to manage the acquired `mutex`.
The standard library provides a class called [`scoped_lock`](https://en.cppreference.com/w/cpp/thread/scoped_lock), which acquires a `mutex` lock on initialization, and releases it on destruction.
Thus, acquiring this `mutex` resource is now bound to the lifetime of this `scoped_lock` object, releasing the `mutex` once it goes out of scope, even if that is due to an exception.

RAII is the dominant way of managing resources in C++.
It both provides an elegant solution to the problem of remembering to manually acquire and release resources, and it is the only way to safely release resources in the presence of exceptions.

## Smart Pointers

Smart pointers are RAII classes that manages a pointer, avoiding the need to manually deallocate memory.
After learning about smart pointers, it should no longer be necessary to use regular, or "raw" pointers in your code.

### `unique_ptr`

`unique_ptr`s (in the `<memory>` header) model the case when you *uniquely* own a pointer's memory.
You should be the only owner of the memory, and nobody else should have a pointer to it, unless it's temporary and you know that they won't store it to use later.
A `unique_ptr` `delete`s its memory in its destructor, and these assumptions help guarantee that this operation is safe, that nobody else is holding a pointer to this same memory.

This class is generic, so it works for many different types.
The type of the value that it is meant to point to is written inside the `<>`.
It can even hold an array, which must be specified in the type as, for example, `<int []>`.
To initialize a `unique_ptr`, it can be initialized directly with the address it is meant to manage, e.g. `unique_ptr<int> i {new int {1}};`.
Alternatively, we can use `auto` and `make_unique`, e.g. `auto i {make_unique<int>(1)};`.
`make_unique` calls `new` for you, and calls the constructor using the arguments you pass to it.
This option is recommended, as it allows us to avoid repeating the type twice (annoying for long type names).
This also ensures that the pointer that the `unique_ptr` manages is really something that is owned solely by this object, instead of just any random pointer, which could be the case if we initialize it in the first way.

To use the `unique_ptr`, all the usual pointer operations, `*`, `->`, and `[]` for pointers to arrays, are overloaded to work the same way as normal pointers.
There are also several member functions:
- `get()` returns the underlying raw pointer, which can be useful if you are dealing with library code that expects a raw pointer.
Be sure not to just store and use this, however, since that would completely defeat the purpose of using a smart pointer.
- `reset(p)` which deletes the current pointer, and replaces it with a new pointer to manage, `p`.
Alternatively, you could call this without any arguments, which just deletes `p` and sets the pointer to a null pointer.
- `release()` which returns the underlying raw pointer, and releases it from being managed by this `unique_ptr`, setting the `unique_ptr` to null. This does not delete the memory, and basically gives up ownership of the memory to the caller.

`unique_ptr` disables the copy operations, but they can be moved.
This is because a shallow copy is clearly unsafe, it breaks the invariant that `unique_ptr` should model *unique* ownership.
A deep copy is also not clearly desirable, so it is disabled.
If you want a deep copy, you can do this manually by creating a new `unique_ptr` and initializing it with a copy of the memory pointed to by the other `unique_ptr`.

https://github.com/CIS1900/2022-fall/blob/5312b53b9930a6616fdb9a81c3733450737a386b/06/unique.cpp#L1-L42

Finally, it's also possible to customize what happens on destruction, which can be useful for holding other pointer resources, like `FILE *`, which must be deallocated using `fclose`.

### `shared_ptr`

`shared_ptr`s model the case when ownership of a pointer's memory is *shared* among several owners.
Internally, `shared_ptr`s keep track of the number of `shared_ptr`s pointing to the same memory, and will only `delete` the pointer if they are the last one remaining, a technique called *reference counting*.
Most other operations are the same as that of `unique_ptr`.

The most important thing to remember is that to create multiple `shared_ptr`s to the same memory, you must *copy* a `shared_ptr`, not create a new `shared_ptr` to the same raw pointer (e.g. *not* `int * p {new int}; shared_ptr<int> p1 {p}; shared_ptr<int> p2 {p};`).
Doing the latter will lead to the two `shared_ptr`s not knowing about each other, each thinking they are the sole owner of the memory, leading to a double free and thus undefined behavior.
Instead, to have multiple shared owners of the same memory, create a `shared_ptr` and copy it using either the copy constructor or the assignment operator.
This will properly use the internal systems of the `shared_ptr` to increment the reference count properly.

https://github.com/CIS1900/2022-fall/blob/5312b53b9930a6616fdb9a81c3733450737a386b/06/shared.cpp#L1-L33

### `weak_ptr`

`weak_ptr`s model the case when we want to temporarily access memory that is owned by some `shared_ptr`s, but we do not own it ourselves, meaning the actual owners could destroy the memory without our input.
`weak_ptr`s must be created from a `shared_ptr`, and cannot be used directly as with the other types of smart pointers.
To temporarily access the `weak_ptr`'s pointer, we must first call `lock()`, which attempts to access the original `shared_ptr`.
If the original `shared_ptr` no longer holds that memory, then `lock()` returns a null `shared_ptr`, signifying that this `weak_ptr` cannot access the memory anymore.
If that `shared_ptr` is still holding the original pointer, then `lock()` returns a copied `shared_ptr`, allowing us to use it to do standard pointer operations.
Since this made a copy, the reference count increased by one and it is guaranteed that the pointer stays alive for the duration of this `shared_ptr`.

https://github.com/CIS1900/2022-fall/blob/39c32843c1291e9838a9c48396def58a3e998d45/06/weak.cpp#L1-L41

One essential use of `weak_ptr` is to break cycles in `shared_ptr`s.
As shown in the code above, if two objects have `shared_ptr`s to each other, the reference count will never fall to 0, and the two objects will never be destroyed, leading to a memory leak.
By breaking this cycle of `shared_ptr`s using a `weak_ptr`, we can still have cycles without memory leaks.

## Exceptions in Constructors

If something goes wrong in a constructor, you should throw an exception.
Since they cannot return, it's difficult to signal failures in any other way.
However, if a constructor exits via a `throw`, the destructor is *not* called.
This is because destructors are only called for constructed objects, and since the constructor never finished running, it was not successfully constructed.

For exceptions that are thrown in the body of the constructor, they can be wrapped in a try-catch block.
However, there is also the initalizer list, which can also throw exceptions.
To handle this, you can use a *function-try-block*, which is like a try-catch around the entire constructor, *including* the initializer list.

https://github.com/CIS1900/2022-fall/blob/5312b53b9930a6616fdb9a81c3733450737a386b/06/ctor1.cpp#L1-L41

This function-try-block can then handle exceptions from the initializer list.
One thing to note is that any catch clauses of a function-try-block *must* rethrow, and will rethrow the exception for you if you don't do it, like in the example above.
This is because if the function-try-block has to handle an exception, it is saying that the function as a whole has somehow failed, so the handler cannot recover from this, it *must* be up to whoever called the function to decide how to handle this failure.

Function-try-blocks can also be used for normal functions, not just constructors, but there is not much utility there---you could just use a try-catch inside the function body instead.
More details about function-try-blocks can be found [here](https://en.cppreference.com/w/cpp/language/function-try-block).

Finally, note that the code above is not very good---it requires us to know that the initialization of `q` is the one that fails, so we `delete` the memory that we know was successfully allocated: `p`.
To handle this in general, we should use RAII, just like everywhere else in C++.
By using a smart pointer here, only the smart pointers that were successfully constructed will be destructed, `delete`ing the allocated memory just as desired, without having to know in advance which allocations will succeed and which will fail.

https://github.com/CIS1900/2022-fall/blob/5312b53b9930a6616fdb9a81c3733450737a386b/06/ctor2.cpp#L1-L37

Now that we can properly exceptions in our constructors, the copy and swap example from earlier can finally be fully safe and leak-free.
A failing copy constructor would not leak memory, and get properly cleaned up by the function-try-block.
