## Exercise 1: string append

Write a function
```c
char * append(const char * const first, const char * const second);
```
which appends 2 strings.

This function should return a new string which contains the string `first` followed by `second`.
Take care that the returned string is valid (i.e. null-terminated).
Functions in the string library found at `string.h` may be useful.
Documentation for these functions can be found [here](https://en.cppreference.com/w/cpp/header/cstring).

## Exercise 2: pointers to pointers

With your table, discuss when you might need to use a pointer to a pointer.
Remember the two main uses of pointers, and how they might apply here.

After we discuss about possible use cases, write a function that performs the task you chose using pointers to pointers.

## Exercise 3: debugging exercise

Compile and run [`exercise.c`](exercise.c).
Ignore the bugs in the code and the compiler warnings for now.

Try using `gdb` to see if you can find the bugs in the code.
What is the state of the program when it crashes for the first time?

Once you fix that, there will be a second bug.
This crash occurs in a strange place.
How can you see what caused your program to arrive at this location?
