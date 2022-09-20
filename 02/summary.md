# Class 2 summary

I went through one solution for exercise 1: [code](strappend.c).
You could also use `strcpy` instead of manually copying the `char`s in to the new array.

While doing exercise 2, someone mentioned that M1 macs do not support GDB.
The alternative is to then use the LLVM toolchain.
Instead of `gcc` use `clang`, instead of `g++` use `clang++`, and instead of `gdb` use `lldb`.
You may need to update LLVM on your computer so that the code in assignment 0 works.

I went over some questions people had from the notes:
- Namespaces: Namespaces are typically used for libraries, so that common function names do not conflict. If you are writing a regular program, you likely do not need to use namespaces. Once we introduces classes, we will see that they are similar to namespaces in that definitions inside the class sometimes have to be prefixed with the name of the class and `::`, just like namespaces.
- Function overloading: when thinking about what function overloads are allowed, think about how the compiler would differentiate two overloads. For example, two overloads that only differ in return type would not be allowed, since you can call a function without using its return value, and the compiler would not know which to choose.
- "Spaceship" operator: This would be used just as you would typically code with other comparison operators. Its main use is to make it easier to define comparison operators for user-defined classes. By defining one operation, `<=>`, you get `>`, `>=`, `<`, `<=`, `==`, `!=`, all for free.
- Going over the example with references in the for-each loop: With the first example without references, the variable in the header of the for-each loop copies each value in the array. In the example where that variable is a reference, instead of copying each value, it references it, and we are able ot use it to change the value in the array. This is because all references are initialized by referencing something, and all other variables are initialized by copying something.
- Style and `auto`: style guides vary but many recommend you use `auto` as much as possible. It should be fairly clear what the type is from the value you initialize the variable with. For more complex types that we'll see later on when we introduce more standard library features, `auto` will be very useful to save looking up the names of the types.
- Compiling `int i = 1.1;`: The compiler sees that you are initializing an `int` with a `double`, so it can just convert the 1.1 into 1 at compile time.
- Style and `using`: It is generally good style to bring in as few names from the namespace as possible. So if you only need `cout` then it's good to use `using std::cout` instead of `using namespace std`.
- 2D arrays using `new`: If `COLS` is a compile-time constant, you can do `new int[n][COLS]`. Otherwise, you can create an array of pointers using `new`, and initialize each of those pointers using `new` again. See [this answer](https://stackoverflow.com/a/936702/3776956) for more details.
- using 0 and 1 for `bool`s: `true` and `false` are the same as `1` and `0`, just like how `A` is the same as `65`.
- How do references work under the hood: typically they are implemented using pointers, but the standard does not force them to be implmented in any specific way
- References vs pointers: try to use the least powerful tool you can, so if you make a mistake the result is not too bad. So try to avoid pointers by using references (for pass by reference) and normal stack variables (for allocating memory) when possible.
- Const reference argument example: C++ argument passing is pass by copy by default, even for objects. So if your function takes as an argument an object which could be very large, and you only need to call some getters on it, then you could avoid the expensive copy by taking that argument as a `const` reference, allowing you to query it without performing the copy.

One correction from class: I misread the stackoverflow answer, and it *does* work to put multiple names in one `using` statement, like `using std::cout, std::cin;`.

The solution to exercise 3 is here: [code](simple.cpp)

Here is a solution to exercise 4: [code](strange.cpp).
This is extremely bad style and is an example what *not* to do.
Returning references is common, but if the function doesn't have access to something that will outlive it, it can only create something with `new`, which will have to be eventually deallocated with `delete`.
This requires the caller to `delete` a reference, something that nobody expects.
In the next set of notes we'll see an example of something that returns a reference more apropriately.
