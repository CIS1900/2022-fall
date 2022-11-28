# Class 11 summary

Questions answered in class:
- What are function objects: they are not any special feature, they are just regular objects that have an overloaded `operator()`.
- Other member variables or functions for the class of a functor: you can definitely add more members to the class, even if it's a functor class, since it's still a normal class & object.
You could even add `operator()` to an existing class, so you can use its members normally *and* call it like a function.
- Inheritance when using functors: You can have class hierarchies of functor classes, since they're just normal classes and objects.
- Overhead for using functors: Yes, there is more overhead to using functors over regular functions, since we need to create the actual function object. If there is also inheritance in the mix, then there could also be overhead from virtual function calls. But otherwise, the function call itself (i.e. invoking `operator()`) should just be a normal function call, so that should be the same as any other normal function call.
- Why function objects are important, and when we would need to use them: The only real alternative to using function objects is (C-style) function pointers.
These struggle with being template arguments and dynamically creating functions, as mentioned in the notes.
To expand on that second point, one way we could customize a function is by using some global state, and customizing behavior inside the function based on that global state.
But there isn't really a way to have some state *per* function instance, like we can do with function objects.
Since each functor is an object, it can store some state inside it, allowing us to very easily customize behavior for that *specific* functor, not affecting other function objects of that same class.
- Functors in C++ vs. in Haskell: They're spelled and pronounced the same, but they're completely different concepts. Haskell functors come from category theory, and C++ functors mean "function objects". I'm not quite sure what the origins of the C++ version are, and it doesn't really make sense to me.
- `decltype` of lambdas: We did a poll in class about how each lambda has a different unique type. This type is basically an "anonymous" class that the compiler generates for a functor, which is defined by the lambda. All `decltype` has to do is look up what compiler-generated type that lambda has.
- Why not always use `[&]` or `[=]`: In general, you should always try to limit your capabilities to reduce bugs.
You might accidentally use a variable and change it, which could cause issues if you captured by reference.
With `[&]` you also start depending on local variables, and can fail if the lambda outlives the captured variable.
- Truly random numbers from `<random>`: `<random>` can only generate "real" random numbers if your machine supports it with special hardware: https://en.wikipedia.org/wiki/Hardware_random_number_generator. `<random>` is supposed to support special hardware like this through [`random_device`](https://en.cppreference.com/w/cpp/numeric/random/random_device), but will fall back to pseudorandom numbers if your machine doesn't have the special hardware.
- Why there are different compilers and why it varies based on OS: C++ is a standardized language, which means its rules and features are decided by committee.
This is nice as decisions are democratic and anyone can join the committee, allowing for many different perspectives into the language to be heard.
The committee writes the C++ standard, a document of the rules of C++, and anyone is free to write a compiler for the language.
In practice, there are three major compilers: GCC, Clang (the compiler frontend)/LLVM (the compiler backend), and MSVC (Microsoft).
The compilers have different strengths and compete with each other, improving the overall landscape of compiler performance.
MSVC is closed-source and mostly used on Windows. Its interface (command line interface and IDE, among other things) is significantly different from the others, which is why I encourage students to use another compiler for this class.
Clang and GCC are both open source, and should be installable on any machine.
GCC is very old, and around 2005 Apple started work on Clang.
It both wanted to avoid GCC's restrictive open source license, and to have a compiler with better support for Objective-C, used for many of its tools.
So eventually Apple switched to Clang, and for backwards compatibility just switched the `gcc` and `g++` commands to point to the clang compiler instead.
This was on the original goals of Clang, which is why the interfaces (e.g. command line flags) of Clang and GCC are so similar.
But nowadays, the default version of Clang on Macs is "Apple clang", which is maintained by Apple, which picks which features from mainline Clang to integrate into Apple Clang.
It is typically at least a few months behind actual Clang, and is not as feature-rich.

Questions I didn't get to:
- empty capture list `[]`: This captures nothing, meaning you will not be able to use any "outside" variables. A correction/addition to what I said in class: lambdas can use global variables without capturing, as well as `static` variables and `constexpr` variables.
- scope of `[=]` and `[&]`, and efficiency: Capture everything will only do something for variables you use in the lambda, so it is not needlessly inefficient. For scope, it's possible to capture anything currently in scope at the point of defining the lambda, so scope works as normal.
- Why `remove` when we already have `erase`: `remove` works on a range rather on an entire container, so it's a bit more general.
- Function types: Regular functions have a "function pointer" type. You can define the class of a function object in which case it will have that type. Lambdas have some unspecified compiler-defined type. These are all different types, so we can use templates to operate over all of them. If we need to put them all together, we can stuff each of these into a `std::function`, which needs the argment and return types attached to its `std::function` type.
- is "opening" a namespace like we did for the hash example dangerous: not too dangerous, you're not able to remove things from a namespace, though you are able to add functionality that might be unexpected by other code.
- Benfits of using `algorithm` functions over coding it yourself: The main benefit is not having to reinvent the wheel. `algorithm` functions are reliable and fast, and you avoid making mistakes when trying to implement it yourself.
- Advantages of using functional programming, and other paradigms in C++, and how often/how best are they mixed: The functional paradigm is very nice for processing large amounts of data, where you can easily chain together operations on a range of elements. In my experience the functional parts are best used for this and for using functions from `algorithm`. Use different paradigms as you see fit, C++ has many ways of doing anything, for better or for worse.
- Why do functional programming in C++ over other functional languages: You probably don't want to do completely functional programming in C++. At its core the language is still fairly focused on mutable state. If you want to write functional style code, I would use a functional language, but if you want to just do a few things, C++ supports it.
- Should we always use iterators to iterate through a container: No, if you need numerical indices into a vector for example you probably don't want them. They are useful in most other cases though, since they make it easier to use member functions or functions from `algorithm` that expect iterators.
- Memory mangement of functions: for functors, they are just regular objects so their lifecycles are the same as any other object. Lambdas are also just objects, and you shouldn't have to worry about managing their memory.
- type arguments for `std::less`: `std::less` is a class template, so there always has to be a type argument provided. The reason that sometimes you see only `std::less` on its own is because the compiler is able to deduce the types for it, so they don't have to be written out explicitly.
- syntax of recursive lambda: We are creating a `std::function` object, and initializing it with a lambda, which is a function object. The lambda captures `fib` by reference. At this point the reference is not really valid yet, but that's fine since the lambda won't be called until later.
When `fib` is called, the reference now works properly, and it refers to itself.
- Why is there so much emphasis on things like virtual and small performance differences, isn't the overhead insignificant: The overhead may be insignificant for some applications, but for others where speed is important (e.g. embedded programming, finance, gaming, fields where C++ is popular), it can be crucial to cut down on overhead. [This post](https://stackoverflow.com/questions/449827/virtual-functions-and-performance-c) includes some performance tests that people did. One quote: "From this I conclude: yes, virtual functions are much slower than direct functions, and no, unless you're planning on calling them ten million times per second, it doesn't matter. "
- When to use templates for helper functions: Don't use them if they're not needed. I'm assuming you mean the case when the function of interest is a template, and it uses a helper function. If the helper function doesn't rely on the type argument of the main function, then don't include it.
- why do both `invocable` and `regular_invocable` exist: To signal different things. `invocable` is just any old function, but `regular_invocable` is a more well-behaved and deterministic function. You might use it in cases when your function expects that (e.g. maybe you can memoize `regular_invocable` calls).
- When are templates helpful: Mainly when your code needs to work for different types.
- Going over the modifying containers section: The main point is that if you only have an iterator, or only an index, *without the container itself*, how can you remove an element from the container? So the `<algorithm>` functions cannot do this, and instead just shift elements around, and you need to do something more to modify containers.

Exercise: we wrote [`shapesoln.hpp`](shapesoln.hpp), [`shapesoln.cpp`](shapesoln.cpp), and [`main.cpp`](main.cpp).
A slightly different version of the shape class is in assignment 7.
