# Class 10

This week we'll talk about the feature we've been using without explanation for quite a while, templates.

## Templates

Templates are useful for writing code that is parameterized by a *type*.
The way that templates work is that the compiler simply generates a different copy of the code for each instantiation of the template, like `vector<int>` and `vector<double>`.
This results in no runtime overhead from using templates, though the compilation time overhead can be significant.
For larger projects that make heavy use of templates, hours-long compile times are not unheard of.

We've seen many examples of templates already.
Any code that uses angle brackets, like `<T>`, is passing an argument `T` to a template.

### Function Templates

The first kind of template we'll look at is the *function template*.
This is a function that abstracts over input types.
The template parameters, identified by the `typename` keyword, go inside the angle brackets.
You could also use `class` instead of `typename`, but I prefer `typename`, since not all types are classes (e.g. built-in types like `int`).
`class` is the older syntax, and is more common in older code.
Here is an example of how the standard library `std::swap` might be implemented:

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/function_template.cpp#L1-L33

Here, we were not required to explicitly give the template argument when we called `my_swap`, since the compiler can deduce it from the arguments we pass.
This example also includes an alternate way of writing function templates called an [abbreviated function template](https://en.cppreference.com/w/cpp/language/function_template#Abbreviated_function_template): using `auto`.
Each parameter with `auto` as its type is equivalent to adding another template parameter and giving the function parameter that type.
Here, this use of `auto` is not ideal, because for a `swap` function we really want our parameters to have the same type, which we cannot express using this syntax.

Last class, we talked about the fact that most parts of the standard standard library are *not* object-oriented, to avoid the overhead of virtual functions.
Instead, we can use templates to write code that abstracts over these types, since they still have a common interface, just not one that is formalized in a base class.

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/containers.cpp#L1-L21

Note that templates are not a complete replacement for subtyping and virtual functions.
While templates are useful for writing code for different types, they do *not* provide dynamic dispatch, where the program can choose which function to call based on what type an object has, all at *run time*.
Instead, all the work that templates do is at *compile time*.
They generate code and call functions for different types, based purely on compile-time type information, known as *static dispatch*.

### Class Templates

We can also write *class templates*.
We've seen lots of examples of these already, like smart pointers and container classes.

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/class_template.cpp#L1-L29

Unlike function templates, there is no abbreviated version using `auto`, since it is typically essential to refer to the name of the type in a class template.

### Separate Compilation

Unlike normal classes, templates do not allow for separate compilation.
This is because when a file `foo.cpp` uses a template, like our `wrapper` class, and instantiates its template argument with say, `int`, then the compiler needs to generate the `wrapper<int>` code when compiling `foo.cpp`, which requires *all* the `wrapper` code, including the definitions of its member functions.
If the template implementation *were* compiled separately, then it would have to know in advance what template arguments it needs to be instantiated with, which is not possible since it may be linked with any arbitrary code later on.

The solution is to implement all your functions in the header file.
You could do this directly, like in our example above, or you could break up the interface and implementation for organizational reasons.
The class definition would go in a header file as usual, and the function definitions can go into an implementation file (usually with a `.tpp` file extension for template implementations), which is then `#include`d at the bottom of the header.
The following shows this approach for the same `wrapper` class example as earlier:

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/templates/wrapper.hpp#L1-L17

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/templates/wrapper.tpp#L1-L15

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/templates/main.cpp#L1-L11

## Type Constraints

Often, we want some restrictions on what kind of type arguments are acceptable for a template.
For example, the `print` function template in the earlier example expects its template argument to be a container type that can be iterated through using a ranged-based for loop.
If we call `print` with a template argument that is not a container, like `print(1)` (where the deduced template argument is `int`), then things will go wrong.
However, due to how templates work, what happens is that the compiler will generate the code for `print<int>`, and only then will it see that something is wrong, that we are trying to iterate over an `int` using a range-based for loop.
This leads to long and confusing error messages, like the following for `print(1)`:

```
print_error.cpp: In instantiation of ‘void print(Container) [with Container = int]’:
print_error.cpp:23:12:   required from here
print_error.cpp:12:5: error: ‘begin’ was not declared in this scope
   12 |     for(auto i : c)
      |     ^~~
print_error.cpp:12:5: note: suggested alternatives:
In file included from /usr/include/c++/10/string:54,
                 from /usr/include/c++/10/bits/locale_classes.h:40,
                 from /usr/include/c++/10/bits/ios_base.h:41,
                 from /usr/include/c++/10/ios:42,
                 from /usr/include/c++/10/ostream:38,
                 from /usr/include/c++/10/iostream:39,
                 from print_error.cpp:1:
/usr/include/c++/10/bits/range_access.h:108:37: note:   ‘std::begin’
  108 |   template<typename _Tp> const _Tp* begin(const valarray<_Tp>&);
      |                                     ^~~~~
In file included from /usr/include/c++/10/bits/stl_iterator_base_types.h:71,
                 from /usr/include/c++/10/bits/stl_algobase.h:65,
                 from /usr/include/c++/10/bits/char_traits.h:39,
                 from /usr/include/c++/10/ios:40,
                 from /usr/include/c++/10/ostream:38,
                 from /usr/include/c++/10/iostream:39,
                 from print_error.cpp:1:
/usr/include/c++/10/bits/iterator_concepts.h:896:10: note:   ‘std::__detail::begin’
  896 |     void begin(const auto&) = delete;
      |          ^~~~~
In file included from /usr/include/c++/10/string:54,
                 from /usr/include/c++/10/bits/locale_classes.h:40,
                 from /usr/include/c++/10/bits/ios_base.h:41,
                 from /usr/include/c++/10/ios:42,
                 from /usr/include/c++/10/ostream:38,
                 from /usr/include/c++/10/iostream:39,
                 from print_error.cpp:1:
/usr/include/c++/10/bits/range_access.h:855:44: note:   ‘std::ranges::__cust::begin’
  855 |     inline constexpr __cust_access::_Begin begin{};
      |                                            ^~~~~
print_error.cpp:12:5: error: ‘end’ was not declared in this scope
   12 |     for(auto i : c)
      |     ^~~
print_error.cpp:12:5: note: suggested alternatives:
In file included from /usr/include/c++/10/string:54,
                 from /usr/include/c++/10/bits/locale_classes.h:40,
                 from /usr/include/c++/10/bits/ios_base.h:41,
                 from /usr/include/c++/10/ios:42,
                 from /usr/include/c++/10/ostream:38,
                 from /usr/include/c++/10/iostream:39,
                 from print_error.cpp:1:
/usr/include/c++/10/bits/range_access.h:110:37: note:   ‘std::end’
  110 |   template<typename _Tp> const _Tp* end(const valarray<_Tp>&);
      |                                     ^~~
/usr/include/c++/10/bits/range_access.h:856:42: note:   ‘std::ranges::__cust::end’
  856 |     inline constexpr __cust_access::_End end{};
      |                                          ^~~
/usr/include/c++/10/bits/range_access.h:434:10: note:   ‘std::ranges::__cust_access::end’
  434 |     void end(const auto&) = delete;
      |          ^~~
```

If you read it carefully, you can see that the issue is that `int` does not have the `begin` and `end` members.
From this you can infer that the problem is that the function template expects a container as the template argument, but this is far from clear, as you may have experienced in previous assignments.

C++ allows us to define *type constraints* for templates, or requirements on types.
These requirements are then enforced at compile-time, and result in better error messages.
For the `print` example from before, we can add a requirement that the template argument be a `range` (from the `ranges` library), or something that can be iterated through using `begin()` and `end()` iterators:

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/print_error.cpp#L1-L26

Compiling this function now gives an error like:

```
print_error.cpp: In function ‘int main()’:
print_error.cpp:25:12: error: use of function ‘void print(Container) [with Container = int]’ with unsatisfied constraints
   25 |     print(1);
      |            ^
print_error.cpp:10:6: note: declared here
   10 | void print(Container c)
      |      ^~~~~
print_error.cpp:10:6: note: constraints not satisfied
In file included from /usr/include/c++/10/string:54,
                 from /usr/include/c++/10/bits/locale_classes.h:40,
                 from /usr/include/c++/10/bits/ios_base.h:41,
                 from /usr/include/c++/10/ios:42,
                 from /usr/include/c++/10/ostream:38,
                 from /usr/include/c++/10/iostream:39,
                 from print_error.cpp:1:
/usr/include/c++/10/bits/range_access.h: In instantiation of ‘void print(Container) [with Container = int]’:
print_error.cpp:25:12:   required from here
/usr/include/c++/10/bits/range_access.h:872:13:   required for the satisfaction of ‘range<Container>’ [with Container = int]
/usr/include/c++/10/bits/range_access.h:872:21:   in requirements with ‘_Tp& __t’ [with _Tp = int]
/usr/include/c++/10/bits/range_access.h:874:15: note: the required expression ‘std::ranges::__cust::begin(__t)’ is invalid
  874 |  ranges::begin(__t);
      |  ~~~~~~~~~~~~~^~~~~
/usr/include/c++/10/bits/range_access.h:875:13: note: the required expression ‘std::ranges::__cust::end(__t)’ is invalid
  875 |  ranges::end(__t);
      |  ~~~~~~~~~~~^~~~~
cc1plus: note: set ‘-fconcepts-diagnostics-depth=’ to at least 2 for more detail
```

This is shorter and nicer than the previous one, telling us explicitly that there's a problem with the type: it does not satisfy the constraint `range` because it doesn't have `begin` or `end`.
This type constraint is specified by the `requires ranges::range<Container>` line.
If it is commented out, then you would get an error similar to the first longer one above.
This line says that the requirement is something called `ranges::range`, applied to the type `Container`.

In place of `template <typename Container> requires ranges::range<Container>`, this could also be written as just `template <ranges::range Container>`, replacing `typename` with the constraint that should be applied to that argument.
Another alternative is to use the `auto` syntax, and to write `void print(ranges::range auto c)`, which applies the constraint to the type of `c`.

As another example, if we consider the previous `wrapper` code, then using a type that disable copy assignment for its template argument should fail, since `set` uses the copy assignment.
However, if we never call `set`, then `wrapper<foo>::set` is never generated, and the code compiles without error.
By adding a type constraint `copyable`, we can catch this error "early" in compilation, rather than "late" as is usual with templates:

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/wrapper_error.cpp#L1-L37

Comment out the type constraint to see this code successfully compile as long as `set` is not called.

If we do want this behavior that it should be ok that `T` does not support `=` as long as we never call `set`, then we can do this as well.
We can add the type constraint to the member function only, instead of the entire class template, using `void set(T t) requires copyable<T> ...`.
Then, this requirement on the type `T` is only enforced if `set` is called.
Since copy construction is still used by the constructor, we should still require that type constraint for the entire class, which we can do using `requires copy_constructible<T>` for the class template.

### Concepts

The type constraints we saw above used named constraints, which are called *concepts* in C++.
These concepts were defined in the standard library, and we can also define our own.
While the above examples use a single concept each, multiple can be combined using `&&` and `||` after the `requires`.
The ones in the example above all are requirements on a single type, but concepts can also be constraints over multiple types, such as `convertible_to<From, To>`, which requires that `From` is convertible to `To`.

In the following example, we define a concept requiring that the type `T` allows `const T &`s to be added using `+`:

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/concepts.cpp#L1-L23

Concepts are essentially predicates that are checked by the compiler at compile-time.
This concept checks that `x + x` is a valid expression.
For one-off type constraints, it is not necessary to name the constraint as a concept.
A `requires` expression can be used on its own, however note that it is separate from the `requires` used to signal that a constraint is coming up, so we would have to write
```c++
template <typename T>
requires requires (const T & x) {x + x;}
```
(with the double `requires`) if we wanted to write the `addable` concept inline.

Built-in concepts in the standard library can be found [on this page](https://en.cppreference.com/w/cpp/concepts).
These concepts often have some *semantic* meaning on top of the *syntactic* ones in the concept definition.
For example, this could be things like `addable` ensuring that `a + b == b + a`.
Such semantic constraints are only in the comments and documentation, and are not checked by the compiler.

You may notice that the error messages for standard library classes like `vector` or `unique_ptr` are long and cryptic, exactly the problem that type constraints and concepts aim to solve.
This is because the standard library does not typically use type constraints in order to (I'm speculating here) keep these class templates flexible.
For example, while it may be useful to have the values in a `list` to be copyable, it would not matter if the `list` is never copied.
Though as mentioned earlier, this could be achieved by attaching type constraints to individual member functions.
A second---and probably more important---reason is that concepts and type constraints are very new, only introduced in C++20, so it may just take time before they are added to existing code in the standard library.

## Other Notes on Templates

### Default Template Arguments

Templates also support default arguments, which is how class templates like `queue` have a default underlying container type.
These have similar rules as default argument for normal functions, and are written using `=`.

### Template Specialization

Templates can also be *specialized*, which is when a custom implementation is given for specific template arguments.
For example, the standard library provides a specialization for [`vector<bool>`](https://en.cppreference.com/w/cpp/container/vector_bool) to make it more space efficient.
This is because the size of a `bool` is implementation-defined, and is typically not one bit, but a byte.
In general, it would be difficult to make a 1-bit `bool` work properly.
For this reason, `vector<bool>` gives a more space-efficient implementation, though how it is more efficient is implementation-defined as well.

We will see examples of template specializations in next class's notes.

### Type Aliases in Templates

All containers have some common *member types*, like `value_type` which holds the type of the value inside the container, so these can be used generically in templates.
To do this, the template uses `using`.
Inside the definition of the container class template, `using value_type = T;` would define a member type named `value_type` equal to `T` (assuming that `T` is the name of the template argument).
This is the same as `typedef`, and is equivalent to `typedef T value_type;` in C, but is preferred since its ordering is the same as regular variable assignment.

When using such member types, it is necessary to prepend `typename` to the member type, to tell the compiler that the member is a type:

https://github.com/CIS1900/2022-fall/blob/d91d40f24cae43d79b0da73dcd98b96db62e39ec/10/value_type.cpp#L1-L34

In this example, the compiler cannot know that the abstract `Container` type's member named `value_type` is a type, since `Container` can be instantiated with anything.
So to help it along and make this code compile, we need to tell it that this member is supposed to be a type using `typename`.
For things like `vector<int>::iterator`, this was not necessary, since all types there are concrete, and it can be easily checked that this expression evaluates to a type.

The above code also contains an example of a more complex type constraint, requiring both that we can iterate through the container and that the `value_type` of the container can be summed up using `+=`.
