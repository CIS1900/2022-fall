# Class 11

## Higher-Order Functions

Higher-order functions, as you've seen in CIS 1200, are really useful.
To pass around functions, we can use function pointers, but these are not ideal.
Combining function pointers with template arguments can be difficult, and function pointers only work with regular functions, defined at compile-time.
This means you cannot dynamically build a function at runtime and obtain a function pointer to it.
Function objects solve both of these problems in C++.

## Function Objects

A function object, also called a *functor* in C++, is an object that can be called as a function.
This is done by overloading `operator()` in the class.
The parameter list and return type of `operator()` can be fully customized, and serves as the parameter list and return type of the functor as a whole.
Since a function object is still a normal object, you could give the class some member variables in order to change its behavior:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/functor1.cpp#L1-L25

The class of a function object can also be made into a class template, to accomodate different type arguments.
Then, a function template that expects a function argument can use a template argument to specify the type of the function, as shown in this example:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/functor2.cpp#L1-L40

This code uses the `predicate<T>` concept, which constrains the type to be something that can be called with a `T` and returns something that can be converted to a `bool`.
This allows for both functors and function pointers to be passed in as arguments.

Other useful concepts involving "callable" types like functors and function pointers are `invocable` and `regular_invocable`, which specify argument types.
The difference between the two is purely semantic (i.e. their definitions are equal, but they are supposed to have different meanings when used).
`regular_invocable` requires that the function is equality-preserving (i.e. equal inputs always give equal outputs), which rules out functions like random number generators.
These concepts do not specify the return type, which can instead be specified using the `invoke_result` and `convertible_to` concepts together.

### Hash

As mentioned in an earlier class, `hash` is implemented using templates, to avoid the overhead from using virtual functions.
`unordered_map` uses `std::hash` by default to hash values.
One way to provide a hash function for a custom key class is to write your own class for a functor, and pass that to the `unordered_map` to use instead of `std::hash`:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/hash1.cpp#L1-L32

This hash function uses bitwise xor (`^`) and left bitshift (`<<`) to combine the hash values of the first and last name (obtained using `std::hash`) in a "random-looking" way.
`==` is required to be defined for our key class, and is used when there are hash collisions.

An alternative method is to make `std::hash` work with your class, rather than bypassing its use entirely.
`std::hash<T>` is a class template, which, when instantiated into an object, is a function object representing the hash function for the type `T`.
Of course, this class cannot provide hash functions for all possible types.
The standard library provides specializations of `std::hash` for the built-in and standard library types.
For all other types without a specialization, trying to use `std::hash` will result in a compiler error.
So for your own class, you can write a specialization of `std::hash` for it:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/hash2.cpp#L1-L35

Since `hash` is inside the `std` namespace, we first need to "open" the `std` namespace.
Then, to specialize the template, we just need to provide the type argument we are writing the specialization for.
The `template<>` with the empty `<>` is required to tell the compiler that this is still a template, just that there are no more template parameters.

### Map

As discussed in a previous class, `map` and `set` uses `<` by default to compare keys.
We can customize this using the template argument following the key and value types of these class templates, which defaults to `std::less<Key>`.
Objects of this type are function objects that take two `Key`s and returns the result of comparing them using `operator<`.
The standard library also provides `greater<Key>`, `less_equal<Key>`, and `greater_equal<Key>`, though the ones involving equality are likely not suitable, as shown by the [example](../09/map2.cpp) from class 9.

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/map.cpp#L1-L37

Remember that just as with `std::hash`, the template argument is a *type*, where every object of this type is a function object, and each of these function objects behaves identically.

## Lambdas

While function objects are a good solution for representing functions, it is very inconvenient to write a new class for each function.
One useful feature in C++ is that classes can be nested within other classes and functions, reducing the amount of classes polluting the overall context.
An even more useful feature for shortening your function object code is the *lambda*.

A lambda expression is basically shorthand for declaring a function object (not the class of that function object!):

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/lambda.cpp#L1-L19

The syntax is fairly straightforward, allowing you to write the parameter list and body of the function as usual.
The `[]` part is the capture list, discussed below.

The type of a lambda expression is *unspecified*, meaning you cannot write down the "real" type of the lambda, forcing you to use `auto`.
If you do need to write down the type of the lambda (e.g. if you want to use a lambda as the comparison type for `map`), then you can use `decltype` to get the type of the lambda.
For example, if you have an `auto lam = [](...){...};`, then you can make a copy using `decltype(lam) copy {lam};`.
`decltype` stands for "declared type", and gives the type of an expression.

The `[]` is the *capture list* of the lambda expression.
The lambda "captures" variables listed inside the brackets, allowing the body of the lambda to use them.
`[var]` captures `var` by value, copying the value of `var` when the lambda is created.
On the other hand, `[&var]` captures `var` by reference, taking a reference to the original variable, allowing for the lambda to see and make changes to the variable.
Multiple variables can be captured by separating them by commands in the capture list.
Finally, `[=]` and `[&]` capture *everything* by value and reference respectively.

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/capture.cpp#L1-L24

Lambda expressions can also be *generic*, by using `auto` for its arguments.
This is basically shorthand for declaring a template class for a function object:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/generic_lambda.cpp#L1-L14

To write the return type of a lambda, you can use the syntax `-> T` after the parameter list to specify that the lambda has return type `T`.
Typically this can be inferred and it can be omitted, but sometimes it must be manually specified.
When dealing with generic lambdas, `decltype` is often useful here, since you do not know the types in advance.

## `<algorithm>`

The standard library provides a long list of algorithms in `<algorithm>`.
Most of these are function templates that take iterator arguments.
In fact, the current standard library is heavily based on the *standard template library*, or STL, a library written in 1993.
The influence of this library is massive, and many people still call the standard library the STL.
We will look at some examples of function templates provided by the standard library next:

`for_each(first, last, f)` calls a function `f` on each element of the range defined by iterators `first` and `last`.
A range-based for loop is often simpler for an entire container, but `for_each` allows the exact range to be specified.

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/for_each.cpp#L1-L18

`find(first, last, v)` returns an iterator to the first place where `v` is found in the range, or the end iterator `last` if no match is found.
There are also `find_if` and `find_if_not` variants that take a predicate rather than just a value.

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/find.cpp#L1-L27

This example gives a taste of the variety of functions available in `<algorithm>`:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/algorithms.cpp#L1-L42

`iota` fills a range with an incrementing sequence, starting at some value.
It is named after the ι function that did the same thing in APL, a programming language.
The standard library also provide classes (in `<random>`) that generate pseudorandom as well as *truly random* numbers in much more sophisticated ways than C `rand` and `srand`.
See the [documentation](https://en.cppreference.com/w/cpp/numeric/random) for more details.
`shuffle` reorders the range such that any permutation has equal probability to be chosen.
It requires a source of random numbers.
`replace` and `replace_if` replace some elements of a range.
`sort` sorts a range defined by *random access* iterators.
Other containers like `list` have a member `sort` function, since the implementation has to differ from the random-access one.

### Modifying Containers

Algorithms that take iterator arguments operate on *ranges* defined by those iterators, so they cannot access the container directly.
This means that these functions cannot directly affect the size of the container, for example by calling `insert` or `erase`.

For example, `remove` shifts the elements in the range to remove some elements, and returns a new end of range iterator. The underlying container's size is unchanged:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/remove1.cpp#L1-L22

One option to modify the container is by directly referring to it.
For example, after calling `remove`, we can call `erase` on the container using the iterator returned by `remove`:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/remove2.cpp#L1-L16

As another example, `copy` assumes there is enough space in the destination container to copy the source range:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/copy1.cpp#L1-L14

One solution is to ensure there is enough space by resizing before calling `copy`:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/copy2.cpp#L1-L16

Another solution is to use an iterator that knows more about the container.
For example `back_inserter`, `inserter`, and `front_inserter` (in `<iterator>`) create special iterators that call `push_back`, `insert`, and `push_front` respectively, so the container grows when something is written via the iterator:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/copy3.cpp#L1-L17

### Parallel Algorithms

Most of the standard library algorithms also accept an *execution policy* as their first argument.
These execution policies specify how the function should be run.
`execution::seq`, for sequential execution, is the default when no execution policy is specified.
Passing `execution::par` allows the algorithm to be parallelized, where execution is split up and run on parallel threads.
If this code can result in a race condition when parallelized, then you must add synchronization such as `mutex`s yourself.
Additionally, there is also `execution::par_unseq` and `execution::unseq`, which permits the code to be *vectorized*.
This means that code can run out of sequence on a single thread, using [SIMD](https://en.wikipedia.org/wiki/SIMD) instructions, which are instructions that run on multiple values at once, and are available on some processors.
For example, if your code is trying to sum up an array, vectorization might allow for the first four numbers to all be added at once, instead of one by one.
This may also break some synchronization methods, so many operations like using shared memory or `mutex`s are not safe within vectorized code.

These execution policies are actually just *guidelines* to the compiler.
It is not guaranteed that using them will actually result in parallel execution, and the compiler can always fall back to sequential execution.

Execution policies are not supported by Clang and require installing additional libraries for GCC, so I won't be going in depth into any examples.
However, if they are available for your platform, then they are a very lightweight and easy way to improve performance.

## Functional Programming in C++

C++ is a multi-paradigm language.
We've seen object-oriented programming, generic programming, and procedural programming so far, as well as some rudimentary functional programming features.
More sophisticated functional programming is also possible.

`bind` in `<functional>` partially applies a function, a useful operation for functional programming:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/bind.cpp#L1-L18

To specify that an argument should be left unbound, use `placeholders::_1`, `placeholders::_2`, etc . (in order).
In this example, we first create a functor using `less<int>()` (which calls the constructor of the `less<int>` class), then uses `bind` to set its second argument to 3, resulting in the function `fun x => x < 3`.

`<functional>` also provides many other functor versions of common operations, like `plus`, which just wraps `+`.
Other common higher-order functions are available as well, like `accumulate` (in `<numeric>`) which is a fold, and `transform` (in `<algorithm>`) which is a map:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/higher.cpp#L1-L21

### Function Types

So far we have used templates to let us take arguments of "function type", which could be the type of a function object or a function pointer type.
In the case when the functor is a lambda, the type is some *unspecified* unique type generated by the compiler, so you cannot write its type without `auto`.
An alternative is `std::function<T>`, which is a class that can hold any callable function of a type `T`.
This is useful for writing recursive lambdas, since `auto` needs to know the type of what it's capturing, but that type depends on itself:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/recursion.cpp#L1-L17

In this example, we need to capture by reference since capturing by value gets an uninitialized `function`.
Note that you can't return this lambda since it depends on a reference to a local variable (itself).
There are also other methods to create recursive lambdas, but this is the most straightforward.

`function` is also useful for storing a heterogeneous collction of functions with the same function signature, but not necessarily the same type:

https://github.com/CIS1900/2022-fall/blob/f4e525a9d150bcf9eb98be13aaf7aaa99f0e6255/11/functions.cpp#L1-L29

This cannot be done using templates, just like how templates cannot store a collection of objects of different types in the same container, and something like object-oriented programming must be used instead.
Just like that use case, you should prefer templates to using `function` if possible, since `function` has some runtime overhead, due to indirection from having to follow a pointer to the actual function, as well as possibly having to copy the function object.
