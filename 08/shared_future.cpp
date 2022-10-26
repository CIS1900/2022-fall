#include <future>
#include <thread>
#include <iostream>

using namespace std;

void producer(int i, promise<int> & p)
{
    p.set_value(i + 1);
}

// this could also take a non-reference---shared_futures can be copied as well
void consumer(shared_future<int> & result)
{
    cerr << result.get();
}

int main()
{
    promise<int> p;
    auto result {p.get_future()};
    auto shared {result.share()};

    jthread t {producer, 12, ref(p)};

    jthread t1 {consumer, ref(shared)};
    jthread t2 {consumer, ref(shared)};
    jthread t3 {consumer, ref(shared)};
}
