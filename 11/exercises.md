## Exercise 1

In assignment 2, you implemented a class hierarchy for `shape`, `triangle`, and `square`.
To add a new kind of shape, one must write a new subclass of `shape` and write its custom behavior in this class.
This requires every shape to be defined statically in your code.

An alternative is to store an `std::function` for the custom behavior (in this case, calculating the area), and customize that on a per-object basis.
This is closer to what one would do in a functional language.
This also allows different shapes to be created *dynamically*, where the area calculation is supplied at runtime.

Starting from [`shape.cpp`](shape.cpp) and [`shape.hpp`](shape.hpp), modify the class so that the behavior of its `getArea` can be customized.
