#include <iostream>
#include <vector>
#include <functional>

using namespace std;

class functor
{
public:
    bool operator()(int i)
    {
        return i == 0;
    }
};

bool f2(int i)
{
    return i == 1;
}

int main()
{
    functor f1;

    vector<function<bool(int)>> funcs {f1, f2, [](int i){ return i == 2; }};

    for (auto & f : funcs)
        cout << f(1) << " ";
}
