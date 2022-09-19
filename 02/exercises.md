## Exercise 1 from last class: string append

Write a function
```c
char * append(const char * const first, const char * const second);
```
which appends 2 strings.

This function should return a new string which contains the string `first` followed by `second`.
Take care that the returned string is valid (i.e. null-terminated).
Functions in the string library found at `string.h` may be useful.
Documentation for these functions can be found [here](https://en.cppreference.com/w/c/string/byte) (updated link from last class).

## Exercise 2 from last class: debugging exercise

Compile and run [`exercise.c`](exercise.c).
Ignore the bugs in the code and the compiler warnings for now.

Try using `gdb` to see if you can find the bugs in the code.
What is the state of the program when it crashes for the first time?

Once you fix that, there will be a second bug.
This crash occurs in a strange place.
How can you see what caused your program to arrive at this location?

## Exercise 3: Writing simple C++ programs

Take [this C program](simple.c), and convert it to C++ instead using the small features we've covered so far.

## Exercise 4: Returning a reference

Write a function with the signature `int & foo();`.
This is tricky and *strange*.

It may help to think of how you would do this if `foo` were returning a pointer instead.
Does your code cause any issues?
Is writing this sort of code desirable?
