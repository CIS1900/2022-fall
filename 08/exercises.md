## Exercise 1

Consider [this code](mutex_ex.cpp) which adds up the numbers between 1 and 50 million twice.
We are trying to use threads to speed it up, but since we need a `mutex` to protect the global variable, this actually ends up being slower than the [sequential code](seq_ex.cpp).
You can time how long it takes to run a program using `time ./a.out`.
On my machine, the `mutex` version takes 8.7 seconds, and the sequential version takes 0.27 seconds.
These programs also overflow, but we are only interested in the speed for this example.

What alternative would work better for speeding up this code (see in-class poll)?
Write the program that uses this alternative, and check how fast it runs.

## Exercise 2

Consider [this code](add_ex.cpp) which has some threads which produce numbers to push to a shared queue, and some threads which add up numbers in the queue.
The adder threads can only add if there are two values in the queue.
What synchronization method can you use here (see in-class poll)?
Update the program so that it runs properly.
Changing the number of producer and adder threads should not cause any issues.
