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
        // we are not writing using std::swap here because we do _not_ want std::swap, which would result in an infinite loop.
        swap(*this, i); // since the arguments are integers, this swap chooses the friend swap associated with integer

        std::cerr << "=\n";
        return *this;
    }

    friend void swap(integer & i, integer & j) noexcept
    {
        using std::swap; // makes std::swap accessible as just swap. If i.p was of some unknown type where we don't know if there is a custom swap available for it, this will allow C++ to fall back to std::swap if there is no custom swap available.
        swap(i.p, j.p);
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
