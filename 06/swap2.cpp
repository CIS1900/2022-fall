#include <iostream>
#include <utility>

class integer
{
private:
    int * p = nullptr;
public:
    integer(int i) : p {new int {i}}
    {
        std::cerr << "ctor\n";
    }

    integer(const integer & i) : p {new int {i.get()}}
    {
        std::cerr << "copy ctor\n";
    }
    integer(integer && i) : p {i.p}
    {
        i.p = nullptr;
        std::cerr << "move ctor\n";
    }
    ~integer()
    {
        delete p;
        std::cerr << "dtor\n";
    }

    integer & operator=(integer i)
    {
        std::swap(p, i.p);

        std::cerr << "=\n";
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
    integer j {2};

    j = incr(i);

    std::cerr << j.get() << "\n";
}
