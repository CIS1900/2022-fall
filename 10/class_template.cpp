#include <iostream>

using namespace std;

template <typename T>
class wrapper
{
    T t;
public:
    wrapper(T t) : t {t} {}

    T get() const
    {
        return t;
    }

    void set(T t)
    {
        this->t = t;
    }
};

int main()
{
    wrapper w {12}; // again, could be explicit as wrapper<int> w {12};

    w.set(100);
    cout << w.get();
}
