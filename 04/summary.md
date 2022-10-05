# Class 4 summary


Questions from the Canvas quiz:
- Why abstract classes: Abstract classes let you write a function without a definition and restrict creating objects. It often doesn't make sense to let users make an object of an abstract class like `animal`, and to declare a function that isn't defined for that abstract class.
- Using abstract class to define an interface: I use the term interface informally, there aren't formal interfaces in the language as in Java. An abstract base class should have all the functions to be implemented, so that forms an interface for the user to use.
- Abstract classes vs interfaces in Java: I don't know that much about Java, but I think the only (small) difference is that pure virtual functions *can* have an implementation when they're first declared in the base class. Derived classes can then use this (using `baseclass::functionname(...)`) in their override, but they still have to override it to become a concrete class. Java abstract methods can't do this (I think).
- Specifics of virtual methods: [here](https://stackoverflow.com/questions/2391679/why-do-we-need-virtual-functions-in-c?rq=1) is a relevant stackoverflow question.
- Syntax for multiple inheritance: `class A : public B, public C { ... };`
- When to use `class` vs `struct`, and why do both exist: Mostly for backwards compatibility. You should use a `struct` if you are only trying to package some variables together, and a `class` for anything else.
- Why delete a default constructor like `point() = delete;`: to be more explicit when the default constructor already does not exist, or you can delete it and disable construction entirely. This is not very useful but in next week's notes we'll see things that might make sense to disable.
- Write default constructor or use implictly defined one: Use the implicitly defined one if you can (if you don't need to do any custom initialization logic).
- Class definition in multiple translation units are allowed as long as they're the "same", what does "same" mean here: It means character-by-character the same. These multiple class definitions typically come from the same header `#include`d multiple times, so this isn't usually an issue.
- Going over separate compilation: I summarized the typical process, and showed the following demo.
You can try to compile using the -c flag to *only* compile, not link. This will produce a `.o` object file.
Then you need to link your object files together, for which you could run `ld` manually, but you would also have to figure out how to link the standard libraries.
An easier way is to just compile using `g++` and specifying your object files, which will link them together plus standard libraries.
- Overwriting reserved methods in C++: Many fundamental ones about copying and cleanup will be covered next class! The question mentioned double underscore functions in Python, and some functionality like that, like comparisons functions or arithmetic operations can be done via overloads. Some others like hashcodes we will see later on.
- `#pragma once`: basically the same as include guards, but not standard. Many compilers support it, though.
- `const` at end of function declaration vs beginning: `const` at the beginning signifies that the return value is a `const`. Putting it at the end is very different, it signifies that the function itself does not change the object.
- Are private variables accessible in the current object only, or the class (e.g. why does `point6.cpp` let + see `p.x` and `p.y`?): private just means that only member functions and friend functions of the current *class* (not object!) can see the private members.
So even if we are trying to access a different object's members, we can since it is an object of the same class.
- Organizing large C++ projects: Mostly the same. One thing you can do to speed up compilation times is to reduce dependencies between files, so if you change a header not much else needs to be recompiled. One way of doing this is forward references, where you can remove an `#include` if you only use pointers or references in the header. See [this](https://stackoverflow.com/a/553869/3776956) stackoverflow answer for details.
Yiyun also mentioned the [pimpl (pointer to implementation) idiom](https://en.cppreference.com/w/cpp/language/pimpl), which is used to hide the implementation details of a class from its header. This can also reduce dependencies between files.
- What if you don't override a `virtual` function: The function inherits the base class's version.
- `this->x`, why is `this` a pointer vs reference, and when do we need to use it: `p->x` is shorthand for `(*p).x`, and can also be used on member functions. `this` does not need to be used typically for referring to member variables, and the only major use is for disambiguating between a member variable and a local variable with the same name. There is not really a big reason for having it as a pointer. I suspect it's because it predates the introduction of references in C++.
- Going over question 3 (non-virtual overriden function) in the quiz: If the function is not virtual, then what function gets called is determined at compile time. Since it is accessed through a base class reference, it will call the base class's `pet` function.
- Construct an object by providing an object of a subclass as in question 3: Note that we did not construct a second object. The `animal` variable is a *reference*, and it references the original dog object.
- `#define`: `#define` is typically used to define text-expansion macros, like `#define TRUE 1`, then being able to use `TRUE` in your code.
For include guards, we just define the name and make it expand to nothing.
The name is then used in the `#ifndef` to see if it's been defined yet.

Exercise 1: there are lot of issues with this.
Since inheritance is supposed to model "is-a", this would mean that the Earth and Mars are both solar systems, and that the Moon is an Earth, and transitively is also a solar system.
The relationship being modeled here by inheritance is more of ownership; the solar system has an Earth and a Mars planet, and the Earth has the Moon.
Has-a is more effectively modeled by "composition", which is just having an object of that class as a member variable.
For inheritance purposes, it might be nice to have a base class called something like `Body` to represent a celestial body.
Then Earth, Mars, and the Moon could all inherit from this class, and the solar system could have a collection of `Body`s.

Exercise 2: [here](exercise/) are the converted files.
Remember to move member function definitions into the `.cpp` (remember the `classname::` in front of the name of the function!), and to add include guards to the headers.
Remember also that you shouldn't use `using` in the headers, because it will force anyone `#include`ing your header to use them too.
