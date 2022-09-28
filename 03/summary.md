# Class 3 summary

Questions from the Canvas quiz:
- Redirection in C vs C++: redirecting output is handled by the shell, it is completely independent of the programming language. Any executable's input/output can be redirected.
- Standard streams vs C++ streams: standard streams (stdin, stdout, stderr) are part of the shell. C++ streams are the mechanism that C++ provides to interact with the standard streams.
- Using an input stream as a boolean, does it read over some data (e.g. `if (cin)`): No, doing this does not read anything from the input source or move forward in the input. We will only "move" in the stream if we explicitly ask the stream to do so, using `>>` or `getline`, for example.
- Moving backwards to previous input already read by an `istream`: not always possible. e.g. for `cin`, you would have to store all the user input yourself, nobody else is doing it for you. But for a file stream you could go back to the beginning of the file. See the [documentation](https://en.cppreference.com/w/cpp/io/basic_istream) for some functions that let you move around in the input.
- `endl` vs `flush()`: `endl` also inserts a newline into the `ostream`, which may not be desirable.
- Buffering behavior: typically you can expect the buffer to be printed when you reach a newline. But if your program depends this much on buffering behavior you should really flush the buffer manually.
- Good practice to call `flush()`: you typically would not need to think too much about buffering, so you would not call `flush()` too often.
- Mutable strings: C++ `string`s are mutable. This mirrors the underlying implementation of `char` arrays and is a pretty natural choice when implementing a string class. The more interesting question is why do other langauges make strings immutable, which I don't really have much insight on. [Here](https://stackoverflow.com/questions/22397861/why-is-string-immutable-in-java) is an interesting post on SO.
- `string` efficiency: `string` is fairly efficient, similar to an array. The implementation is similar to that of Java's `ArrayList`, doubling in size.
- Associated operations in C: In C you would use file handles (`FILE*`, with functions `fopen` and `fclose`) with functions like `fprintf` and `fscanf`. You can specify stdin or stderr as special file handles. For strings, there are functions like `sprintf` and `sscanf`.
- Multiple inheritance in C++: It is allowed, but can be complicated. You should probably avoid it unless you need it and you really know what you're doing.
For more details see this [opinionated FAQ answer](https://isocpp.org/wiki/faq/multiple-inheritance)
- Why doesn't `fstream` inherit from `ifstream` and `ofstream`: Probably to avoid the diamond problem in multiple inheritance (since `istream` and `ostream` have a common ancestor), but there is no technical reason why you couldn't.
- Why use classes that can only read or write when you could use one that does both: We typically want to use the least powerful tool possible, so that if things go wrong, the least harm is done. Reading and writing at the same time is relatively rare for most use cases, so we can typically get away with just using the input or output stream.
- Why use stringstreams, is it to cast other types into strings: This is one possible use for `ostringstream`, though as we'll see in next week's notes that typically we just print objects to a stream directly, without converting to a `string` first. String streams are generally useful when you need to parse a string that you already have, or to create a custom string.
- Defining your own streams: It is generally not recommended to inherit from standard library classes. **Correction from class**: streams can be inherited from relatively easily, and a whole [book](http://www.angelikalanger.com/iostreams.html) has been written on the topic. It would require a lot of technical work with the internals of streams, much more than what we have talked about in the notes.
- Can we always find a "safe" alternative to returning by reference: generally it's always possible, but there's always a tradeoff between value and time spent. It's best to try to code safely from the start.
- Can anyone overload operators or is it only the standard library: you can also overload operators, as long as they're already in the language (i.e. you can't make up new operators).
- is `cout` is a literal: a literal is a value you can write directly in the code, like `1` or `'c'` or `"hello"`. `cout` is an object, and though there is only one copy of it, it is not a literal. Objects cannot generally be literals since they must be created and manipulated, you cannot just "write down" an object directly in the code like you can with built-in types like `int` or `char *`.
- streams vs Java streams: C++ streams are really meant just for printing. Java streams are meant more for infinite streams of data of any time, and you can apply operations on them.
- Polymorphism and object slicing: As an example, consider a class hierarchy `Animal` and a subclass `Cat`. A `Cat` object can be thought of as an `Animal` object plus some additional members, like new functions and variables, plus things like overriden functions.
If we call by value, then the `Cat` object is copied into an `Animal`, "slicing" off those additional members. This means that any overriden functions will just call the default `Animal` version, effectively erasing everything that makes the `Cat` object different from an `Animal`. Changing the function argument to a reference or pointer eliminates this copy and the slicing issue.

Exercise 1 was a warm up to the assignment.
`>>` will ignore any amount of whitespace between 2 reads.
If the input is of an unexpected type, the stream will go into an error state, meaning that the implicit conversion will return false, and `fail()` will return true.

Then we started on the assignment.
Two small "hints":
1. you don't need to google too much, you can do it with just stuff we've covered in class
2. do you need to store every line? it might be simpler to process it one line at a time.
