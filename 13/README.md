# Class 13

The following notes are rougher than usual, since this material was covered in class.
The material on casting was covered early, in class 12, and the material on history and C in class 13.

## Casts

In general, casts do not do any sort of actual conversion.
They only operate on references and pointers, giving the programmer a new "view" of some object using a new reference/pointer.

### `dynamic_cast`

```c++
shape * s {...};
if (square * sq {dynamic_cast<square>(s)})
   ... // the shape pointer s actually did point to a square, not some other shape
```

`dynamic_cast` is used for casting within a class hierarchy.
The cast is checked at runtime (*dynamically*), returning `nullptr` (when casting a pointer) or throwing an exception (when casting a reference) if it fails.
It is most commonly used for downcasting: casting a base class pointer/reference to a derived class.
In general, this is a *code smell* that tells you there may be something wrong with the design of your classes.
Typically your code should not require knowledge of the exact subclass a base class pointer/reference points to, and requiring it makes the code more difficult to extend with more subclasses.

### `static_cast`

[`static_cast.cpp`](static_cast.cpp)

`static_cast` performs "safe" casts, such as those between numeric types, and explicit upcasts in a class hierarchy.
The compiler checks that the types are convertible to each other at compile-time (*statically*).
These casts can often be performed implicitly, but `static_cast` is useful when you need to make the type you are casting to explicit.
`static_cast` can also be used to perform downcasts, but these are not checked like `dynamic_cast` is.
Use this if you need the performance, and are *sure* that it will succeed (e.g. if you have already checked earlier using a `dynamic_cast`)

### `const_cast`

[`const_cast.cpp`](const_cast.cpp)

`const_cast` removes `const` from a reference or pointer.
Note that you can always add `const` without a cast.
However, if the original object is `const`, modifying it is undefined behavior.

### `reinterpret_cast`

[`reinterpret_cast.cpp`](reinterpret_cast.cpp)

`reinterpret_cast` changes a pointer or reference type without any checking. It interprets the argument just as a pointer/reference to a sequence of bytes and just gives it the new type.
`reinterpret_cast` cannot remove `const`, you need to use `const_cast` for that.
No actual conversion occurs. `reinterpret_cast` just tells the compiler it can view this block of memory as the new type.

### C-style casts

You should never use C-style casts (`(int)v`) in C++!
They behave in often unexpected ways.
It performs (approximately) `const_cast`, `static_cast`, `static_cast` then `const_cast`, `reinterpret_cast`, and `reinterpret_cast` then `const_cast` until one of them succeeds.

## History of C++

- For context, other popular current languages: C (1972), Bash (1989), Python (1989), Haskell (1990), Java (1995), JavaScript (1995), PHP (1995), OCaml (1996), C# (2000), Scala (2003), Go (2009), Rust (2010), TypeScript (2012)

### Early C++

- 1979: "C with Classes" at Bell Labs by Bjarne Stroustrup.
  - Wanted to write distributed cluster code using OOP abstractions from [Simula](https://en.wikipedia.org/wiki/Simula) and using the low-level hardware and concurrency support from C.
  - According to [this Youtube video](https://www.youtube.com/watch?v=UNSoPa-XQN0&t=50s) (the source of their data is a bit sketchy), the most popular languages (in order) in 1979 were Fortran, Pascal, COBOL, BASIC, Lisp, and C.
- 1984: Renamed to C++
- At this point, C++ had most of its OOP features, `const`, function and operator overloading, references, and virtual functions.
- Notably added constructors (first time a language had support for multiple constructors) and destructors (first time ever) for resource management.
- C++ also had streams, complex numbers, and vectors at this time.
- 1991: Added templates and exceptions, and introduced the RAII idiom.

### Standardization of C++

- 1987: discussion begins about standardizing the language.
- 1991: ISO (international standards organization) C++ committee created: known as WG (working group) 21.
- 1993: STL (standard template library) submitted for standardization.
  - A library that included most of the containers (`vector`, `list`, `map`, etc.), iterators and algorithms in the standard library today.
  - You may hear the STL name used frequently still. This usually refers to the C++ standard library, but strictly speaking it refers to the original library from the 90s. I personally don't like using the term and say "standard library" or something similar instead.
- 1998: C++98 standard published.
- 2003: C++03 standard published, "bug-fix" standard to C++98.

### "Modern" C++

- 2011: C++11 standard published. This added things like move semantics, `auto`, range-based for loops, lambdas, concurrency, and many other features.
  - C++11 was originally supposed to be released in the late 2000s, so you may see it referred to in older documentation as C++0x (if we use hexadecimal, it's C++0B).
  - C++11 was a huge update to C++. The main goals were to make C++ better for systems programming and building libraries, and to make it easier to teach and learn.
- A policy of updates every 3 years was adopted, with updates alternating between minor and major.

- 2014: C++14 standard published. This update was fairly small and added mostly minor improvements.
- 2017: C++17 standard published. This update added a filesystem library, parallel algorithms, `variant`, among other topics.
  - C++17 ended up being a minor update, not the major update that was hoped for.
- 2020: C++20 standard published. This was the major update which C++17 was supposed to be. This added concepts, modules (compiler support isn't good yet), coroutines, ranges, and other smaller features.

- What _is_ modern C++? Prominent members of the C++ community are working on the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines), a set of rules and rationales for them.

### C and C++

- Neither of C nor C++ are a strict subset of the other. They are really more siblings than ancestors.

![family tree](family.png)

Picture from "A Tour of C++, 2nd Edition" by Bjarne Stroustrup.

- e.g. variable names from C may be keywords in C++.
- e.g. in C `void *` can be implicitly converted to any pointer type. In C++ it cannot.
- e.g. Linking object files is different in C and C++, since C++ has function overloading and C does not.
  - A C++ function can be given C linkage (to be used in a C program) by declaring it with `extern "C"` at the front.
- Coding style is very different.
  - Use `string` and its member functions instead of `char *` and `strcmp`, `strcpy`, etc.
  - There are many alternatives to macros, like `const`, `constexpr`, `inline`, templates, etc. in C++.
  - `new` and `delete` instead of `malloc` and `free`.
  - Avoid `void *` and casts, and if needed, use other types like `any`.
  - Use a named cast (`static_cast`, `dynamic_cast`, etc) instead of C-style casts to be precise about your intent.
  - Use built-in containers, like `array` or `vector`, plus abstractions like iterators instead of plain C arrays and pointer arithmetic.
- Note that you should avoid writing "C/C++" on your resume or other documents. At this point you should see that C and C++ are very different languages!
