# Class 10 summary

Questions I answered in class:
- Using concept name in place of `typename` vs `requires ...`: no difference, you can only do this when the concept takes one template argument (or when you've filled in all the other arguments except for one), though.
- When to use templates over OOP, or when is dynamic dispatch necessary: Generic programming and object-oriented programming are somewhat orthogonal.
I would think about it as, if you are trying to reuse code, you should use templates.
If you are trying to make your system extensible, you should use OOP.
e.g. you can write a bunch of code for objects of type `Animal`, and make that work for your `Dog`s and `Cat`s, and the advantage of OOP is that you can make new subclasses of `Animal` and it will should well with the existing code.
If both templates and subtyping can work, you should (probably) prefer templates since they are faster, since they work at compile-time rather than run-time.
- Templates and inheritance: Templates are basically just a feature for code generation, so they can work together with inheritance.
e.g. `template<typename T> class foo : public bar<T> ...` would define a class `foo<T>` which is a subclass of `bar<T>`, for any `T`.
Note that there is no subtyping relationship between argument types `T` (e.g. `foo<int>` is a subtype of `bar<int>`, but not of `bar<bool>`).
- Going over type aliases and `value_type.cpp` type constraints: The `using` syntax is just a more convenient way of doing `typedef`s, that is more consistent with the ordering of arguments and overloads the other uses of `using` in C++.
For the `value_type.cpp` type constraints:
  - Type constraints: At compile time, the compiler will check the type passed in to see if it satisfies the type constraints.
  A type constraint follows the `requires`.
  This particular type constraint is constructed using `&&`, meaning both of those pieces must be satisfied.
  The first is `ranges::range`, which checks that `Container` is a type that has `begin()` and `end()`.
  - `requires`: The second part is defined using a `requires`, which first lists some variables of some types, then lists the operations that should be valid using those variables. This constraint checks that we can use `+=` the way we do in the `sum` function.
  Note that this use of `requires` is different from the first one.
- If you write a vector class and are writing an iterator for it, should the iterator be a template too: For a vector class in particular, the iterator could be a pointer to the underlying array. Remember that iterators aren't necessarily objects, they're just some "thing" that has the appropriate operations like `++` and `*`, which fit pointers perfectly.
If you did write a class for it, it likely would be a class template since it needs to depend on the type being stored inside the vector. Then when defining `vector<T>` you could have have `begin()`, for example, return `iterator<T>`.

Questions I didn't get to:
- Using default template arguments: e.g. `queue` would be defined as something like `template<typename Value, typename Container = std::deque<Value>> class queue {...`.
Then to use this, we don't have to specify the second argument if we're ok with the default value of `deque`.
We could just declare a `queue` as `queue<int>`, and the default argument would set the second argument to `deque<int>`.
- Performance difference with `auto` vs `template`: There is no difference performance-wise, the `auto` versions are just translated to templates during compilation.
- Containers and iterators with templates:
- How `vector<bool>` saves space: Exactly how is not mandated by the standard, but a typical implementation would be to store each `bool` in a bit, and use bitwise operations to extract bits when you need them.
If you create 8 `bool`s using an array normally that will probably use 8 bytes, but if you do this that will only use 1 byte.
- Are concepts similar to typeclasses in Haskell: Somewhat, see this [SO answer](https://stackoverflow.com/a/32147454/3776956) from the designer of concepts.
- "Not all types are classes": built-in types like `int` are not classes. So classes are types, but not all types are classes. For templates you can use `class` instead of `typename` (they behave identically), but I prefer using `typename`.
- How to dynamically check the type of something at runtime: You can do this using `dynamic_cast` which I'll cover in the last class before final project presentations, but it's typically a code smell, or an indication that your code design is not quite correct.
- Where to use concepts: When your template assumes something about the type that is passed in, you probably want some kind of type constraint describing these assumptions. Concepts are common cases that are defined for you already (or that you can define yourself).
- Type aliases just for convenience: The `using` syntax vs. `typedef` is mostly just for convenience, but the type members inside class templates are quite useful.
Without them, we would not have any way of accessing types that might be "hidden" inside the other type.
For example, in our `edit` example, we need to use `value_type` to get the type of values of our container.
For `edit` we know it's always `int`, but for a different function that might work with any value type, there wouldn't be any other way of accessing it without a similar type member.
- Non-template code in `.tpp` files: The usage of `.tpp` files is purely convention, and you can write whatever you want in them.
You could always write your code in random files and then `#include` them, though it would be very strange to see that.
- Can templates generalize any class: yes, any type can be passed in as a template argument, any class or built-in type like `int`.
- Templates and performance: templates basically automate copy and pasting a class or function, and replacing the type everywhere. So there is no runtime performance overhead compared to writing the class specialized to a certain type. The compilation overhead can vary greatly based on what you're doing.
- in-place mergesort using iterators: in-place mergesort is actually very tricky to get right, so I won't show an example. See some options on [wikipedia](https://en.wikipedia.org/wiki/Merge_sort#Variants).
- Separate compilation: separate compilation does not work with templates. Remember that with separate compilation, the compiler compiles each translation unit on its own, without knowledge of other translation units. In this model, the templates would have to be compiled into some special "generic object file", which doesn't exist in C++, so we must `#include` the template implementations.

For exercise 1, some students who were using clang were having trouble with the `std::ranges::range` concept, which doesn't seem to be implemented in clang.
You could write this concept yourself:
```c++
template <typename T>
concept range = requires (T & t)
{
    t.begin();
    t.end();
};
```
The completed version from class is [here](exercise.cpp).
More could be added, like how the iterators support `++`.

The exercise 2 solution is [here](unique_ptr.hpp), with a [test file](test.cpp).
