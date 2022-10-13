# Class 5 summary

Questions from the Canvas quiz:
- lvalues vs rvalues:

lvalues are expressions that can appear left of the `=`.
Alternatively, lvalues can be thought of as things that can be converted to regular references.
These are things that have some set address in memory (either stack or heap).
These include things like variables, array elements, or dereferenced pointers.
Of course, references are already references, so they are lvalues as well.
If a function returns a reference, that can be an lvalue as well.

rvalues are expressions that can *only* appear right of the `=`, and can be thought of "temporaries".
They include literals and the return values of functions (including operators!), as long as that return value is not a reference.

Knowing whether something is an lvalue or an rvalue is useful to tell when a move vs copy would occur.
By the way, rvalue references can be used in regular functions as well, and will be called when an rvalue is passed to it.
If a function takes an rvalue reference, then it can only be called by passing an rvalue.
However, if a function takes an lvalue reference, then it can be called with both lvalues and rvalues, since the rvalue, or temporary, can be put somewhere in memory to temporarily act as an lvalue.

[this file](rvalues.cpp) contains the examples I showed in class.
[I made a variant which I think is better](rvaluesbetter.cpp), which might be clearer.

Again, this is a simplified explanation, but should be sufficient for most purposes.
If you want to know the real technical explanation, there is the link to the very long explanation in the notes.
- Move constructor/assignment: This should be thought of mostly as an optimization to copy operations. They get called automatically when the thing we are trying to copy is actually a value (a temporary value). They should behave identically to the copy operations, from the view of the object being copied *to*. For example, `a` should be the same whether `a = b` did a copy assignment or a move assignment.
- Instead of using move constructor, can we force the code to use the copy constructor (with an rvalue): This is an interesting question, and I don't think it's possible, at least not with a simple solution. However, this should not really be something you need. The move constructor should effectively steal the other object's resources such that the newly created object is identical to what it would be if it was copied from that object.
- When to use move assignment: They should just happen automatically when you are dealing with temporaries or returned objects from functions. If you mean manually forcing a move using `std::move`, you can use this if you are certain that you are done with an object, so you are going to take its resources and move it somewhere else. This is quite a low-level optimization, so you should only use it if you really need the performance, or if you really need something moved (e.g. an object that cannot be copied).
- When does copy/move assignment get called when you have `a = b;`:
Only the copy assignment will be called here, if those are both variables.
The move assignment will never be called since the move assignment would be changing `b`, which would be very unexpected here.
If the copy assignment is deleted, then the compiler would give you an error saying there is no available function.
The only way to call the move assignment here is to do `a = move(b);`, which explicitly asks for a move, and changes `b` (depending on how the move assignment is defined). Typically, `b` would not be valid to use again after this.
- Will the implicit destructor free dynamically allocated memory: No, the implicit destructor will call the destructor of all members, but if the dynamic memory is stored in a normal pointer, normal pointers do not do anything when they go out of scope, so `delete` will not be called on it unless you do it yourself. Next class we will see *smart pointers*, which are objects that *do* deallocate their pointer when they are destroyed. In that case you don't need to define your own destructor.
- Order of destruction: This is always opposite of the order of construction, and is not that important unless you have complex dependencies between objects.
- Should you always have a destructor if you have a constructor: no, only if your object needs to clean something up on destruction. For example, the `point` class did not need a destructor, since there was nothing to clean up.
- Syntax of these different constructors: All of the special constructors are still constructors, and the only difference is the arguments to the constructor. A default constructor has no arguments, a copy constructor has an argument of type `const` reference to an object of the same class, and a move constructor has an argument which is an rvalue reference to an object of the same class.
- Rule of 3 vs. rule of 5, does rule of 5 supercede rule of 3:
Firstly, these are guidelines for how to write your classes, not functions or features of C++.
If you define any of the destructor, copy constructor, or copy assignment, the implicit definition rules say that the move constructor and move assignment will not be implicitly defined.
So you can definitely apply the rule of 3 in this case, and only have those 3 special functions.
But you do typically want the rule of 5 instead of the rule of 3.
The rule of 3 is somewhat outdated, it originates from older C++ before move constructors and move assignment were in the language, so there was no such thing as a rule of 5 then.
Nowadays, you should do either rule of 0 or rule of 5.
However, if you do rule of 3 things will work perfectly fine, you will just miss out on the performance benefit of having move operations available.
- Examples of other resources besides memory (e.g. files):

Consider a C file handle: `FILE *`. If you have a class that represents a file, you might use one of these and `fopen` it in the constructor. Then to clean it up automatically when your object is destroyed, you would need to write a destructor that calls `fclose`. But then if you don't define the copy operations, then copying that object would duplicate the `FILE *`, and multiple objects would try to `fclose` the same `FILE *`, which is undefined behavior.
Note that if you do try to do something else, like having copies `fopen` the same file, with a different `FILE *`, things will likely not work very well either, since writes to the same file would overwrite each other. It might make more sense to disable copying entirely with `= delete;` and only allow for moves.
Another example is a lock for multithreading.
You might want to unlock it on destruction to prevent accidentally leaving something locked forever, in which case you would need to do this in the destructor.
You would also likely need to customize copying (e.g. by disabling it).

In general, you do not typically write classes that manage memory too much in the real world, since it's complicated (as shown by these notes), and somewhat rare.
As we'll see later C++ provides many classes that will do resource management for us, like smart pointers and container classes.
However it's a very subtle topic and can lead to many bugs if you do not understand these concepts, which is why I cover it in detail.
- Does passing by reference make a copy: No, only passing by value (where the argument is not a reference or pointer) will make a copy. Alternatively, you can think of passing by reference as making a copy of the *reference*, which won't copy the original object.
- what does `p {new int {other.get()}}` mean: this initializes the pointer `p` with the address of a newly allocated `int`, where that memory has the value `other.get()`.
- Testing that move/copy construction works properly: The easiest is probably to add print statements, and you can also use a debugger.
- Do you need copy constructor if you only plan on returning by reference (the question said pass by object but I'm assuming this is what they meant): technically no, since copies are not performed, but I would urge you to then disable your copy constructor using `= delete`, since if you do mess up and accidentally use the (buggy, I'm assuming) copy constructor, then things can go wrong.
- Copy constructors with inheritance: copying a derived class to a base class is easy. e.g. if we have an inheritance hierarchy `Base` and `Derived, `Base b {d}` would call the copy constructor of `Base` on an object of type `Derived`, which is fine since a `Derived` is a `Base`. This would copy the `Derived` into `b`, as if `d` were a `Base`.
In the other direction, if you try to do `Derived d {b}`, this would not call a copy constructor since a `Base` is not a `Derived`.
If you want to do this, you could define your own constructor which takes an argument of type `Base` (or `Base &`, or with const, etc.).
This would not be a copy constructor, but a "regular" one, since its type does not match.
- Copy elision: this is just a way of eliding, or removing, multiple unnecessary copies. The details are not important but I just wanted to emphasize that these things can be optimized by the compiler, and that you should not rely on e.g. counting the number of copies.
- Friend functions: Friend functions just mark a free function as a friend of your class, allowing the function access to private and protected members of your class. Full details if you are interested are [here](https://en.cppreference.com/w/cpp/language/friend).
- Assignment operator vs. constructor: the constructor is used to create an object from nothing, and is implicitly called when a copy/move is made when passing arguments to functions or returning objects from a function. The assignment operator is when you already have an initialized object and set it to be equal to another object: `a = b;`.
- `=` vs `{}`: for initialization, where you are declaring the object and immediately initialize it, these are the same. They both invoke a constructor, and *not* `operator=`. For assignment, only `=` works. `{}` would not work since that is only used for initialization.
Even though there is an equals sign in `integer a = b;` this is initialization, and calls the constructor, not `operator=`.

We then did the exercises together on the projector since the wifi went out.
The exercise 1 solutions are [here](list1soln.cpp).
An alternative way to define the assignment operator is to recursively call it in the case when both `next` and `other.next` are non-null.
This gets rid of the reallocation and instead, reuses the existing nodes.
It would look something like `if (next && other.next) *next = *(other.next);`.

The exercise 2 solutions are [here](list2soln.cpp).
