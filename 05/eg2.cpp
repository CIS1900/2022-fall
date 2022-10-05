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

    // Constructors can use other constructors in their initialization list.
    // It would be nicer and repeat less code if we reused the other
    // constructor, by doing : integer {i.get()}, but that would print
    // an extra "ctor".
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

int main()
{
    integer i {1};
    integer j {2};
    integer k {i};

    cout << k.get() << "\n";
}
