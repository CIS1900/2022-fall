# Class 4

## Classes

The `struct`s that you are familiar with from C are classes in C++.
Whereas C they were special "`struct` types" and had to be used with the `struct keyword` (e.g. `struct point p`), in C++ they are first class types, and do not require a special keyword (e.g. `point p`).

https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/point1.cpp#L1-L22

As an aside, in this code we use the `cmath` header for `sqrt`.
This is just the C `math.h` header, but converted to C++ conventions, with improvements like namespaces and more precise types.
All C headers are provided in in this way, by prepending "c" and removing the file extension.

Creating an object in C++ is similar to initializing a built-in type.
You would only use `new` if you wanted to dynamically allocate an object and obtain a pointer to it, just like with other types.

Within member functions of the class, we have access to the `this` keyword, which is a pointer to the object that the member function was invoked on.
Note that this is a pointer rather than a reference, so using it requires us to do `this->x`, not `this.x`.

### Access Modifiers

C++ has the familiar access modifiers of private (only accessible within the class), protected (only accessible within the class and descendant classes), and public (accessible to all).
Rather than writing the access specifiers for each member of the class like in Java, you can set the accessibility of all *following* members using things like `public:`.
With a `class`, members are private by default and with a `struct`, members are public by default.
This is the *only* difference between `class` and `struct`.

https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/point2.cpp#L1-L27

### Constructors

Like many other languages, C++ has constructors to initialize objects.
A *default constructor* is a constructor with no arguments.
This is the constructor used (by default) when creating an object without any arguments (e.g. `point p`), and is required for some applications like arrays.
In the code above, C++ generates an implicit default constructor for us since we didn't define one, and it is called when we create the object.
Now we can define our own constructor, which also removes the auto-generated default constructor:

https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/point3.cpp#L1-L25

The part after the constructor's parameter list is known as the *initializer list*, and is used to initialize member variables.
Note that initializing a member variable in the initializer list is different from assigning to it in the body of the constructor.
Assigning to it in the constructor first initializes it to whatever initial value it gets, then the body of the constructor is run.
This means that if a member object of a class isn't initialized in the initializer list of the constructor, its default constructor is first called.
This is an issue if, for example, we have a class which contains a member variable of type `point`.
`point` has no default constructor, only a constructor taking two arguments, so it must be initialized in the initializer list: `... : p {0, 0}`.
Similarly, since references must be initialized, they too must also be initialized in the initializer list, not set in the body of the constructor.

Going back to the auto-generated constructors, we can explicitly ask for this using `point() = default;`, which gives us the implicitly generated default constructor that was removed when we defined our own constructor.
Note that this terminology can be confusing.
Default constructor in C++ means a constructor with no arguments, but `default` here means the "default-generated" constructor.
We can also explicitly remove this auto-generated constructor, using `point() = delete;`.
If we do this and remove our other constructor as well, then this would make it so that we cannot create `point` objects, since there are no constructors.

Default values for member variables can also be specified where they are declared, which is used to initialize the variable if it was not already initialized in an initializer list.

https://github.com/CIS1900/2022-fall/blob/026adcb5023fc3e777d43dec48131a8692263f9d/04/point4.cpp#L1-L24

### `const`

`const` variables of built-in types like `int` cannot be modified using `=`, but objects can be modified using member functions.
Disallowing all member functions to be called on `const` objects would be too restrictive, however, so C++ allows us to mark which operations are safe to be called on `const` objects.
This is done by adding `const` to the end of the member function declaration, and the compiler checks that the definition of the function does not mutate the object.
These `const` member functions are the only ones that can be called on `const` objects.

https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/point5.cpp#L1-L25

## I/O for User-Defined Classes

We've seen that streams can perform I/O with a variety of types, printing terms of types like `int`s and `string`s and reading in text and converting it to types like `double`.
These operations depend on the type we are working with, and we can define them for classes we write ourselves as well.
This is the idiomatic way of printing objects in C++, as opposed to solutions like `toString` in Java and other languages.

### Operator Overloading

Calling a member function can be thought of as calling a free function (a regular non-member function) with an extra first argument.
i.e. `a.foo(b)` can be thought of as `foo(a, b)`, where the first argument is a reference to the class.
Following this intuition, overloading an operator for your class can be a member function, as long as the first argument to the operator is an object of your class.

For example, overloading addition for two `point`s can be either a member function inside the `point` class or a free function.
But since the operator has to access private members of your class, it must be defined as a member function here:

https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/point6.cpp#L1-L30

### Friend functions

For stream operators like `>>` and `<<`, the first argument is not a `point`, it is a stream.
So to define it as a member function, it would have to be a member function of the `istream` or `ostream` class, which we do not have access to.
Furthermore, neither this nor defining it as a free function would give us access to the private members of `point`, which would be needed when reading or writing a `point`.

The solution to this is the `friend` keyword, which marks a free function as a "friend" of your class, giving it the same access as a member function.

https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/point7.cpp#L1-L58

## Separate Compilation

While it's possible to write all your code in one file, it is typically not desirable.
For large projects, changing any code would require the entire file to be recompiled, which can take a long time.
Breaking up work and using others' code in the form of libraries would also be very difficult.

The compilation process looks something like this:

1. The preprocessor runs on each source file, expanding `#include`s and `#define`s, among other tasks.
2. The compiler compiles the output of the preprocessor (called a *translation unit*) into `object files`.
3. The linker links multiple object files together, creating the executable.

Headers are necessary so that compilation succeeds even when the code in broken up into multiple files.
Headers typically include declarations that specify the types of functions defined in other translation units.
In the linking phase, the linker finally goes into the different translation units to find the definitions, "linking" everything together into one executable.

Here is the split-up version of the `point` code above, which you can compile by just listing both `.cpp` files together in the usual compilation command:

https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/separate/point.hpp#L1-L24
https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/separate/point.cpp#L1-L37
https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/separate/main.cpp#L1-L14

Notice that we need to prepend `point::` to the function name when defining them in the `.cpp` file.
This is like a namespace, where we are specifying that we mean the function found in the `point` class.

Now we go over some of the reasoning behind these choices.

### One Definition Rule

Just as in C, multiple *declarations* of functions are allowed as long as the declarations are the same.
Multiple definitions are not allowed.
There is an exception to this for `inline` functions, which signals to the compiler that this function could be inlined (that the body of the function could just be substituted in, rather than be compiled into an actual function call).
You are also allowed to put member function definitions in the header file, which will work because C++ will automatically treat that as an `inline` function.
This, however, is not typically good practice and you should avoid it.

With classes, things get a little more complex.
Multiple declarations of classes (e.g. `class point;`, without the body of the class) are still allowed, but there can be only definition per *translation unit*.
However there can be multiple definitions in different translation units, as long as they are the same.
This is because the definition of the class is required for the compiler to determine its size (by looking at its members), which is needed to generate code for things like function arguments.

More details about the one definition rule can be found [here](https://en.wikipedia.org/wiki/One_Definition_Rule).

### Include Guards

Include guards (the preprocessor directives around the entire header file) prevent headers from being included more than once in the *same translation unit*.
As mentioned above, this will violate the one definition rule if the header contains a class definition.

Remember that translation units are compiled separately into object files and only linked at the end, so include guards cannot help with enforcing the one definition rule for the entire program.
Multiple translation units can all include the same header even with include guards, and if that header contains a function *definition*, for example, this would result in a *linker* error since the one definition rule was violated.
You can try this by adding a function definition to the header and compiling it.
Both translation units include the header, and you will get a linker error.
Note that linker errors look different from compiler errors, and will typically result from an executable like `/usr/bin/ld`, the linker.

### Modules

This system of splitting up code into header and implementation files is not ideal, and C++20 introduces a replacement found in many other languages: modules.
Modules allow files to export selected functions and classes, and other files to import them, hiding all the other details of that module, like what modules *it* imports or any unexported helper functions.
Unfortunately [most compilers](https://en.cppreference.com/w/cpp/compiler_support) do not fully support modules yet, so we will not use them in this course.

## Inheritance

To define a class `derived` inheriting from the class `base`, declare it as `class derived : public base ...`.
To call the base class's constructors, you can just call them normally.
To call the base class's member functions, you need to specify the class name (e.g. `base::f()`).
This is the alternative to using something like Java's `super()`.

The entire example is presented here:

https://github.com/CIS1900/2022-fall/blob/b58a4c3baae30e58c198c287190016f8e9dc1e3c/04/inheritance.cpp#L1-L58

We now go over the different portions one by one.

### `virtual`

In the base class, member functions that you want to behave polymorphically must be marked as `virtual`, before its return type.
In a derived class, `virtual` is not necessary in the declaration since the `virtual` marker in the base class remains.
Without `virtual`, if you have a `base` reference and call the non-`virtual` function, even if the reference is to a `derived` object, it will call the `base` version, rather than the `derived` version.
This allows us to customize the exact behavior of our objects, whether we want *dynamic dispatch* or only *static dispatch*.
The function to call is determined at compile-time for non-`virtual` functions, and determined at runtime for `virtual` functions.

This option exists because there is overhead to using `virtual` functions, which are the default in many languages.
Objects must hold what's called a "virtual table" or *vtable*, which has pointers to its member functions.
During runtime, the program must perform a lookup through the vtable to find what function should *really* be called.
Near the beginning of object-oriented programming, many programmers were skeptical about using dynamic dispatch in this way, since they viewed the overhead as completely impractical.

When overriding member functions in derived classes, you can add `override` after the parameter list.
This is not necessary, but it is useful for documentation and can catch bugs (e.g. if you try to override a function but misspelled the function name, the compiler can tell you that you are not actually overriding anything).

You can also add `final` in the class declaration (e.g. `class derived final : public base`) or member function declaration (e.g. `void foo() final {...}`) to disallow inheritance and overriding, respectively.

### Abstract Classes

In C++, an *abstract class* is one with a pure virtual function, which is a `virtual` function declared as `... f(...) = 0;`.
A pure virtual function does not need to have a definition, but it can, if you want to provide some common functionality.
You cannot instantiate objects of abstract classes, since there is conceptually some operation (the pure virtual function) that is not yet implemented.

If a derived class overrides the pure virtual function, then it is not pure virtual anymore in that derived class.
A class without any pure virtual functions is *concrete*, and objects of that class can be instantiated.
Abstract classes allow us to define an *interface* with operations that are forced to be implemented by derived classes if they want to use the class.
