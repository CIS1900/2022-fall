#include <iostream>
#include <concepts>

using namespace std;

class foo
{
public:
    foo() {};
    foo & operator=(const foo &) = delete; // disable copy assignment
};

// same as previous example, but with this new type constraint
template <typename T>
requires copyable<T>
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
    wrapper<foo> w {foo()};
    w.set(foo());
}
