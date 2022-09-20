# Class 3

In these notes we will see how C++ handles input/output using *stream* objects.

## Standard Streams

Standard streams are an abstraction that generalizes communication with its execution environment.
There are three standard (std) streams, stdin, stdout, and stderr.
By default, these are directed to the terminal and result in input from the keyboard and output to the terminal, but these can be redirected to other programs or files.

stdin represents input, and comes from the keyboard by default, which is read when you press enter.
We can also redirect input to come from a file, for example using `./program < in.txt` which uses the contents of `in.txt` for stdin, instead of the keyboard.

stdout represents output, and goes to the terminal by default.
stderr represents error messages, and also goes to the terminal by default.
While by default these two sources would be interspersed in the terminal, we can also redirect them.
For example, we could redirect strerr to store errors to a log file.
To redirect stdout you would do `./program > out.txt`, and to redirect stderr, you would use `./program 2> err.txt`.

See the [wikipedia article](https://en.wikipedia.org/wiki/Standard_streams) for more detailed information.

## Input/Output in C++ via Standard Streams

C++ also provides objects called streams which are used for input/output.
The first ones we will talk about are streams which interact with the standard streams.

### Output

We have already seen the C++ stream that prints to stdout: `std::cout`.
`std::cout` is an *object* of type `std::ostream`.
For the rest of the file we will drop the `std` namespace for convenience.
A second stream object that C++ provides in `iostream` is `cerr`, which is also an `ostream` that prints to stderr.

The primary operation that `ostream`s provide is `<<`, the operator which prints something to a stream.
As mentioned in last week's notes, this works via operator overloading, where the usual bit shift operation `<<` is made to have different functionality when used on `ostream`s.
We also saw that we can chain multiple things to be printed.
This works because of two things: the fact that `<<` is left associative and that it returns a reference.

Left associativity means that `a << b << c` is interpreted as `(a << b) << c`, and not `a << (b << c)`.
If it were interpreted in the second way, then `cout << 2 << 1` would be `cout << (2 << 1)` and print 4, rather than the 21 that we expect.

`<<` should be thought of as a function that is just called in a different way.
Its first argument is an `ostream &`, and it has overloads for many different types as its second argument, like `int` and `string`.
The return value is an `ostream &`, returning the same reference that was passed in.
Unlike our example from class 2, this is safe and commonly done, since we are not creating a new thing and returning a reference to it.
Rather, we are just giving back the same reference that was passed in, which should still be active, since it is "owned" by something outside the function call.
This enables us to chain the `<<` applications, since `cout << 2 << 1` first prints the 2, and returns `cout` again, effectively becoming `cout << 1`, and the `1` can then be printed.

In addition to just printing, you can also format how certain things are printed, using *I/O manipulators*, which change the internal settings of streams.
Documentation for the various manipulators can be found [here](https://en.cppreference.com/w/cpp/io/manip).
For example, `cout << scientific << 1.1` will set `cout` to print `double`s and `float`s in scientific notation.

### Buffering

While not an inherent feature of stdout and stderr, many languages *buffer* stdout and do not buffer stderr.
This means that when you print to stdout it is not written immediately to the screen, but stored in a buffer until there is a good amount of text to write (typically a line of text).
Conversely, when printing to stderr, it is written immediately no matter how much text there is.
Buffered output is better for performance, however if your program crashes then it might lose some output that is in the buffer, but not printed yet.
For this reason, strerr is not buffered so no error messages are lost in this way.
An example of this can be seen by the below program:

https://github.com/CIS1900/2022-fall/blob/0fe55459910397972ac07aba22f14d9240c61247/03/buffered.cpp#L1-L21

This example uses `flush()`, a *member function* (what we call methods in C++) on `ostream`s to manually flush the buffer, forcing the print to stdout to occur.
It also uses `endl`, which represents printing a newline to the stream and then flushing its buffer.

The unbuffered nature of `cerr` is not always desirable, since it is less performant.
For this reason, C++ provides another `ostream` that prints to `stderr` but *is* buffered, called `clog`.
As its name suggests, it is meant for printing large amounts of text to `stderr`, like logging output.

Because of the difference in buffering behavior between the different stream objects, the terminal output may be slightly different from what you expect when using a combination of `cout`, `cerr`, and `clog`.

### Input

Input streams are objects of the type `istream`.
Unlike in C, where using functions like `gets` and `scanf` to read strings can be unsafe due to buffer overflows, `istream`s are safe, handling buffers and other details for you.

`cin` is the `istream` provided that reads from stdin.
The main operation that `istream`s provide is `>>`, or "get from", which reads data from stdin into a variable that you provide, like `cin >> i`.
Just like with `<<`, these operations can be chained together.
You can remember the `<<` and `>>` operations as pointing in the direction of the data.
For `ostream`s, the data goes into the stream to be printed, and for `istream`s, the data goes from the stream into the variable.

`>>` reads (skipping any leading whitespace) until it encounters something unexpected, like whitespace or a newline.
This could also be something like a letter when you are reading an `int`.
For example, if I am reading an `int` using `cin >> i` and I type in `1a`, `1` will be read into `i` and `cin` will stop there.
Later on you can read the `a` separately by reading a `char` or a `string` (more on `string`s after the example).

https://github.com/CIS1900/2022-fall/blob/0fe55459910397972ac07aba22f14d9240c61247/03/cin.cpp#L1-L13

A useful type that we use here is `string`, a much safer alternative to using C strings (`char` arrays).
`string`s automatically resize and can be easily appended using `+`, as with most other languages.
This is crucial, since otherwise we would have to worry about buffer overflows if we were reading input into C strings.

To avoid `>>`'s behavior of skipping whitespace, there is also the `getline` function, which takes an `istream` and the string variable to read into.
This function reads until it encounters a newline, and also returns a reference to the first argument, just like with `>>`.

Streams can also be implicitly converted to `bool`, so we can do things like `if (cin)`.
This is mostly useful for `istream`s, which will convert to `false` if they are in a failure state.
The most common way this can happen if they have reached the end of input by encountering the end of file character (the actual end of file if reading from a file, or ctrl-D if you are entering input on the command line).
Alternatively, you could also call the `fail()` member function to check if the implicit conversion returns `false` (e.g. `if (!cin.fail())` is equivalent to `if (cin)`).
Using these features together, we can write programs that read all of the input:

https://github.com/CIS1900/2022-fall/blob/0fe55459910397972ac07aba22f14d9240c61247/03/readall.cpp#L1-L12

## Other Types of Streams

While `cout`, `cerr`, `clog`, and `cin` are predefined `ostream`s and `istream`s that deal with the standard streams, we can also create other streams that read and write to other things: files and `string`s.

### File Streams

File streams allow us to read from and write to a file.
These declarations are found in the `<fstream>` header.

For input, we have the `ifstream` type which inherits from `istream`.
For output there is the `ofstream` type which inherits from `ostream`.
When creating an object of these classes, it can be initialized with a file name (e.g. `ifstream ifs {"in.txt"};`), or the file to open can be specified later using the member function `open()` (e.g. `ifstream ifs; ... ifs.open("in.txt");`).
Both of these examples call a constructor of `ifstream`, the first of which takes one argument and second of which takes no arguments.
When the file stream object goes out of scope, it cleans up the file handle, so there is usually no need to call the `close` member function, though one is available.

Apart from these ways of associating the stream with a file, these streams are subclasses of `istream` and `ostream`, so they can be used in the same way as the streams we have already seen:

https://github.com/CIS1900/2022-fall/blob/0fe55459910397972ac07aba22f14d9240c61247/03/fstream.cpp#L1-L25

Since these streams need to open files, there are also new ways the streams can fail, as shown in the example.

Finally, there is also an `fstream` type which can do both input and output, which inherits from `iostream`.
`iostream` is a class which inherits from both `istream` and `ostream`, allowing one to both read from and write to it.

### String Streams

String streams allow us to read from and write to a `string`.
This can be useful if you need to read/write a string from/to some other source, like over the network, from a GUI, or from using `getline`.
These declarations are found in the `<sstream>` header.

For input, we have the `istringstream` type which inherits from `istream`, and for output there is the `ostringstream` type which inherits from `ostream`.
When declaring an `istringstream`, it can be initialized with the string to read (e.g. `istringstream iss {"input string"};`) or set later using the `str()` member function (e.g. `iss.str("input string");`).
A string stream's underlying string can also be obtained using an overload of `str()` (e.g. `cout << oss.str()`).

https://github.com/CIS1900/2022-fall/blob/0fe55459910397972ac07aba22f14d9240c61247/03/sstream.cpp#L1-L16

Just as with `fstream`, the `stringstream` type allows you to both read and write, and inherits from `iostream`.

## Polymorphism

To summarize, the stream types we have described form the inheritance diagram (with slightly different names which are used in the implementation of these classes) shown at [this link](https://en.cppreference.com/w/cpp/io).

With this inheritance hierarchy, we can use polymorphism to operate over *any* `istream` or `ostream`.
This is because any `istream`, for example, supports the same operations, no matter if it is `cin` or some `fstream` object.

https://github.com/CIS1900/2022-fall/blob/0fe55459910397972ac07aba22f14d9240c61247/03/polymorphism.cpp#L1-L28

One important thing to keep in mind is that like everywhere else in C++, arguments are passed by value and copied by default.
This means that if your `log` function takes an `ostream` argument by value, not by reference, then when you call it a copy of the argument will be made and passed to the function.
This is fine if you call `log` with an `ostream` like `cout`, but if you call it with a `ofstream`, then this will take the `ofstream` object and copy it into a `ostream` object, removing all the extra parts that are defined in `ofstream`.
This is known as *object slicing*.
This means that the new object no longer behaves like a file stream, but rather as a "plain" `ostream`, and things will likely go wrong.
In this example, trying to write this `log` function will not work since `ostream`s are not able to be copied, since it typically does not make sense to duplicate streams, since they are designed to hold exclusive access over the underlying resource, like access to stdout or the file handle it is reading from.

Typically you want to have your objects behave polymorphically, and to do this you need to pass by reference or by pointer, which does not cause the underlying object to be copied.
