#include <iostream>
#include <memory>

using namespace std;

class list
{
public:
    // left the members public for convenience
    int val;
    unique_ptr<list> next;

    list(int val) : val {val}
    {}

    list(const list & other) :
        val {other.val},
        next {other.next ? make_unique<list>(*other.next) : nullptr}
    {}

    // the implicitly defined move ctor moves every member, which is what we want
    list(list && other) = default;

    list & operator=(const list & other)
    {
        val = other.val;

        // unlike last week's code, this recursively calls itself as long as both lists have enough nodes
        if (other.next)
        {
            if (next)
                *next = *(other.next);
            else
                next = make_unique<list>(*other.next);
        }
        else
            next = nullptr;

        return *this;
    }

    // same as the move ctor, the implicit one is what we want
    list & operator=(list && other) = default;

    void print() const
    {
        std::cout << val << " ";
        if (next)
            next->print();
    }
};

int main()
{
}
