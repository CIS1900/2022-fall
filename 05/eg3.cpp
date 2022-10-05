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

    virtual ~integer()
    {
        delete p;
        cout << "dtor\n";
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
