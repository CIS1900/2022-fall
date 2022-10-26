# Class 7 summary

Questions from Canvas:
- Smart pointers and RAII review:

RAII: attach resources to object lifetimes.
This means that we can use stack variables, which will be destroyed automatically, to manage resources like memory which we used to have to do on our own.
For example, `string` is an RAII class since it manages its internal memory properly using its constructor and destructor, and doesn't require you to call `.free()` on it or something to clean it up properly.

Smart pointers are classes that use RAII to manage pointers.
They are a bit more complicated than things like `string` because they can work with any pointer.
The key notion for pointers is to keep track of ownership, or "responsibility".
The owner of a pointer is who is responsible for `delete`ing the pointer, and pretty much all bugs are caused by sharing the pointer so that ownership is inconsistent or forgotten.

`unique_ptr` forces ownership to not be shared (by disabling copy) and forcing ownership to be transferred to exactly one destination (by enabling move).
This models the case when a pointer has exactly one owner, so when the pointer should be `delete`d is easy to determine.

When there are multiple owners all sharing one pointer, `shared_ptr` can model this case.
It keeps track of all the owners of a pointer using the reference count, and will not `delete` the pointer until all owners are done with the pointer.

A `weak_ptr` models temporary ownership, where there is some `shared_ptr` out there that the `weak_ptr` wants to access occasionally, without becoming a "full-time" owner.
So when the `weak_ptr` wants access, it can ask if the `shared_ptr` is still there, and if so, it will grab a temporary `shared_ptr` object to it.
- How should we ensure classes work properly with regard to resources and smart pointers, etc.: The easiest way: try to avoid manually managing resources in your classes whenever possible!
Use classes that manage resources for you, e.g. smart pointers or `string` from the standard library.
Try to strive for the rule of 0 whenever you can, because then you cannot have any leaks or sharing errors.
If you do have to write special operations, do a lot of testing! e.g. the test suite from the BST assignments.
- Obeying rule of 3/5 with smart pointer members: Remember that the rule of 3/5 is not a requirement, but more of a guideline.
If you are writing one of the special functions, *think hard* about whether you need all 3 or all 5.
With smart pointers, we don't need to write the destructor, but we do need to write the copy operations if you want a deep copy.
- `#include` in a header file: This is fine, there is no rule against having `#include`s in headers.
You want to avoid having `using` in your headers since those will force every user of the header to use the `using` as well.
You may want to minimize the amount of headers you include since then there will be more dependencies between your files, and you will have to recompile more files.
- Different ways of initializing an object:

The simplest way is to just call the default constructor: `foo o` or `foo o {}` or `foo o()`.
All of these construct an object using `foo`'s default constructor.
For a normal constructor with arguments, you can use `foo o {12}` or `foo o(12)`.
I prefer curly braces just so it's consistent with the other places we use it for initialization.

For copy construction, you can use `foo o1 {o2}`, or `foo o1 (o2)`, or `foo o1 = o2`.
All of these call the copy constructor, even when there's an equals.
Here you can also use auto instead of `foo` since the compiler sees that you are trying to make a copy, so it knows `o1` should be a `foo`.
Again, I prefer curly braces here to keep it consistent, and to make sure we remember that this is construction, not copy assignment.

You can also just call the constructor on its own as `foo()` or `foo{}`, to create a temporary object, like `f(foo())`.
This creates a brand new `foo` from the default constructor and passes it to `f`.
Using this, you could do things like `auto o {foo()}` or `foo o = foo{}`, which makes a temporary new object, then uses the copy constructor to initialize `o` with the just-created temporary.
The copy is unneeded here, so you should be initializing `o` directly instead.
- difference between defining both assignment operators vs the one that takes an object by value: Defining both assignment operators separately allows you to do different things on a copy assignment vs. a move assignment.
Defining it in one by taking the argument by value allows you to reuse code, but requires the implementation be the same for copy and move, necessitating something like copy/move and swap.
- Pass by reference & returning by value/reference/pointer:

You would pass by reference if you want the original variable passed in to be changed in the function.
You could also pass by const reference to avoid a copy if you only need to call `const` member functions on the argument.

You typically want to return by value, which typically performs a move, since the returned thing is about to be destroyed anyways.
You might want to return by reference if there was some reference passed in and you want the user to be able to refer to it, like in `operator<<` or `operator=`.
You should only return by pointer (and prefer using smart pointers!) when you have to, for example if you want to return an array.
- Headers and include guards:
Suppose you have a function `f()` that you wish to use in two translation units. If you define `f` in a header and include the header in both translation units, then include guards don't save you, since functions can only be defined once.
However if you define f as a `static` function (meaning the function is "private" to the translation unit) in a header, then each translation unit including it gets its own copy, and you can compile fine.
But if you didn't have an include guard, then if you include that header twice in the same translation unit then the translation unit gets two definitions of `f`, which results in a compiler error.
The include guard helps you with this case.
The case with classes is similar.
You cannot define a class twice in the same translation unit, but you can define them once in in each translation unit.

- use cases for `any`: I can't think of too many, and I would avoid this unless you have to use it. One example I thought of was if you had a library for parsing things that you were still working on. So maybe you can represent `int`s, `string`s, `double`s, and so on, but you are still working on adding support for more types.
If you had to give your data representation to some user code for storage or to have them pass it around, you could give it to them as an `any`, since they don't really need to "unpack" this variable, and eventually it is expected to be passed back to your library so you can unpack it yourself, since you know what should be inside the `any`.
The benefit of using `any` here is that you don't have to constantly change the interface as you add support for more types.
Eventually, if you are done adding support, you might want to change this to a `variant<int, string, double, ...>` which contains all the types you support.

Other questions that I didn't get to:
- `#pragma`: These are compiler specific, so I've never used them and can't really answer this. See your compiler's documentation for details, but for this class we'll focus on portable and standard C++.
- Circular shared pointers: `shared_ptr`s implement what's known as reference counting, where the number of pointers or references to a piece of memory is kept track of.
When there is a cycle in the pointer structure, the reference count never goes down to 0 and the memory never gets properly `delete`d.
e.g. in main we create objects of type `foo` referred to with `shared_ptr`s.
The pointers remember that the reference count to both objects are now 1.
Those objects hold `shared_ptr`s themselves to another `foo`, and let's make both of those objects point to each other.
Now the reference count to both objects is 2.
If we're done with those original `shared_ptr`s in main, the reference counts go down to 1, but that means those objects are now inaccessible, and the count is still not 0, so they are never `delete`d.
- pointers to pointers: There were main uses for pointers to pointers in C.
The first is if you have a pointer to something in a variable, and you want a function to change what that pointer pointed to inside the function. This can be done using references in C++.
The second is to represent 2D (or even more dimensions) data, and the main reason I can think of to use pointers to pointers in C++. See the project starter code for an example of this.
- Default arguments in header vs implementation file: In the header, you would write something like `void f(int i = 10);`, and in the `.cpp`, you would define it as normal, `void f(int i) {...}`.
- how `any_cast` works: `any_cast<T>(a)` casts from an `any`, which is `a`, into a value of type `T`. This will only work if the original value put into the `any` was a `T` in the first place.
- "If I return a variant<int, double>, then it has to remain that way for the entirety of the program, right? In other words, I can't make some assumption and try to assign a double to the variant<int, double> without some sort of compilation error?": A `variant` is a normal object that can hold one of several types. If you have a `variant<int, double> v`, you can assign either `int`s or `double`s to it: `v = 1;` or `v = 1.5;`. Once you do that, then the `variant` will keep track of what is actually stored inside and use that information for when you want to grab the value from inside the `variant` (using `.get<int>()` or `.get<double>()`).
- `release()` vs `reset()` for `unique_ptr`: `release()` returns the underlying pointer and gives up on managing it, meaning the `unique_ptr` no longer owns any pointer, and you're free to do whatever you want with the returned (raw) pointer. `reset()` will `delete` that pointer for you, clearing the underlying pointer and cleaning it up.
- `value()` vs `*` for `optional`: `*` is a bit faster but you should only use it if you are certain that the optional holds a value (which is easy to check). You should use `value()` if you are writing code with a lot of exception handlers already, or if you don't care so much about the small performance hit. There is not too much of a difference.
- Avoiding memory leaks: Use RAII classes whenever possible, and check that there are no cycles in your `shared_ptr`s. If for some reason you can't do this, you should do the same thing you would in C (since you would be coding in a C-like style): think very hard and write a lot of tests.
- Disambiguate between function overloads with default arguments in the mix: e.g. `void f(int i = 0) {...}`. Defining a function with default arguments can be thought of as defining two overloads of the same function: one without the argument with a default value, and one with the argument. So if you had that function, it's like defining two overloads of `f`: `void f(int)` and `void f()`. So if you had that definition plus another one which looks like `void f() {...}`, then there is an ambuity issue since there are two functions with the same name and type.
- `enum class`es: These are basically just improved `enum`s. We haven't really dont anything that warranted using an `enum` or `enum class` in class, but for example maybe you want to have a few difficulty settings for your minesweeper game. You could make an `enum class` which contains the different difficulty levels like `EASY, INTERMEDIATE, HARD`. These are more readable than using something like an `int`, and force the types to be checked properly when you use them as arguments.
- `virtual` functions: regular functions, when compiled, are fairly simple. During compilation the compiler looks at function calls, and looks up where that function should be in the program, and replaces the function call with the address of the code for that function. When it comes to polymorphism and dynamic dispatch, that all changes. Now, the member function call does not know where to go at compile time, the program must do a lookup for what type the pointer or reference "actually" is, and finds the correct function to call. What `virtual` does is enable this process. It is given to you as an option because this has costs compared to a normal function call: you must use memory to store information about the virtual functions (the vtable), and you need extra instructions to do the lookup and find the function to jump to.
- lvalues, rvalues, and rvalue references: in short, lvalues are "variables" and rvalues are "temporaries". You may want to review the previous summaries and notes for this. rvalue references are something you can attach to a function parameter to tell C++ that your function will only accept rvalues passed in for that argument.
That is, that argument will only work for temporary values or objects, because you're going to do something special to that argument in the body of the function, like stealing its pointers or other resources.
- Multiple inheritance where both parent classes have the same member function:
If you don't override the function then the compiler will tell you there is an ambiguous function.
You could just override the member function in the derived class, and customize it.
You could even call the parent versions like `parent1::f()` and `parent2::f()`.
If you are dealing with the diamond problem, you could use virtual inheritance: https://stackoverflow.com/questions/254673/multiple-inheritance-from-two-derived-classes?rq=1

For exercise 1, the solution are: [header](studentsoln.hpp) and [.cpp](studentsoln.cpp).
The main thing is that the class uses a `unique_ptr` to represent the grade, allowing the grade to be unspecified and later providing an actual `double`.
This adds a bunch of extra complexity, like having to allocate memory using `make_unique` and having to write copy operations manually since unique pointers cannot be copied normally.
By switching to an `optional`, we get the same behavior but can avoid much of the complexity.
Another small change would be to call `setGrade` so we don't have to duplicate the code which bounds the grade between 0 and 100.
In the header, we need to add an include guard.
I left the `= default` for the move operations, though it is not necessary.
Without them the class would still have implicitly-generated move operations, since it doesn't have any custom copy operations (see implicit generation rules from before).
