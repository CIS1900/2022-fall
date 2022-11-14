## Exercise 1: Unifying different implementations

Consider the two `edit` functions you wrote last class (the one we did together is [here](../09/exercise.cpp)).
Unify these functions into one using a function template.

Then, look at the [concepts library](https://en.cppreference.com/w/cpp/concepts) and think about which concepts may be useful as type constraints for your template argument.
Are these concepts sufficient to capture everything you need out of the template argument?
Add type constraints to the `edit` function template you wrote.

## Exercise 2: Writing our own `unique_ptr`

We will write a class [`unique_ptr`](unique_ptr.hpp) as a class template.
We will go over the rule of 5 functions together.
Complete the other member functions.
You may want to consult the [documentation](https://en.cppreference.com/w/cpp/memory/unique_ptr) on what they should do.
