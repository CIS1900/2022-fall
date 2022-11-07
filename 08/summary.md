# Class 8 summary

Questions from canvas that I answered in class:
- Applications (in industry, data related applications), and reliability: Concurrency is very common in modern programming. [Moore's law](https://en.wikipedia.org/wiki/Moore%27s_law) is slowing down and the main alternative method to improve performance is by using multiple processors, rather than improving each individual processor.
So to take advantage of these hardware features, we need to use concurrency.
Concurrent programming is also very difficult. The nature of race conditions means that they can occur extremely rarely, since they depend on the exact order of instructions.
One high-profile is the [2003 northeast blackout](https://en.wikipedia.org/wiki/Northeast_blackout_of_2003#Causes), which was caused by a race condition in the alarm and error software for the power company, causing the alarm systems to trigger silently and the power outage to get out of control.
Multiple threads wrote to the same data structure which corrupted it, leading to the alarms failing.
This race condition was extremely rare, and the software had run for over 3 million hours (total) without issue.
To cap it all off, this happened in a C codebase of over four million lines.
In general, testing-based approaches are not sufficient for concurrent code, since there are exponentially more possible executions, since different threads can run in different orders.
So very careful design and reasoning is required to write concurrent code that works.
There are also some tools that can help find bugs, like *model checkers*.
For data-related applications, concurrency can be very useful for speeding up your code.
Often your code for processing data runs on independent pieces of the data, so no synchronization (or at least nothing complex) is needed.
- Detect race conditions/deadlocks: This is very difficult to do in general. I would say think about what shared resources each thread is accessing. If any of those accesses are writing to the resource, you will probably have a race condition. For deadlocks, the main cause is holding a lock while waiting to acquire another one, so if you are using `scoped_lock` to acquire all `mutex`s at the same time you should be ok.
- Why might we use a `barrier`: There may be multiple "stages" to your computation. For example in the first stage you use multiple threads to build a large data structure together, and in the second stage you read from that data structure. It would be useful to make sure all the threads are done modifying the data structure before proceeding to stage 2 in all threads, to avoid data races.
- Waiting using condition variables: I went over the example in the notes. Basically, a condition variable is a method to wait and signal threads waiting. Other synchronization primitives in the notes are all fairly "coarse". `mutex` only prevents multiple threads from being in the same critical section at once. Semaphores let a number of threads in. Condition variables let us wait on any condition we like, and are more flexible.
While answering this question I realized that the `condition_variable.cpp` example code was not very good, since it's better suited to a semaphore, which would allow multiple consumers in to access the `queue` at the same time.
The example has been updated now to use an `optional` shared value, and the `condition_variable` is used to wait until a value is available.
- Why use `promise`s/`future`s, and the differences between them: `promise`/`future` provides a very simple mechanism for synchronization. It allows one thread to wait on data provided by another.
The `promise` is the way to *send* a value, and the `future` is used to *receive* it.
So the two are linked, and you can't have just a `future` on its own (the one that's returned by `async` is kind of associated with an implicit `promise` from the `async`).
- What if you use a variable instead of a `promise`/`future`: The `future` will block until the value from the `promise` is ready. You would have to encode this with a special value or something like that if you used a regular variable, and then you would have to continually loop on that variable, which wastes CPU time.
- `promise`s vs promises in Javascript: I'm not too experienced with Javascript, but it seems quite similar, at least in its motives.
- Use cases of `async`: `async` gives us this `promise`/`future` pair in a very convenient way, when the value we care about is the return value of a function.

Questions I didn't answer in class:
- Using `mutex`s and semaphores, etc. seems difficult. Can libraries do this automatically: Not that I know of. These features *are* very difficult and lead to many bugs. This is why you should stick to the simpler "high-level" concurrency in the notes when possible.
- Can you attach a handler to be executed when a `future` is ready, like `.then` in javascript: Yes, though not directly. You can create another `thread`, `.detach()` it, and have that thread call `.get()` on the `future`. As soon as the `future` is ready, that thread will continue running whatever code you put into its task.
- `packaged_task<int(int)> pt {f};`: We have to specify the type of the function that the `packaged_task` is wrapping.
This is kind of strange, since the compiler should be able to figure it out for us.
It looks like there is a proposed fix for this for C++-23: https://cplusplus.github.io/LWG/issue3117
- Can context switching without parallelism improve performance: I guess it depends what you mean. If there is actual computational work to be done, then no, switching between the two won't help. But if you need to wait (like in the examples with `sleep_for`, or waiting on user input, or a web request, etc.), then you can do something else while waiting, which could improve performance in that sense.
- Why do we need `ref`: It has to do with the fact that calling a function which takes an argument by value vs by reference looks the same, so `ref` tells the compiler we are passing a reference. This is necessary here because of rather complex technical reasons. See this SO link for more details: https://stackoverflow.com/questions/11833070/when-is-the-use-of-stdref-necessary
- When might you use a barrier and latch with semaphores: A `barrier` or `latch` is typically used to make sure all threads make it to the next "stage" before continuing, and semaphores are often used to keep track of how many available values there are in some shared resource.
These two are pretty orthogonal so I could see using both at the same time.
e.g. a multi-stage algorithm where we use a `barrier` to make sure all the threads are done before proceeding, plus a semaphore in one of the stages to manage access to a queue of items.
- How to decide which concurrency features to use: In general (not just for concurrency) you should use the least powerful tools possible to solve your problem. This means trying to use the "higher-level" concurrency features, since there are fewer ways they can go wrong.
- How `mutex` works and what is it: `mutex` stands for mutual exclusion, and it is an object used to enforce that two threads cannot be running the same code at the same time. To enforce this, before the code you should try to lock the `mutex` (which is shared by all the threads), and after the code you would unlock it. The way the `mutex` is implemented, the `mutex` cannot be locked if another `mutex` has already locked it (it will wait there until the `mutex` is unlocked), thus ensuring that only one thread is running the code between the lock and unlock at each point in time.
If it helps you could try thinking about how you would implement a `mutex` on your own. `lock` would check some internal flag that is held, and loop on the state of that flag, only returning once it is able to successfully obtain the lock.
There are some small limitations to this (you need an atomic hardware instruction to do this), but the details are not too important.
- When to use `mutex` & `condition_variable` over `counting_semaphore`: The two are somewhat similar, but also quite different. Both would allow you to wait until a number goes above 0, but `condition_variable` allows for waiting on any arbitrary condition. Orthogonally, `counting_semaphore` could be used to allow multiple threads into the same critical section, which cannot be done with `mutex` and `condition_variable`.
- How `jthread`s work: `jthread`s are basically the same as a regular `thread`, except its destructor calls `.join()`. If this question was asking about threads in general, then `thread` corresponds to an OS-level `thread` (which is why during class, the program crashed when we tried to create too many). For the details of OS threads, I will leave that to your OS class or to your own internet research.