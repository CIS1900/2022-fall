#include <iostream>
#include <concepts>

using namespace std;

class less_than
{
    int limit;
public:
    less_than(int limit) : limit {limit} {}

    bool operator()(int i) { return i < limit; }
};

bool foo(int i)
{
	return i == 1;
}

int main()
{
    less_than c1 {10};
    cout << c1(11) << " " << c1(9) << "\n";

    cout << foo(11) << " " << foo(1) << "\n";
}
