# Class 8

In this week's notes we'll go over the concurrency features of the C++ standard library.
Not everyone has seen concurrency before, and this will likely be difficult if it's your first time seeing it.
For that reason, this is during the midterm project week, and there won't be an assignment associated with it.

## Introduction to Concurrency

*Concurrency* in programming is running multiple tasks simultaneously.
This might mean that your code is *multitasking*, or switching between tasks, known as *context switching*.
Alternatively, it may also mean that the tasks are running in *parallel*, or at the same time.
This is only possible if your computer has multiple processors, which most computers do nowadays.

Concurrency has two main benefits:
1. Improved throughput.
If there is some parallelism possible, then we can split work up amongst processors.
2. Improved responsiveness.
If your program must wait for something and there is other work to be done in the meantime, you can proceed with the other work while waiting at the same time.

In these notes, we'll define a *task* as a computuation that can be executed concurrently with other tasks.
Programatically, a task will be a function, or other things that can be called like functions, which we'll see in a future class.
A *thread* is the representation of a running task by the operating system.
Threads can run concurrently, either in parallel or by multitasking.

There is a lot of code in this document, and it will be very useful to try running the programs, which can be much more illustrative than just reading the notes.
To compile the code, you will need to link against the `pthread` library using `-lpthread` at the end of your `g++` command.
This is only the case if you are using GCC or Clang.
If you are using Microsoft Visual C++ on Windows (not `g++` via WSL), then your code should just compile without any additional commands.

## `thread`

A `thread` (in the `<thread>` header) is the standard library class for representing an OS-level thread.
The task associated with the `thread` along with any initial values---the function to run and its arguments---must be passed to the constructor.
As soon as the `thread` is initialized with a task, it starts running (or at least it's possible for it to start running, it depends on how threads are scheduled).
Once a thread is initialized, at some point you must tell it how you want it to finish, by calling either `.join()` or `.detach()`.
`t.join()` will block the current thread, waiting until `t` is finished its task.
`t.detach()` will allow `t` to run independently, without the current thread waiting for it to finish.
If you don't call either, once the `thread` goes out of scope, its destructor will terminate the program, which makes signalling your intent mandatory.
A similar class called `jthread` (for joinable thread) is the same as a `thread`, except it will call `.join()` in its destructor.

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/thread.cpp#L1-L24

The return value of the `thread`'s task is ignored, so we must use a reference to return a value back to the main thread.
However, in this example we must wrap the argument passed by reference with `ref`.
This is due to a technical issue with the variable amount of arguments that the `thread` constructor can take, and is used to tell the compiler that the argument is a reference.

## "Higher-level" Concurrency

This section will discuss methods of writing concurrent code without complex *synchronization*.
Synchronization is controlling the timing of threads, ensuring that they are ordered correctly and work properly.
We will discuss some common synchronization primitives in the next section, but in this section we will focus on simpler code that does not require complex synchronization.

### `async`

The `async` function (in the `<future>` header) allows you to perform a task *asynchronously*, that is, without waiting for the task to finish as with sequential code, and getting the task's return value later.
This can be thought of as separating the function *call* from its *return value*.

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/async.cpp#L1-L30

The return value of the `async` call is a `future`, holding the return value which only exists "in the future".
Once you have the `future`, you can call `.get()` on it to get the return value.
If that return value is not available yet since the asynchronous task is still running, then we will block here, waiting for the value to be ready.
Note that calling `.get()` multiple times is undefined behavior.
You should save the returned value and reuse it, and you can use `.valid()` to check if `.get()` on the `future` is still safe.

So what does `async` do?
It may run the task in another thread, or it may choose to run the task synchronously when the user asks for the return value by calling `.get()`.
This decision is made at runtime, and might depend on the complexity of the task (if it's simple it may not be worth the overhead of the extra thread) or the current availability of OS threads, or other factors.
You can force one of those options to always happen by passing in `std::launch::async` or `std::launch::deferred` as the first argument to `async`.
In the above code, if the `async` call runs asynchronously, then the program should take 1 second, since both threads sleep at the same time.
Alternatively, if the `async` call runs synchronously, then the program will run in one thread, and take 2 seconds overall.

In general, you should use `async` for "simple" tasks that just perform some computation independently and returns a value.
If the task requires shared variables or synchronization, you likely want something more complex than `async`.

`future`s can also propagate exceptions thrown in the task, rethrowing them when `.get()` is called on the `future`:

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/async_exception.cpp#L1-L24

One final thing to note is that `future`s returned by `async` will block in their destructor until the task is finished and the return value is ready.
This means that if you don't store hte returned `future` in a variable, then it will be immediately destroyed and block, essentially making the `async` call synchronous rather than asynchronous:

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/async_sync.cpp#L1-L20

This is specific for `future`s obtained from `async`, and not for the general `future`s in the next section.

### `future`s and `promise`s

More generally, `future`s can represent any result that is provided in the future, not just return values from functions.
This is done using a complementary object called a `promise`, an object representing a "promise" to produce a value later, to be passed to a user through a `future`:

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/promise.cpp#L1-L20

A `promise` has a `future` associated with it, which is obtained using `.get_future()`.
This member function can only be called once for each `promise`, and calling again is undefined behavior.
Once you have the `future`, the `promise` can "send" it a value `v` using `.set_value(v)`.
If something goes wrong and and you want this to be signalled to the `future`, `.set_exception(e)` is also available to throw `e` when `.get()` is called on the `future`.

Sometimes, many threads will want the result of a `future`, but `.get()` can only be called once.
We could take the return value of `.get()` and share it, but then we have to wait until it's ready in a single thread, then distribute afterwards.
More conveniently, a `shared_future` allows multiple `thread`s to wait independently on a result, and can be created from a regular `future` using `.share()` (which also invalidates the original `future`).

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/shared_future.cpp#L1-L29

### `packaged_task`

The final "simple" concurrency class we'll see is `packaged_task`.
A `packaged_task` basically wraps a function with the capability to return a `future`, allowing us to easily transfer the return value or any thrown exceptions to another thread.
Since the object wraps a function, the function's type must be specified in the angle brackets following the `packaged_task` type.

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/packaged_task.cpp#L1-L20

Since the `packaged_task` owns resources (the task and the internal state for managing the `future`), it cannot be copied and must be `move`d.
Using `packaged_task`s and `thread`s can be thought of as a "lower-level" alternative to `async`, where the programmer has more control.
We decide whether the task runs in a separate thread, and when to start it by moving it into the `thread`.

## "Lower-level" Concurrency

The features above are sufficient if the computations are relatively independent and only require some simple communication between threads using `future`s.
For more complex tasks that share variables and have sophisticated ordering requirements, we need more powerful tools for concurrency.

### Concurrency Bugs

Two main classes of bugs that arise as a result of concurrency are race conditions and deadlock.

A race condition is when multiple threads are accessing the same data, and at least one of them is changing the data.
This results in the threads "racing" to get to the data first, resulting in different outcomes depending on who wins the race.
For example, a line of code like `x += 1;` is actually compiled to two operations: we first load the value of `x`, and then we store that value plus one back into the location `x`.
If we have two threads both running `x += 1`, then there is a race condition---both threads are trying to update the data, and depending on how the threads are scheduled, it could be that after both threads are done, `x` has only been increased by one.
This can happen if the two threads context switch in the following sequence: load `x` in thread 1, load `x` in thread 2, store back to `x` in thread 1, and store back to `x` in thread 2.
The threads never "saw" the increment in the other thread, thus overwriting the other thread's work.

A deadlock is when multiple threads wait for each other, resulting in a "cycle" of waiting where no progress can be made, and the program is stuck.
For example, if thread 1 cannot send a message until thread 2 sends it a message, and thread 2 cannot send a message until thread 1 sends it a message, then there is a deadlock.

In the rest of these notes, we will cover synchronization tools and how they can be used to solve and avoid these issues.

### `mutex`

A `mutex` (in the `<mutex>` header), also commonly called a *lock*, is a common synchronization mechanism.
A `thread` can `.lock()` a `mutex`, *owning* that `mutex`.
If another `thread` already *owns* a `mutex`, calling `.lock()` on the `mutex` will block until the `mutex` is no longer owned, at which time one of the waiting `thread`s will stop blocking and obtain ownership of the `mutex`.
To release ownership, the `thread` owning the `mutex` must call `.unlock()`.
The code between a `.lock()` and `.unlock()` is called a *critical section*.
Multiple threads cannot be in the critical section concurrently.

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/mutex.cpp#L1-L30

### Locking `mutex`s

Using `.lock()` and `.unlock()` on a mutex is not ideal.
Like using `new` and `delete` manually, this results in no exception safety guarantee:

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/mutex_exception.cpp#L1-L29

When the exception is thrown, the `.unlock()` call is skipped, and this results in the other `thread` waiting for the `mutex` forever.
Like memory, a `mutex` is a resource, so using RAII to acquire and release the resource is essential here.
`scoped_lock` is an RAII class that does this.
It locks a `mutex` on construction and unlocks it on destruction:

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/scoped_lock.cpp#L1-L29

This time, the `mutex` will always be unlocked, no matter how we exit the code block.

Another problem with `mutex` is the possibility of deadlock.
A deadlock occurs when multiple `thread`s acquire and hold a `mutex` while waiting for another `mutex`:

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/deadlock.cpp#L1-L46

The classic solution to this is to require all threads to acquire `mutex`s in the same order.
Rather than inspecting the code, an easy way to do this is to use a single `scoped_lock`.
`scoped_lock` can take multiple `mutex`s as arguments in its constructor, which will use a deadlock-avoiding algorithm to acquire them all.
This algorithm will never block trying to acquire one of the `mutex`s while owning another of the `mutex`s, avoiding deadlock.

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/scoped_lock_deadlock.cpp#L1-L46

### Sharing With Reader and Writer Threads

A common pattern in concurrent code is that there are some `thread`s that create data, and some other `thread`s that read that data.
While there would be a data race if one `thread` accesses the data while another is updating it, there is no danger in having multiple reader `thread`s read the data at the same time.
`shared_mutex` is a synchronization mechanism for this situation.
It has two levels of locking, a *shared* level and an *exclusive* level.
Multiple `thread`s can all own the `shared_mutex` at the shared level, but only if the exclusive-level lock is not acquired.
Only one `thread` can acquire the `shared_mutex` at the exclusive level at a time, and only if the `shared_mutex` is not acquired.
To acquire the `shared_mutex` at the different levels, we have RAII classes like `scoped_lock`, `shared_lock` and `unique_lock` for the shared and exclusive levels respectively.

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/shared_mutex.cpp#L1-L44

In this example, multiple reader threads are in the critical section at the same time, which you can see by the occasionally interleaved prints.

### Waiting Using Condition Variables

`mutex`s allow us to block until an exclusive resource is available, but what if we want to wait for some other condition to occur?
One option is to use a loop to continually check the condition: `while (condition == false) {}`.
If the code makes it past this loop, then `condition` must be true!
Unfortunately, this is both inefficient---this `thread` is stuck checking the condition over and over, wasting CPU time---and it doesn't actually work!
`condition` is not protected by a `mutex`, so its value may only be `true` for a moment, and then get changed back to `false` by another `thread` once we leave the loop.
If it were protected by a `mutex`, that `mutex` would have to be continually held, so no other thread could acquire it to safely update `condition`.

`condition_variable`s (in the `<condition_variable>` header) are a synchronization mechanism for threads to wait until another thread informs it that some condition has become true, and that it can proceed, effectively solving both the issues outlined above.
A `condition_variable` is used together with a `mutex`, in order to protect the condition that they are waiting on.
The `thread` that is meant to wait on a condition should first acquire the `mutex` using a `unique_lock`.
Inside the critical section, check the condition, and if it is not yet true, we can wait on it using the `condition_variable` by calling `.wait(lock)`.
We pass the `unique_lock` to this function, which releases the lock and suspends the `thread` until it is signalled by another thread to "wake up".

To wake up `thread`s waiting on `condition_variable`s, a `thread` that changes the condition can call `.notify_one()` or `.notify_all()` to wake up one or all waiting `thread`s on that `condition_variable`.
When a thread wakes up, it will acquire the `unique_lock` again to leave the `.wait()`, to ensure that there is still only one `thread` in the critical section.

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/condition_variable.cpp#L1-L46

In this example, note that we check the condition using a while loop.
This is to guard against *spurious wakeup*, where the condition variable wakes up, but the condition is not actually true yet.
This can happen if another thread uses `.notify_all()` and a different thread woke up and changed the condition first, or if the thread who woke up you did not actually update the condition.
Try adding more consumers, changing the while loop to an if, and changing `.notify_one()` to `.notify_all()` to see this happen.
The consumer threads all get woken up, but the condition cannot be true for all of them, so a thread will then try to pop an empty queue.

### Semaphores

An alternative to using `mutex` and `condition_variable` to wait on conditions that are based on integers is to use a `counting_semaphore` (in the `<semaphore>` header).
This synchronization primitive keeps track of a number, and provides operations to increment (`.release()`) and decrement (`.acquire()`) the number.
If you try to decrement below 0, the `.acquire()` operation blocks.
This is useful to signal to waiting threads that something new is available, by incrementing the count.

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/semaphore.cpp#L1-L39

This is useful for allowing multiple threads into a critical section, not just one.
In this example, we use the semaphore to represent how many values are available to consume in the shared `queue`, and that many consumers are allowed in at the same time.

In addition to `counting_semaphore`, there is another type of semaphore called a `binary_semaphore` which has a maximum number of 1.
This class is useful for signalling boolean conditions, rather than numerical ones.

Unfortunately, if you are on GCC 10 (like me), semaphores (along with barriers and latches below) are not yet implemented.
clang or newer version of GCC should support them.

### Barriers and Latches

The final synchronization mechanisms that we'll see are `barrier`s and `latch`s (in the `<barrier>` and `<latch>` headers respectively).

A `barrier` requires a set number of threads to arrive before these threads can continue running.
`thread`s must call `.arrive_and_wait()` to increment the count of waiting threads.
Once enough `thread`s have arrived, the waiting `thread`s all stop blocking and a function attached to the `barrier` is called.

https://github.com/CIS1900/2022-fall/blob/4162567a1ad3c63722e4261090519c50e0ad0410/08/barrier.cpp#L1-L26

`latch` is a very similar class, except that threads can increment the count multiple times.

## Conclusion

These notes have *introduced* some basic concurrency features provided by the C++ standard library.
Even though there was a lot of material, I've left out many of the more complex concurrency topics like `std::atomic` and the C++ memory model.
Concurrency is a very large and complex topic, and it is unlikely you understood everything if this is your first time seeing it, which is ok!
Many of these topics will be revisited in courses like CIS 3800, and this material is not crucial to know for using C++.
