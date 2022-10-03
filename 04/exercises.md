## Exercise 1

The internet is full of bad sources.
For example, this [GeeksforGeeks](https://www.geeksforgeeks.org/inheritance-in-java/) tutorial about inheritance talks about how inheritance should model "IS-A" relationships, and then proceeds to define the following class hierarchy (in Java):

```
public class SolarSystem {
}
public class Earth extends SolarSystem {
}
public class Mars extends SolarSystem {
}
public class Moon extends Earth {
}
```

Discuss with your partner and table about how you would change the class hierarchy, and if you'd introduce any new classes.
Always evaluate your sources!

## Exercise 2

Take the [inheritance example](https://github.com/CIS1900/2022-fall/blob/main/04/inheritance.cpp) from the notes and convert the `animal` and `dog` classes to be in separate files.
You should create 4 files, `animal.hpp`, `animal.cpp`, `dog.hpp`, and `dog.cpp`.
You should also test these using another file with a `main` function.
