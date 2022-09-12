# Class 1

## What is C++?

C++ was originally created in 1979 as "C with Classes" by Bjarne Stroustrup.
After many decades of development, it is now a general purpose, multi-paradigm language, where one can program in imperative, functional, object-oriented, or generic styles.

Unlike many languages, C++ is not managed by a company or person, but by a committee.
Anyone can join the committee if they commit to meetings and pay a small fee.
The committee votes and decides on changes to the language and any new features.
Rather than write a compiler for this language, the committee creates a *standard*, describing how the language should behave.
This allows for others to write their own compilers, all implementing a common standard.

### Design Principles of C++

According to Bjarne Stroustrup, C++ has 2 major design principles:
- Zero-overhead abstractions
  - Abstractions should not have any more overhead than implementing them manually.
- Direct map to hardware.
  - No language "below" C++ besides assembly.
  - C++ operations directly map to hardware operations. e.g. the standard library `vector` is just a contiguous block of memory, like an array. Arithmetic operations map directly to the assembly instructions.

Of course, since the standard is set by the entire committee, other contributors have different priorities for what they want when thinking about the future of the language.

## Logistics

See the course webpage for full details.
This will be an active learning class.
For future classes, you will be required to read the course notes ahead of time to get familiar with the material.
Before class, you will complete a small quiz to check your understanding.
The quiz is only graded for completion.
In class, we will do small ungraded exercises and bigger graded assignments to solidify understanding, both in pairs.

## C

C++ was designed to be very similar to C, and inherits much of the syntax of C.
You are expected to already be experienced with C for this class.
In this lecture, we will do a very quick review of the language.

### Hello World

We start with a standard hello world program:

https://github.com/CIS1900/2022-fall/blob/26cc1f911eb59684b41d9e2e58d300ba443f4cb4/01/hello.c#L1-L7

Compile this program with `gcc -Wall -g -o hello hello.cpp`, and run the resulting executable with `./hello`. The command line options we used are the following:
- `-Wall` enables all compiler warnings, so we can see if the compiler finds any strange things in our code. Try adding an extraneous variable to the file and compile with and without `-Wall`.
- `-g` enables debugging information in the output executable. This allows us to use the `gdb` debugger to interactively step through our program using that extra information (more on this later).
- `-o hello` specifies to put the output executable in a file called `hello`. If you omit this option, it will put your executable in a file named `a.out`.

The first line of this program is `#include <stdio.h>`.
`#include` is a *preprocessor directive*, which are instructions that occur early on in the compilation process.
`#include` tells the preprocessor to include (basically copy and pasting) the contents of a file at that location.
By using `<stdio.h>` in angle brackets, this signifies to the preprocessor that it should be looking in the typical locations where library header files are stored for that file.
Conversely, if you use `"stdio.h"` in double quotes, it will tell the preprocessor to look first in the current directory, and if it can't be found, then fall back to the same process as the angle brackets.
`stdio.h` is the header file for the input/output library for C.
C headers typically have the `.h` file extension.
We need it here for the function declaration for `printf`.

The next line defines the `main` function.
This is the starting point of any C program.
It can optionally take arguments of type `int` and `char **`, for the number of command line arguments and the values of those arguments, if the program was run with arguments.
`main` returns `int`, which can be queried from the command line.
`0` represents successful termination, and non-zero return values represents some error has occured.
The exact non-zero value returned can be used as an error code, signifying how the program reached that error.

The last part of this program is the call to `printf`.
We call `printf` with the string `"Hello, world!\n"`, which prints out
```
Hello, world!

```
Notice that it prints out a new line after the text.
This ensures that the command line prompt starts on a new line once the program terminates.
The `\n` is an *escape sequence*, which always start with a backslash in C, and represent another character, in this case a new line character.
If you want to represent the backslash character directly, you need to use `\\`.

### Functions Declarations

Functions must be *declared* before being used.
This function declaration is also called a function *prototype*, and includes the name of the function, its return type, and the number and type of its arguments.
A function definition also functions as a declaration, but declarations are sometimes necessary, when you have cyclic dependencies between functions, like the following:

https://github.com/CIS1900/2022-fall/blob/26cc1f911eb59684b41d9e2e58d300ba443f4cb4/01/evenodd.c#L1-L22

There is no way to reorder the function definitions so they are defined before use, so we need a declaration at the top:

```c
int odd(int);
```

Note that with default command line options, GCC does compile this code with a warning.
*Implicit declarations* were legal in C89 and previous versions, but were removed in the C99 standard.
GCC chooses to allow them by default (probably for historical reasons), but other compilers do not have to.
If you compile with `-pedantic-errors` or `-Werror`, then this will instead result in a compiler error.

This is also why we need header files, which include the function declarations for a library.
The definitions of these functions are typically not in the header files however.
These definitions are found later on, during the linking process.

Multiple declarations are ok, as long as all of their types and arguments match.
However, multiple definitions are not allowed, even if they consist of the same code.
This is why headers typically do not contain definitions.

### Control flow

The `if` statement is the most commonly used way to branch on some boolean value.
The following are some common ways of using if statements:

```c
if (condition)
{
    ...
}
```

```c
if (condition)
{
    ...
}
else if (condition)
{
    ...
}
else
{
    ...
}
```

```c
if(condition)
    statement;
else
    statement;
```

Since C does not have boolean types, the condition should be of integral type, where 0 is interpreted as false and anything else is interpreted as true.
Curly braces are required if you want to run multiple statements in one of the branches.
If you only have one statement however, curly braces are not required.

Another way of branching on some value is `switch`.
`switch` allows you to examine an expression and write `case`s for what the value is equal to.
The following is an example of a `switch` on a `char` variable:

```c
char c = ...;

switch (c)
{
case 'a':
    ...
    break; // leave the switch
case 'b':
    ...
    // falls through to the next case
default:
    ...
}
```

Unlike `if` statements, `switch` statements are a bit less structured.
Within each `case` clause, once the code within that `case` is done, it *falls through* to the next `case`.
If there is a `break`, however, that causes the program to leave the `switch` entirely.
There is also a special `default` case which runs either due to fall-through or if none of the other cases match.

C has three kinds of loops to repeat statements.
The first is a for loop:

```c
for (init; condition; update)
{
    ...
}
```

The `init` portion is run first, then the loop begins to iterate, as long as `condition` is true.
After each iteration, the `update` portion is run.

A `while` loop is like a `for` loop, but without the initialization and update portions:

```c
while (condition)
{
    ...
}
```

Like with `if` statements, if the body of the `for` or `while` loop is a single statement, the curly braces can be omitted.

The third type of loop is a `do-while` loop:

```c
do
{
    ...
} while (condition);
```

The `do-while` loop is similar to a `while` loop, except the condition is not checked for the first iteration, ensuring that it always runs at least once.
Note the semicolon at the end, which is not present in the other types of loops.

### Pointers

A pointer is an address to some other value in memory.
By writing a `*` after the type, it becomes a pointer type to that type.
Given a variable `v` (of type `T`), you can obtain the address to the memory it refers to using `&v` (of type `T *`).
This is called the "address of" operation.
If you have a pointer `p` (of type `T *`), you can access the memory it is pointing using `*p` (of type `T`).
This is called deferencing the pointer.

Pointers are mainly useful for two things:
- Passing variables by reference
- Dynamically allocating memory

The first reason, passing by reference, is related to function calls.
When calling a function with arguments, arguments are always passed *by value*.
This means that the value you pass to the function is copied, so if you pass a variable as an argument to a function, the function parameter is a different variable inside the function body, with the same value as the variable at the call site.

By passing a pointer to the variable, we can access it from inside the function, allowing us to, in some sense, simulate passing by reference.
In the below example, we pass the quotient and remainder variables by reference, so that the function can update both of those variables from inside the function.

https://github.com/CIS1900/2022-fall/blob/26cc1f911eb59684b41d9e2e58d300ba443f4cb4/01/remainder.c#L1-L20

The second reason deals with memory allocation.
Sometimes, regular variables allocated on the stack are not sufficient.
We may not know how much memory we need until the program is run, or we may want data held in a variable to outlive the current function.
Neither of these are possible with regular, stack-allocated variables.

A variable always contains one piece of data of that type.
We do not have the option of not having any data at all, or of having many pieces of data using an array of arbitrary length.
When declaring an array, the size of the array must be known at compile-time (not true in all version of C, but is true in all versions of C++).
Instead, we can use `malloc` to allocate some memory to use as an array, for any size that we provide.
The downside is that we are now in charge of the memory, and need to `free` it ourselves.
These operations use pointers to keep track of that chunk of memory.
Furthermore, by using pointers to represent the memory location of our data, we can represent having no data at all using a `NULL` pointer.

Another issue with regular stack variables is that they strictly obey lexical scope.
When a variable goes out of scope, its data is automatically deallocated later, and it is no longer allowed to refer to that piece of memory.
Sometimes, this is undesireable, and we want memory that stays alive for as long as we want.
Manually allocated memory using `malloc` has this property, which is another use of pointers.

https://github.com/CIS1900/2022-fall/blob/26cc1f911eb59684b41d9e2e58d300ba443f4cb4/01/malloc.c#L1-L33

### `const`

`const` is a modifier you can add to types, which basically says that you cannot change values of that type.
When it comes to non-pointer types, `const` is fairly straightforward.
`const int` and `int const` mean the same thing, though `const int` is more common.
With pointers it becomes a little more complex.
`const` applies to the type to the left of it, so `const int *` is a pointer to a `const int`.
This means the pointer itself can be modified to point to another address, as long as it points to another `const int`.
To make the pointer `const`, we need to write it so that `const` applied to the pointer portion.
`const int * const` does the trick, declaring a `const` pointer which points to `const int`s.
This is a pointer, that once initialized, cannot be changed to point to something else.
The thing it points to is also `const`, so operations that change it, like `(*p)++`, are also disallowed.

In general, reading the type from right to left helps with understanding which `const` applies to which part of the type.

### Common pointer bugs

Pointers, though very useful, are a huge source of bugs.

One common bug is a null pointer dereference, where you dereference a pointer that you expect points to some memory, but it is instead `NULL`.

Other similar bugs involving dereferencing invalid memory include out-of-bound array accesses, where you access invalid memory outside the bounds of an array, dereferencing uninitialized pointers, and dereferencing pointers to memory which has been deallocated.
This last possibility, using pointers to deallocated memory, can include stack-allocated memory, which was deallocated automatically when it went out of scope, or heap-allocated memory, which the program deallocated manually using `free`.

Finally, memory leaks are another common bug involving memory.
A memory leak occurs when memory you allocated using `malloc` is not `free`d, resulting in your program using more and more memory as the program runs.
Eventually, your computer becomes unusable since it runs out of memory, and your program must be restarted.
Memory leaks typically do not result in your program crashing, but can be disasterous for long-running programs.

Many of these bugs will result in a *segmentation fault*, or a *segfault*.
This is the operating system telling you that your program accessed memory it shouldn't have, and it stopped it from potentially harming other running programs.

Like many C bugs, making these errors result in *undefined behavior*, which means that the C standard does not describe what the compiled program must do.
This means that while often your program will crash with a segfault if you make one of these errors, it is very possible that it does not crash.
Such a program is legal output from the compiler, since it obeys the rules of the C standard.
This sort of behavior is much scarier than your program crashing, since a program that works on your computer may break the next time you run it, or when you compile with another compiler or run on another computer.

### Strings

Strings are implemented as arrays of `char`s in C.
In addition to the `char`s that make up the string, there also needs to be an extra `char` at the end of the string: the `0` value or `'\0'` if you write it as a `char` with single quotes.
This means that if you are declaring an array, you need one more element than the length of your string in order to store it properly.

When you write a *string literal* in your code like `"Hello, world!\n"`, the program contains that string in a read-only location in your program, so you don't have to deal with managing that memory.
If you declare and initialize a string like `char * str = "hello";`, this points directly to that read-only string, and it is undefined behavior to modify it (declaring it as a `const char *` would be better!).
Alternatively, if you declare a string like `char str[] = "hello";`, this declares an array and *copies* the contents of the string literal to this array.
Modifying this array is perfectly fine and does not result in undefined behavior.

`<string.h>` includes many useful functions for dealing with strings, like `strlen`, which returns the length of the null-terminated string.

## Debugging

### `gdb`

`gdb` is a *debugger* which allows you to inspect what your programs are doing.

Consider the following program:

https://github.com/CIS1900/2022-fall/blob/26cc1f911eb59684b41d9e2e58d300ba443f4cb4/01/debug.c#L1-L21

Once we compile this program (with the `-g` flag!), we can use `gdb` to load (TODO) the executable using `gdb debug` if `debug` is the name of the executable.
This will start `gdb`, and you can then start running the program by typing `run`.
By default, the program will run until it returns or something unexpected happens.
In this case, what (probably) happens is that the program crashes with a segmentation fault, since we try to dereference a null pointer.
Once that happens, `gdb` tells us where the crash occured, and some information about the state at that point.
For more information, we can type `backtrace` or `bt` to get a call stack, with information about previous function calls.
We can also type `print depth` to print the value of the variable `depth` at the current context.
This works for any variable that is currently in scope.

`gdb` currently only stops to let you enter commands when it crashes.
To make it stop at other points, you can type `break debug.c:15` to have it stop whenever it reaches line at in `debug.c`.
To let the program continue running, you can type `continue` or `c`.

Some other useful `gdb` commands include:

- `file program` to load an executable named `program`.
You can use this instead of passing the program name as a command line argument to `gdb`.
- `list` which prints the source code around the current line.
- `step` which runs the next line of code, and stops again.
If the current line is a function call, it steps *into* the function call.
- `next` which is similar to `step`, but steps *over` any function calls.
- `watch var` which stops each time the variable `var` changes.

There are many other features which you can find in the `gdb` manfile (using `man gdb`) or online.
There are also likely ways to integrate `gdb` with your favorite IDE or editor, so you can debug directly in the source code editor.

### `valgrind`

`valgrind` is a tool that checks for memory leaks.
Simply run `valgrind ./program` where `program` is your executable, and it will run your executable as normal.
Once the executable terminates, it will give you a report of memory leaks.

For example, try compiling the following program which exhibits a memory leak, and see what `valgrind` tells you:

https://github.com/CIS1900/2022-fall/blob/26cc1f911eb59684b41d9e2e58d300ba443f4cb4/01/leak.c#L1-L6
