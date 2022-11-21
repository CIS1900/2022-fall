# Libraries, applications, and tools for C++

C++ is a general purpose language, so basically everything is doable and well-supported, but may be difficult.
There is a very large list of tools and libraries [here](https://en.cppreference.com/w/cpp/links/libs).

## Build systems and package managers

C++ lacks a central tool to install libraries or compile projects, so you may want to choose one of the build systems in the list linked above.
If you aren't using too many weird libraries, it should be straightforward to compile with a simple Makefile.
[CMake](https://cmake.org/) is a popular tool that _generates_ build files for different environments, like Makefiles, Visual Studio projects, or other targets, using one file.
While this is very useful for cross-platform development, it's probably overkill for the final project.

## Frameworks

The following are less libraries and more frameworks that change the way you need to code.
Many of these are very old, and therefore predate some more recent features of C++, and instead build their own standard libraries (like their own string classes!).
I recommend against these due to the high learning curve unless there's something specific you want to do with one that you can't do otherwise.

- Game engines, like [Unreal engine](https://www.unrealengine.com/en-US/).
  - Most handle every aspect of creating a game, like game assets and animations.
  - Will require lots of non-coding work, so I strongly recommend against this.
- [Qt](https://www.qt.io/) for cross-platform GUIs. It includes most things you would need for writing applications, like networking, databases, and I/O.

## Libraries

When compiling with libraries, you likely need to add the `-L[path to compiled library files]` to tell the linker where to find the library, the `-l[name of library]` flag to tell the linker to link the library files, and the `-I[path to library headers]` flag to tell the compiler where to find the library's header files.

- [Boost](https://www.boost.org/) contains many libraries, all of which are peer-reviewed and designed to work well with the standard library.
  - [Asio](https://www.boost.org/doc/libs/1_77_0/doc/html/boost_asio.html) is a go-to library for doing low-level networking.
  - Many standard library features started off as libraries in Boost.

- For 3D graphics, the lowest-level options are [Direct3D (for Windows)](https://docs.microsoft.com/en-us/windows/win32/direct3d) and [OpenGL (cross-platform)](https://www.opengl.org//). [Ogre3D](https://www.ogre3d.org/) is a higher-level option.

- The above options can also be used for 2D graphics, but for a higher-level option [SFML](https://www.sfml-dev.org/index.php) is a good choice. [SDL](https://www.libsdl.org/) is a similar option but is a C library.

- If you want a simpler user interface, there is also the option of [ncurses](https://invisible-island.net/ncurses/) library for writing text-based terminal UIs.

- [OpenCV](https://opencv.org/) is a popular computer vision library written in C++.

- For testing, we use [Catch2](https://github.com/catchorg/Catch2) for unit testing in the autograder, but there are other options such as the testing library in Boost.

## Other uses of C++

Other areas where C++ is popular, aside from the ones mentioned above (but are not really suitable for a final project):
- Low-level applications, like embedded programming and operating systems. OSX and Windows are at last partly written in C++.
- Applications that require high performance, like browsers, databases, and distributed systems.
