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
    integer k {i.get() + j.get()};

    cout << k.get() << "\n";
}
