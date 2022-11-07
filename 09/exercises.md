## Exercise 1: Using iterators

Write a function `void edit(vector<int> &)` which replaces all occurences of `0` in the input `vector` to two occurences of `0`, and removes all occurences of `1`.
For example, if the input is a vector holding `1, 2, 3, 0, 0` then after running `edit` it should be `2, 3, 0, 0, 0, 0`.

Use iterators and the `.insert` (or `.emplace`) and `.erase` member functions, rather than making a second `vector`.

## Exercise 2: Generalizing to `list`

Write a function `void edit(list<int> &)` to do the same thing for `list`s.
