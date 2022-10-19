# Class 7

For this week we will cover a few small topics, since the project will be released this week.

## Default arguments

Like many other languages, C++ supports *default arguments* for when you don't pass some of the arguments to a function.
This is written as `= ...` in the parameter list after the parameter name:

https://github.com/CIS1900/2022-fall/blob/9b90250a15f59307aa22902260e5f7e86a00b7e1/07/default.cpp#L1-L22

Pretty simple, though there are a few things to keep in mind.
First, all default arguments must be at the end of the parameter list.
If you had some in the front, and ones after without a default value, like `void f(int i = 1, int j = 2, int k)`, the compiler would not be able to figure out which you mean by `f(3, 4)`.
Unlike some other languages, we cannot specify which argument we mean by name (like `f(i = 3, k = 4)`), they must go in order.

Second, when there are both function declarations (e.g. in a header) and function definitions (e.g. in the `.cpp`), the default argument could go in either (not both, that results in a compiler error).
You should always put your default arguments in the declaration (i.e. the header), since then any users of your function can tell that it has default arguments by looking at the header, without having to look at the implementation of your function.

## `enum class`es

In C, you may have used [`enum`](https://en.cppreference.com/w/c/language/enum)s to represent named constants.
While these work, they effectively just define names for `int` (or another underlying type) constants, and these names can be used in place of `int`s and vice versa.
This means that if you have a `color` `enum`, you could do things like `RED - BLUE` or pass numbers to functions that expect a `color`, both of which might be undesirable.

An alternative that C++ provides is an [`enum class`](https://en.cppreference.com/w/cpp/language/enum), which are similar but are more strongly typed.
Values of `enum class`es do not have implicit conversions to `int` or vice versa, though you can expicitly convert between the two.
In addition to this, the constants that are part of the enumeration are placed in the scope of the enumeration, so they must be accessed using `::`:

https://github.com/CIS1900/2022-fall/blob/9b90250a15f59307aa22902260e5f7e86a00b7e1/07/enum.cpp#L1-L14

There are some other features of `enum class`es, like using an underlying type aside from `int` and defining more operations on values, but we won't go further into those features.
In general, you should prefer `enum class` to `enum` in C++, since they are more restrictive and are thus less likely to cause bugs from unexpected behavior.

## Union types

There are some common patterns in C that are dangerous or suboptimal.
Here, we'll present some safer solutions in C++ for these use cases.

### [`optional`](https://en.cppreference.com/w/cpp/utility/optional)

A common pattern in C to signify that an operation has failed to produce a value is to return a pointer, where null signifies failure and a non-null pointer to a value signifies success.
This works, and we could make it safer by using a `unique_ptr`, but it still has drawbacks since it uses dynamic memory.
In general, you should avoid heap allocations if possible, since they require manual memory management (solved using the smart pointer) and have some overhead in the form of pointer dereferencing.
Every time you want to use an object through a pointer, you must dereference that pointer, which is an extra instruction to run in the program.

C++ provides an alternative: the `optional<T>` type (in the `<optional>` header).
The type `T` in the angle brackets signifies what type it should contain.
Values of this type holds either a value of type `T` or an "empty" value, which we write as either `{}` or `nullopt`.

https://github.com/CIS1900/2022-fall/blob/9b90250a15f59307aa22902260e5f7e86a00b7e1/07/optional.cpp#L1-L30

To check whether the `optional` has a value, we can either use an implicit convertion to `bool` or use the member function `has_value()`.
Once we know that the `optional` is not `nullopt`, we can access the value using `*` and `->`, just like a pointer.
Alternatively, we can use `value()` to do a checked access, which throws an exception if the `optional` is actually `nullopt`.
Conversely, `*` and `-> access the value directly without any checking or the possibility of throwing an exception, which is slightly faster.
Finally, we can use `value_or(...)`, which takes a value to return if the `optional` is a `nullopt`.

Importantly, this type does not involve any pointers internally, and holds the value of type `T` directly (on the stack).
This means that we cannot use `optional` to implement something like a linked list, since the compiler would not be able to calculate how big an object should be.
If a class `foo` contains a member of type `optional<foo>`, then the size of a `foo` object would not be calculable at compile time, so this would result in a compiler error.

### [`variant`](https://en.cppreference.com/w/cpp/utility/variant)

A more general case than just a value or nothing is when a value can be one of several types.
To handle this, C has `union`s, a type that can hold one of several types.
In C this is typically used to access the same memory as the different types of a union, like a shortcut to having to cast manually.
These `union`s are *untagged*, meaning the `union` doesn't keep track of which type is being stored inside it, which you must do yourself using a separate variable.

C++ provides a type called `variant<T1, T2, ...>`, which holds a value of one of the types `T1, T2, ...`.
This is a *tagged* union, meaning that the `variant` keeps track of which type it currently holds.

https://github.com/CIS1900/2022-fall/blob/9b90250a15f59307aa22902260e5f7e86a00b7e1/07/variant.cpp#L1-L25

To check if a `variant` `v` contains a value of type `T`, you can use `holds_alternative<T>(v)`.
And to access the value, use `get<T>(v)`, which will throw if `v` does not hold a `T`.
There is no fast, unchecked version of this operation, which is a little surprising to me.
[This discussion](https://stackoverflow.com/questions/48817080/why-stdget-for-variant-throws-on-failure-instead-of-being-undefined-behaviour) speculates on why.

Like `optional`, `variant`s holds its internal value on the stack, avoiding any pointers internally.
So like `optional`, a class cannot hold recursive instances of itself using a `variant`.

### [`any`](https://en.cppreference.com/w/cpp/utility/any)

Finally, one last thing that is fairly common in C is `void *`, a pointer to anything.
These are useful for `malloc`, wihch returns a `void *`, and for representing unknown function arguments.
There are alternatives to both in C++, `new` in place of `malloc` and various forms of function types, which we'll see later.
However, if you still want a type that can represent any other type, you can use `any`.
An `any` can be initialized with anything, and you can use `any_cast<T>(a)` to convert the `any` `a` into the type `T`.
If that `a` does not hold a `T`, this throws an exception.
Alternatively, you can try to `any_cast` a pointer to an `any`, and that will return `nullptr` on failure and a `T *` on success.

https://github.com/CIS1900/2022-fall/blob/9b90250a15f59307aa22902260e5f7e86a00b7e1/07/any.cpp#L1-L30

Using `any` should be a last resort, since it is so unstructured.
[This discussion](https://stackoverflow.com/questions/52715219/when-should-i-use-stdany) contains some examples, though I think `variant` would be a better option for many of those cases.
