#include <semaphore>
#include <queue>
#include <iostream>
#include <thread>

using namespace std;

counting_semaphore sem {0};
queue<int> q;

void producer()
{
	for(int i {0}; i < 100; ++i)
	{
		q.push(i);
		sem.release(); // signal that there is a value in the queue ready
	}
}

void consumer()
{
	while(true)
	{
		sem.acquire();
		auto val {q.front()};
		q.pop();
		cerr << val << "\n";
	}
}

int main()
{
	jthread p1 {producer};
	jthread p2 {producer};

	jthread c1 {consumer};
	jthread c2 {consumer};
	jthread c3 {consumer};
}
