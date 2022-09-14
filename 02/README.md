# Class 2

In this file, we will introduce C++, some of its many small improvements on C, and importantly, a new concept called *references*.
Please read the last section in the [class 1 notes](../01/README.md) on debugging as well, since we didn't get to it in class 1.
These notes are shorter than usual since we did not get through all the material in the first class.

## Hello World

We again start with a standard hello world program:

https://github.com/CIS1900/2022-fall/blob/61d0d122f5947dd1522fa81fc34e929f3d97d241/02/hello.cpp#L1-L8

For C++ programs, we compile using `g++ -std=c++20 -Wall -g -o hello hello.cpp`.
Depending on how you installed GCC from the [instructions](../gcc.md), you may need to use `g++-10` instead of `g++`.
This uses GCC's C++ compiler rather than the C compiler.
The only new flag compared to how we compile C programs is `-std=c++20`, which enables C++20 features.

This is quite a bit to type every time you want to compile, so I put the following into my `.bash_aliases` file to run this command on terminal startup: `alias g++='g++-10 -Wall -std=c++20 -g'`.

The first line of the program is `#include <iostream>`.
`iostream` is a header file that contains declarations for various IO *stream* objects, like  the `std::cout` object below, which we use to print.
In C++, headers for standard libraries do not have any file extension, like `.h` in C.
For C++ code you write yourself, the typical convention is to use `.cpp` and `.hpp` for for code and headers respectively.
You may also see `.cc` and `.hh`, though this is less common.

The `main` function is the same as in C, so that just leaves the `std::cout` line.
This line is used to print the hello world string using a special *object*, `std::cout`.
The `std` portion is the `namespace`, which is used to avoid name clashes.
The standard library uses this namespace for all of its objects and functions, and the `::` operator is how we access names within a namespace.
`cout` stands for *character out*, and is used to print to `stdout`.

Finally, `<<` is the syntax used to print using objects like `cout`.
It can be pronounced or thought of as "put to".
C++ supports *operator overloading*, so we can make operations like `+` behave in user-defined ways.
Only existing operators are supported, so the designers of the standard library used the left bit shift operator here.
As we'll see in more detail in the next class, these `<<`s can be chained to print multiple things, like the following:
```cpp
cout << "Hello," << " world!\n";
```

## Small Improvements

The following are some small and relatively simple improvements that C++ provides:

### Namespaces

You can define namespaces fairly easily, with
```cpp
namespace space
{
    ...
}
```

Anything within the namespace needs to be referred to as `space::...`.
If you are using something from a namespace often, you can use `using` to be able to refer to it without the namespace.
For example, if you write `using std::cout` in a function, then in that function you can just write `cout` rather than `std::cout`.
This effect lasts for the current scope, and can also be used at the file or block level.

If you wish to drop the namespace for everything in the namespace, you can use `using namespace`.
For example, if you write `using namespace std`, then you can refer to everything in `std` without the `std::` prefix.
This is generally discouraged in larger projects since it brings a lot of names into the current context.

Both uses of `using` should be avoided in header files, since that would require anyone `#include`-ing the header to be forced to bring many new names into their context.

### Function Overloading

Unlike in C, function overloading, or multiple functions of the same name but different types, is allowed in C++.
The compiler will only accept overloads that it can differentiate using the types of the overloaded function.
For example, the following code will compile:
```cpp
int triple(int n)
{
    return 3 * n;
}

double triple(double n)
{
    return 3 * n;
}
```

But adding an overload with declaration `int triple(double n)` will not compile.
If someone calls `triple(1.1)`, the compiler has no way of knowing whether you want the version that returns a `double` or the version that returns an `int`.

### `<=>`

C++ has another comparison operator in addition to the standard ones like `>` and `==`.
`<=>` is the *three-way comparison* operator, or sometimes called the "spaceship" operator.
`a <=> b` `< 0` when `a < b`, `== 0` when `a == b`, and `> 0` when `a > b`.

### `bool`

Unlike in C, C++ has a real Boolean type of `bool`.
The values of `bool` are `true` and `false`.

### `if` and `switch` with Initialization

Like how `for` loops have an initialization portion, in C++ `if` and `switch` statements can have one as well.
The syntax looks like the following, and behaves just like initialization does for `for` loops:
```cpp
if (int i = ...; ...)
...

switch (char c = ...; c)
...
```

This is very useful when you want to check the output of some function, but don't want to keep the variable storing the output in scope after the check.
For example, without using the initialization step, your code would look like this:
```cpp
int i = f();
if (i == 10)
{
    ...
}
// i is still in scope!
```
And `i` remains in scope, which may "pollute" your context with useless variables.

### Initializing Variables

C++ has a new method of initializing variables.
While in C you would write `int i = 10;` to declare and initialize `i`, in C++ you can use `int i {10};` instead.
This is not quite the same as using `=`.
`{}` can help detect unwanted type conversions: `int i = 1.1;` works, implicitly converting the `double` into an `int`, but `int i {1.1};` will not compile, potentially alerting you to a bug.

In general you should use `{}` for initializing variables over `=` to avoid these implicit type conversions.
Later on in the course, we will see that using `=` to initialize variables and using `=` to assign to existing variables (`int i = 10` vs `i = 10`) are actually very different operations when it comes to objects.
By avoiding initializing using `=`, you can also avoid mixing up the two.

### Type deduction

C++ also introduces `auto` to use in place of the type when declaring and initializing variables.
For example, `auto i {10};` will automatically figure out that `i` should be an `int`.
This comes in handy when using functions that return values of complex or long-to-write types, as we will see later on.

### For-each loops

C++ also introduces a fourth type of loop: a for-each loop.
It is written as the following:
```cpp
for (auto i : {1, 2, 3, 4})
	...;
```
This loops through the array `{1, 2, 3, 4}`, where at each iteration `i` equals the successive values in the array.
Changing `i` does not change the array you are iterating over, as `i` contains a copy of each value from the array.

Later on, when we introduce other container types, you can also use this for-each loop with those objects, not just arrays.

### `nullptr`

Instead of `NULL`, C++ introduces a new way to represent null pointers, called `nullptr`.
You would use this just as you do with `NULL`, but `nullptr` eliminates ambiguity between `int`s and pointers.
For example, since `NULL` is usually defined as `0` directly using `#define`, we can use it in arithmetic like `1 + NULL`.
`nullptr` has a pointer type, so the compiler would produce an error for `1 + nullptr`.
You should not use `NULL` in C++, only `nullptr`.

### Dynamic Memory Allocation

Rather than using `malloc` (or variants) and `free`, in C++ we use `new` and `delete`.
These operations are easier to use than the C versions, and handle objects properly, whereas `malloc` would just give you a chunk of memory, without initializing the object that is supposed to live in that memory.

There are two variants of each.
The first variant consists of `new T` which allocates enough memory for a single value of type `T` and returns the pointer to that memory, and `delete p` which deallocates the memory that `p` points to.
Unlike `malloc` and `free`, you don't need to calculate the number of bytes you need using `sizeof`, and `new` and `delete` will also construct and clean up objects properly.
The second variant is `new T[n]`, which allocates memory for an array of `n` values of type `T`, and `delete[] p` which deallocates the entire array created.

These variants must not be mixed, so memory allocated using `new` must be deallocated using `delete`, and memory allocated using `new T[]` must be deallocated using `delete[]`.

https://github.com/CIS1900/2022-fall/blob/61d0d122f5947dd1522fa81fc34e929f3d97d241/02/newdelete.cpp#L1-L10

## References

References are an alternative to pointers for passing by reference.
A `&` after the type denotes that it is a reference to that type.
Note that `*` and `&` are part of the type, so it is typically written as `int&` or `int &`.
A reference is effectively a pointer that gets automatically dereferenced, and that cannot be changed to reference something else (like a `const` pointer).
For example, we can redo the pass by reference example from last class:

https://github.com/CIS1900/2022-fall/blob/61d0d122f5947dd1522fa81fc34e929f3d97d241/02/remainder.cpp#L1-L20

We can avoid some pointer bugs by using references, like null pointer bugs.
This is because references *must* be initialized, so there is no "null" reference.
Code like `int & i;` will not compile.
In general, try avoid using pointers in C++.
In a few classes, we will see ways to avoid using "raw" pointers entirely in C++, avoiding many common pointer bugs that occur in C.

For passing arguments, even when you don't want to modify the caller's argument variable, using a reference as the argument type can still be useful.
By taking a `const` reference as the argument, we avoid the cost of copying (expensive in the case of large objects or arrays) but are still able to use the argument inside the function.
This was also a common use of pointers in C.

References can also be used in conjunction with the for-each loop introduced earlier, to change elements of an array:
```cpp
for (auto & i : {1, 2, 3, 4})
    i = 0;
```
