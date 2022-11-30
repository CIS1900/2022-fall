# Class 12 summary

Questions answered in class:
- What is metaprogramming: code that deals with other code as data. Examples include templates and macros. Confusingly, what we call "Template metaprogramming" is different, it refers to doing compile-time computation using templates as a functional language.
- When to prefer compile-time computation: whenever possible! If you can compute something at compile-time, why have the program compute it every time it's run?
Some useful cases are if you are computing a table of mathematical values (think tables from calculus or statistics), or precomputing the results of some mathematical functions (maybe something useful for graphics code).
As a concrete example, for a graphics library, you may need a bunch of matrices for different matrix sizes. Template metaprogramming or `constexpr` functions allows you to compute these at compile time for whatever matrix dimensions you need.
- `constexpr` functions or template metaprogramming: almost always `constexpr` functions. These are much easier to write and typically easier for the compiler to evaluate, since this is the primary purpose of them, whereas templates are not meant for this, and rely on "loopholes".
For example, it would be much better to implement the fibonacci function using `constexpr` over template metaprogramming.
Template metaprogramming is much older, and is partly around for backwards compatibility reasons. Another area is computing at the type level, which `constexpr` cannot do, but template metaprogramming can.
- Why use `constexpr`, e.g. isn't it unnecessary for the `constexpr1.cpp` example: The compiler should be able to figure out that `b` is a compile-time constant from looking at the code, but `constexpr` is good here to signal that fact directly. Try removing `constexpr`, and the compiler complains, and adding `const` fixes it, but as we see in that example `const` is not always sufficient.
- is `constexpr` for the programmer to compute things as they code, or mostly for performance : Mostly for performance, and for marking functions as usable for computing compile-time constants for things like array lengths and template arguments. It would be cool (and seems feasible) to have a tool that runs your `constexpr` code for you while you code though.
- For template metaprogramming, why is the variable be static: we did a poll about several different options. A member function doesn't work since then the generated code just calls other functions, which must still be evaluated at runtime. A (non-static) member variable doesn't work since then it would only exist as part of an object, which would also be evaluated at runtime. A static member variable is basically a global variable and works for template metaprogramming.
- Topics to study further for C++ learning: This depends a lot on what you want to do in C++. C++ is a very large language, and every C++ programmer focuses on some areas. Even Bjarne Stroustroup, the creator of the language does not know every corner of the language.
Broadly I would recommend C++ books.
Online resources are ok, but it can be hard to filter out the good ones from the bad, especially for newer versions of C++.
The main book I used for this class is "A Tour of C++, 2nd Edition" by Bjarne Stroustroup, and the 3rd edition came out a few months ago.

Questions I didn't get to:
- Performance benefits to using lambdas: there's not really a performance benefit, it's mostly used for convenience over having to write a whole new class for a functor.
- When to use `constexpr` vs `const`: They signal different things. `const` prevents the user from changing this value, and `constexpr` says this value is known at compile-time.
- `constexpr` implies `const` and is useful for declaring constants: these are two separate points. First, if you have a `constexpr` variable it is also `const`, so it wouldn't work for non-const settings. Second, it basically replaces `#define` from C. If you have a global constant you could just declare a global `constexpr` variable, e.g. `constexpr double PI {3.14159};`.
- is the fact that the power is 2 important for the `pow` example: no, it would probably also work for other powers of 2. The point is that template metaprogramming generates the code for you, then the compiler compiles which can introduce optimizations. If all this were at runtime, we don't get those compiler optimizations.
- Common cases for metaprogramming: See this [SO post](https://stackoverflow.com/questions/63494/does-anyone-use-template-metaprogramming-in-real-life) for some examples, in addition to the ones above.
- how much faster is template metaprogramming: There should be no runtime cost with the way we did it in examples, so it can be very effective. It depends on what you do. If you are generating code that does call functions and so on, there will still be runtime costs.
- Template metaprogramming being turing complete: The paper I referenced shows the exact construction of the turing machine, but basically any language that allows for unbounded recursion is turing complete (see equivalence of lambda calculus and turing machines, and the Church-Turing thesis), and our examples show that we can perform general recursion (though the compiler sets a recursion depth).
- Differentiating type and value template arguments: type arguments are prefaced with `typename` or `class`, and value arguments have a type in front like `int`.
- How to implement GUIs: You'll need to use some external libraries. See the [page on libraries](../libraries.md).
- When can you not make a function `constexpr`: there are some prohibited operations, like gotos, or try-catch blocks. The full specification is in the [documentation](https://en.cppreference.com/w/cpp/language/constexpr).
- How common is template metaprogramming/`constexpr` functions: For the average programmer, it isn't that important. But for some areas where performance is important these can be very heavily used. For example, I have never run into template metaprogramming "in the wild", but this is a common selling point of C++, and a lot of language design work goes into supporting it. This relates to the point above about how C++ has many niches. This is a small niche, but can be very powerful for work in that niche.
- Compiler error when trying to modify a `constexpr` variable: yes, you get a compiler error, similar to if you try to modify a `const`.
- memoization and template metaprogramming: memoization is built-in to templates! If you use a `vector<int>` twice, the compiler doesn't generate that code twice, it just reuses it. So when programming functionally using template metaprogramming, memoization is automatic, courtesy of the compiler.
- Balance speed and ease of coding/reading with templates: A good principle to follow here is "premature optimization is the root of all evil". Templates do make things harder to read and make error messages worse. If it is clear that templates are necessary (e.g. a container class), then use it. If it's to improve performance before it's clear that you need improved performance, it is likely better to hold off until you need to optimize.
- When to use value template arguments instead of regular arguments: when the argument is always known in the code.
For example, it makes sense for `pow` since for the typical use case, the base of the exponent is not a variable and is known in advance.

We then did the exercise. The solution is [here](collatzconstexpr.cpp). The function is just the natural way you would write it, with a `constexpr` in front. An iterative version should work too.

Lastly, I did a short lecture about casting in C++.
This will be in next week's notes.
