#include <iostream>

using namespace std;

template<typename T, int N>
class array
{
public:
    using value_type = T;

    T val[N];

    int size()
    {
        return N;
    }
};

int main()
{
    array<int, 10> t;
    cout << t.size() << "\n";
}
