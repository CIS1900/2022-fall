#include <iostream>
#include <utility>

using namespace std;

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

    integer & operator=(const integer & i)
    {
        integer temp {i}; // copy

        std::swap(p, temp.p);

        std::cerr << "copy=\n";
        return *this;
    }
    integer & operator=(integer && i)
    {
        integer temp {std::move(i)}; // move

        std::swap(p, temp.p);

        std::cerr << "move=\n";
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
