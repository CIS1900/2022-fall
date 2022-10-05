#include <iostream>

using std::cout;

class integer
{
private:
    int * p = nullptr;
public:
    integer(int i) : p {new int {i}}
    {
        cout << "ctor\n";
    }

    integer(const integer & other) : p {new int {other.get()}}
    {
        cout << "copy ctor\n";
    }

    integer(integer && other) : p {other.p}
    {
        other.p = nullptr;
        cout << "move ctor\n";
    }

    virtual ~integer()
    {
        delete p;
        cout << "dtor\n";
    }

    integer & operator=(const integer & other)
    {
        *p = other.get();
        cout << "copy=\n";
        return *this;
    }

    int get() const
    {
        return *p;
    }
    void set(int i)
    {
        *p = i;
    }
};

integer incr(integer i)
{
    i.set(i.get() + 1);
    return i;
}

int main()
{
    integer i {1};
    integer j {incr(i)};

    cout << j.get() << "\n";
}
