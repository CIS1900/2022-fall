# Accessing Required Software

For grading purposes, we will grade your code using the GCC C++ compiler.
The `valgrind` tool is also useful for checking if your code has memory leaks.
This document has instructions for installing the compiler (usually invoked by `g++`) and `valgrind`.
Since we use C++20 features, we need a relatively new version of `g++`: 10 or above.

While you can use other compilers like Clang (the default on Mac), behavior and command line arguments may differ slightly from what we do in the notes and in class.

## Using SEAS servers

You can also use the SEAS server for access to a Unix command line.
`g++` version 10.4.0 is installed and can be run with `g++-10`.
`valgrind` is also available.
Instructions for remotely accessing the server are [here](https://cets.seas.upenn.edu/answers/remote.html).
You can also use one of the [Linux labs](https://cets.seas.upenn.edu/answers/big-picture.html).

## Linux

If you are on linux, install `valgrind` and `g++` version 10 or above through your package manager.
Depending on how it is installed, it might have to be run as `g++-10` (or whatever version you installed).
Be sure to test the version you installed using `g++ --version` (or `g++-10 --version`).

## Windows

While you can install GCC natively on Windows, a far easier solution is to use the [Windows Subsystem for Linux](https://docs.microsoft.com/en-us/windows/wsl/), which gives you a Linux environment directly on Windows.
Installation instructions are [here](https://docs.microsoft.com/en-us/windows/wsl/install).
The easiest choice is the Ubuntu Linux distribution, where then you can then follow the Linux instructions above.
Specifically, you can run `sudo apt install valgrind` and `sudo apt install g++-10` to install those two tools.

## Mac

`valgrind` is not supported on newer versions of macOS.
To use `valgrind`, you should use the SEAS servers or install a Linux virtual machine.
There is also an [unofficial port](https://github.com/LouisBrunner/valgrind-macos) of `valgrind` that you can install using Homebrew, but I cannot guarantee that it works.

By default, `g++` on a Mac defaults to `clang++`.
To install the real GCC compiler, the easiest way is to use Homebrew.
After installing Homebrew, running `brew install gcc` should install GCC version 12.
Then you should be able to run the C++ compiler using `g++-12`.
Note that if you have an M1 Mac, only GCC 12 will work since previous versions do not support the M1 architecture.
