# Class 5

If all of a class's members are regular variables, then things are easy.
Passing objects of that class around works fine, and copying the object just copies each of its members.

But when a class holds a *resource* that must be manually managed, like dynamically allocated memory, things get more complex.
For example, using objects of the following class would result in a memory leak, since the memory allocated using `new` is never deallocated with `delete`.
In these notes, we go over five special functions that C++ uses to manipulate all objects.

INSERT LINK eg0

Other examples of resources include things like file handles, locks, or remote web connections.
These examples all need to be "cleaned up" once used, which also makes *copying*, a core concept in C++, more complex.

### Aside: implicit conversions

First I want to introduce a feature of constructors.
Constructors define an implicit conversion from its argument types.
For example, the `integer` class above has a constructor which takes an argument of type `int`.
This results in an implicit conversion from `int` to `integer` being created, and if you had a function that takes an `integer`, you could pass it an `int` instead, and an `integer` would be created using the constructor with the provided `int`.
If `integer` had another constructor that took *two* `int` arguments, this also creates an implicit conversion, but it must be used in a slightly unnatural way, for example by passing `{1, 2}`.

These implicit conversions can be confusing.
For example, if you had a class of lists and a constructor which takes the initial length of the list, then the user may mistake that for the contents of a list of length 1, and get the wrong result.
You can disable the conversion by putting `explicit` before the constructor (e.g. `explicit integer(int i) ...`).

## Destructors

To handle cleanup, C++ has destructors, which are like the inverse of a constructor.
These are written like a constructor, but with a `~` in front and has no arguments.
While constructors were present in other languages before C++, destructors were a novel feature introduced by C++.

INSERT LINK eg1

The destructor will be called automatically when the object is cleaned up, either when it goes out of scope for a regular variable or when `delete` is called on it for objects dynamically allocated using `new`.
You should never have to manually call the destructor.

If there is the *slightest* possibility of subclasses for your class, your destructor should be `virtual`.
If it is not, then if you dynamically allocated an object of a subclass, and then call `delete` on a base class pointer to that object, then anything extra in the subclass destructor will not be called, leading to the object not being properly cleaned up.

### Order of Construction and Destruction

In C++, variables and objects are always destroyed in reverse order of being created.
This is the case for both regular variables declared in a function and member variables as part of an object.
This is because ordering often implies dependency, and deallocating objects that depend on each other can go badly if ordered wrong.
For example, consider the following declarations:
```c++
A a;
B b {a};
```
`b` seems to somehow *depend* on `a`, and might internally keep a reference to `a`.
If `a` were destroyed before `b`, things can go wrong when times comes to destroy `b`, since its reference to `a` is no longer valid.
But C++ will always destroy `b` before `a`, since `b` was declared after `a`.
While this is not foolproof and programmers can always cause more issues, this solves many simple use cases.

With a class, the destructor also destroys the object's member variables in reverse order, but what is the original order?
We know that member variables are initialized in the constructor.
Constructors first initialize their base class, either using one of the base class's constructors if it was in the initializer list (e.g. `derived() : base{...} {...}`), or the default constructor for the base class otherwise.
Next it initializes class members in *declaration* order, using the initializer list if the member is found in it.
Finally, the constructor body is called.

Note that the order of member variables in the initializer list *doesn't matter*.
This is because there can be multiple constructors with multiple orderings, but only one destructor, so it always picks the consistent order: the order of declarations.

If you try to initialize members out of order like in the following code, then you will get a compiler warning, and things will probably not work as you expect (try running the code!):

INSERT LINK order

Now back to the destructor.
It should run in the opposite order as construction, so it first runs the destructor body, then destroys the members variables in reverse order of declaration, and finally calling the destructors of base classes.
Note that if you have member variables which are objects, the destructor destroys them and cleans them up, meaning their destructors are called automatically.
Again, you never have to call destructors manually.

## Copy Construction

Copies are made fairly often in C++, and if the object being copied has some resource it manages, then copying can be non-trivial.
The default copy operation just copies each member variable, a *shallow copy*.
For our example, this would cause issues.
When a copy is made, that copy has the same pointer as the original object, and when the copy is cleaned up and its destructor is called, the original object's internal memory is deallocated.
When the original object is destroyed eventually, its destructor tries to `delete` the same pointer, and a double free error occurs.
To fix this, the copy needs to be a *deep copies*, where the value pointed to by the pointer is copied, rather than the pointer itself.
To write this deep copy code and generally to customize how objects are copied, C++ uses a *copy constructor*.

First, we need to understand when copies occur.
Copies are made when:
1. Initialization with another object: `integer i {j}`, `integer i(j)`, `integer i = j`.
2. Passing an object (by value) to a function.
3. Returning an object (by value) from a function.
4. Throwing an object as an exception (more on this in a later lecture).
5. Assigning an object to another object (more on this in the next section).

Here is an example of (1), where you can see we are calling our copy constructor:

INSERT LINK eg2

Try commenting out the copy constructor to see the default shallow copy in action, causing a crash.

### Copy Elision

To illustrate the other common ways objects are copied (2 and 3), we introduce a new function to our example:

INSERT LINK eg3

Looking at the rules for when copies are made above, you might think that this code would perform three copies.
One when `i` is passed to `incr`, one when `i` is returned from `incr`, and one when `j` is constructed from this returned object.
However, try running this code.
You will see that it only calls the copy constructor twice.
This is due to a mechanism called *copy elision*, which can omit some copies in certain cases.
In this case, it can tell that the returned value is immediately used to copy construct another object, so the two copy constructions can be "merged".
The specifics are not super important, but it is important that you keep in mind that these sort of things can happen, and that you do not overly rely on side effects from your copy constructor.

## Copy Assignment

The first four ways that copies can occur all use the copy constructor, but the last has to do with assignment, which is when we use the `=` operator directly.

INSERT LINK eg4

Note that `operator=` returns a reference to the assigned object.
This is to allow things like `i = j = k;`, which would be equivalent to `j = k; i = k;` since `=` is right associative.
A less confusing use of this is something like `if (i = j) ...`, which performs the assignment and then checks the value of `i`.

Another note: when you initialize an object using `=` like `integer j = integer(10);`, this is just a regular copy construction, and does not use the assignment operator at all.
This is another reason why I recommend always using the `{}` initialization syntax, to avoid confusion in which function is being called.

One special case to keep in mind is *self assignment*, when a user does something like `i = i;`.
If we had implemented `operator=` as something like `delete p; p = new int {i.get()};`, this would fail for self assignment!
You can always check for self assignment manually, using something like `if (this == &other)`
Self assignment may seem silly, but in complex code with references or pointers that may alias others, this could happen accidentally.

## Move Construction

Sometimes, copies are made of objects that are about to be cleaned up, for example when returning a local object from a function.
In these cases, C++ provides a useful mechanism to avoid the unnecessary copy by simply *moving* the object that is about to be cleaned up into the new object being created.
This is done using a constructor called a move constructor.

INSERT LINK eg5

Our move constructor "steals" the pointer from the other object and crucially, resets its pointer to `nullptr`. This is to prevent it from `delete`ing the memory when its destructor runs. `delete nullptr` is safe, and does not do anything.

Note that this is doing the same thing as [`eg3.cpp`](eg3.cpp) above, but with a move constructor defined.
This allows the program to eliminate one copy construction, which is replaced with a move.
Objects could contain large amounts of memory, which would have to be allocated and copied over, and this overhead is eliminated with the move.

Move constructors also allow for programmers to enforce that their objects can only be moved, not copied, by disabling the copy constructor using `= delete`.
This makes sense for single-ownership objects, like an `ifstream`.
It may not make sense for you be able to duplicate an object that represents an open file, but why not move it?

### rvalue References

The `&&` denotes what's known as an *rvalue reference*.
To understand this we first need to talk about lvalues and rvalues.
*lvalues` are expressions that can appear on the left side of an assignment.
These include things like variables and references, which can be assigned to.
lvalues can also appear on the right side of an assignment without issue.
rvalues, on the other hand, can *only* appear on the right side of an assignment, and they can be thought of as "temporaries".
For example, the value `2` cannot appear on the left side (e.g. `2 = x` would fail), so it is an rvalue.
The product of two `int`s, `a * b` would also be an rvalue, since it cannot be assigned to.

An rvalue *can* work with a regular reference, also called an lvalue reference, but it prefers an rvalue reference.
So when a copy is made with a temporary `integer`, if there is a move constructor, it will invoke that, since the temporary is an rvalue and the move constructor takes an rvalue reference argument.
If there is no move constructor, then it can still fall back to the copy constructor, which takes an lvalue reference.

This was a simplified explanation of lvalues and rvalues.
In reality, values are categorized into more types, and the differences are rather technical.
If you are really interested, [this in-depth explanation](https://stackoverflow.com/a/11540204/3776956) is "only 10 pages on my monitor", according to the author.

## Move Assignment

Just like with the copy assignment earlier, we need to handle assignment separately from construction.
The move assignment provides an optimization when we assign to an `integer` using a temporary.

INSERT LINK eg6

Self assignment is not possible with move assignment.

### Moving lvalues

Moves normally only happen when dealing with rvalues.
But C++ provides a way to move lvalues as well, with `std::move`.
`move(e)` effectively "casts" an lvalue `e` into an rvalue, forcing a move.
The move will change `e`, however, so you must be careful to not use it any more after the move.

INSERT LINK eg7

This code moves `i` to avoid the copy, even though it's an lvalue.
It would not be valid to use `i` after the move, since its memory has been "stolen" and it is no longer in a usable state.
Using `move` in this way can be useful if you know for a fact that you will not need a variable later, and it can be "given" to someone else for its last use.

Using `move`, self assignment *is* possible with move assignment, as `i = move(i)`.
However, the C++ standard says you can assume this sort of thing is not performed, and the user is not allowed to do it.
But checking for this case is very cheap, so you could add the self assignment check to your move assignment if you so desire.

## Implicitly-Defined Member Functions

Last week we saw that a default constructor is implicitly defined for you if you do not defineyour own constructor.
There are a few other definitions that may be implicitly defined for you:
- The destructor is always implicitly defined if you don't define it yourself.
- The copy constructor and copy assignment operator are implicitly defined if neither the move constructor nor the move assignment operator are user-defined.
This just copies each member using its copy constructor or copy assignment.
- The move constructor and move assignment operator are implicitly defined if the user didn't define any of the other 3 special member functions.
This just moves each member using its move constructor or move assignment.

### Rule of 0/3/5

Based on these implicit definition rules, there are some general "rules" (more like guidelines) which are well-known in C++.

The first is the rule of 0: if your class does not manage any resources and you don't need to define any of the special member functions, don't!
This means that the implicitly defined functions are perfect for your class.
The destructor doesn't need to do anything special, and copying and moving just copies and moves all the member variables.
By letting the compiler do all the work, you also eliminate the chance of you making a mistake in defining the special functions.

Next is the rule of 3: if you define one or more of the destructor, copy constructor, and copy assignment, you probably need all three.
This is because the presence of one indicates that you probably have something in your class that behaves like a resource, and needs to be managed manually.
If something needs special treatment when being copied, it probably also needs special treatment to be cleaned up, and vice versa.

While by following the rule of 3 our objects will work perfectly fine, this will also disable the move constructor and move assignment operator, from the rules above.
This means we lose out on some performance, and our code will perform more copies than needed.
This leads us to the rule of 5: if you defined one or more of the five special member functions, you probably need all five.
The justification is the same as the rule of 3, but adds the move operations for a performance boost.

## Resources and Inheritance

When you have a class hierarchy, you will have to call the copy/move constructor/`operator=` manually in your special functions.
The base constructor should go into the initializer list, as usual, and the `operator=` can be called as `Base::operator=(...)`.
The implicitly-defined functions will do this for you.
For *all* constructors, if the initializer list does not contain any base class constructor, it will automatically call the default constructor of the base class.
Finally, as noted above, destructors will always call the base class destructor.
